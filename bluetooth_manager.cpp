#include "bluetooth_manager.h"
#include <Arduino.h>
#include <Preferences.h>

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

#define BT_MAX_DEVICES  12
#define BT_NAME_LEN     32
#define BT_ADDR_LEN     18

#define ICAR_SERVICE_UUID        "000018f0-0000-1000-8000-00805f9b34fb"
#define ICAR_READ_CHAR_UUID      "00002af0-0000-1000-8000-00805f9b34fb"
#define ICAR_WRITE_CHAR_UUID     "00002af1-0000-1000-8000-00805f9b34fb"

/* ================================================================
 *  数据结构
 * ================================================================ */
typedef struct {
    char name[BT_NAME_LEN];
    char addr[BT_ADDR_LEN];
    esp_ble_addr_type_t addr_type;
} bt_device_t;

static bt_device_t  s_devices[BT_MAX_DEVICES];
static int          s_device_count = 0;
static volatile bool s_scan_done   = false;
static volatile bool s_scanning    = false;

static lv_ui       *s_ui = NULL;
static app_switch_cb_t s_switch_cb = NULL;
static BLEScan     *pBLEScan = NULL;
static Preferences  bt_prefs;

static BLEClient   *s_client = NULL;
static bool         s_is_connected = false;
static bool         s_ble_enabled = false;
static char         s_conn_addr[BT_ADDR_LEN] = "";
static char         s_conn_name[BT_NAME_LEN] = "";
static int          s_connecting_idx = -1;
static int          s_target_idx = -1;

static BLERemoteCharacteristic *s_write_char = NULL;
static BLERemoteCharacteristic *s_read_char  = NULL;

/* 自动连接标志：扫描完成后匹配上次设备名自动连接 */
static bool s_auto_conn_after_scan = false;
static char s_auto_conn_name[BT_NAME_LEN] = "";

/* 状态机 */
static int s_state = 0;
#define STATE_IDLE      0
#define STATE_INIT      1
#define STATE_RECONNECT 2
#define STATE_SCAN      3
#define STATE_CONNECT   4

/* ================================================================
 *  前置声明
 * ================================================================ */
static void refresh_device_list(void);
static void on_list_item_clicked(lv_event_t *e);
static void start_scan(void);
static void do_connect(void);
static void do_auto_reconnect(void);
static void do_disconnect(bool full_cleanup);

/* ================================================================
 *  NVS
 * ================================================================ */
static void save_ble_state(void)
{
    bt_prefs.begin("bt_prefs", false);
    bt_prefs.putBool("enabled", s_ble_enabled);
    bt_prefs.end();
}

static void load_ble_state(void)
{
    bt_prefs.begin("bt_prefs", true);
    bool enabled = bt_prefs.getBool("enabled", true);
    bt_prefs.end();
    s_ble_enabled = enabled;
}

static void save_last_device(const char *addr, const char *name, uint8_t type)
{
    bt_prefs.begin("bt_prefs", false);
    bt_prefs.putString("last_addr", addr);
    bt_prefs.putString("last_name", name);
    bt_prefs.putUChar("last_type", type);
    bt_prefs.end();
}

static bool load_last_device(char *addr_out, char *name_out, uint8_t *type_out)
{
    bt_prefs.begin("bt_prefs", true);
    String a = bt_prefs.getString("last_addr", "");
    String n = bt_prefs.getString("last_name", "");
    uint8_t t = bt_prefs.getUChar("last_type", BLE_ADDR_TYPE_PUBLIC);
    bt_prefs.end();
    if (a.length() == 0) return false;
    strncpy(addr_out, a.c_str(), BT_ADDR_LEN - 1);
    addr_out[BT_ADDR_LEN - 1] = '\0';
    strncpy(name_out, n.c_str(), BT_NAME_LEN - 1);
    name_out[BT_NAME_LEN - 1] = '\0';
    *type_out = t;
    return true;
}

/* ================================================================
 *  BLE 回调
 * ================================================================ */
class BTClientCallbacks : public BLEClientCallbacks {
    void onConnect(BLEClient* pclient) override {
        (void)pclient;
        s_is_connected = true;
        s_connecting_idx = -1;
        Serial.println("[BT] onConnect");
    }
    void onDisconnect(BLEClient* pclient) override {
        (void)pclient;
        s_is_connected = false;
        s_conn_addr[0] = '\0';
        s_conn_name[0] = '\0';
        s_write_char = NULL;
        s_read_char  = NULL;
        Serial.println("[BT] onDisconnect");
    }
};

static void on_notify(BLERemoteCharacteristic *pChar, uint8_t *pData, size_t length, bool isNotify)
{
    (void)pChar; (void)isNotify;
    char buf[128];
    size_t copy_len = length < sizeof(buf)-1 ? length : sizeof(buf)-1;
    memcpy(buf, pData, copy_len);
    buf[copy_len] = '\0';
    Serial.printf("[BT-ELM] RX: %s\n", buf);
}

static bool discover_services(void)
{
    if (!s_client || !s_client->isConnected()) return false;

    BLERemoteService *svc = s_client->getService(ICAR_SERVICE_UUID);
    if (!svc) {
        Serial.println("[BT-ELM] Service not found");
        return false;
    }
    Serial.println("[BT-ELM] Service found");

    s_write_char = svc->getCharacteristic(ICAR_WRITE_CHAR_UUID);
    s_read_char  = svc->getCharacteristic(ICAR_READ_CHAR_UUID);

    if (!s_write_char || !s_read_char) {
        Serial.println("[BT-ELM] Characteristic missing");
        return false;
    }

    if (s_read_char->canNotify()) {
        s_read_char->registerForNotify(on_notify);
    }

    Serial.println("[BT-ELM] Send ATZ...");
    s_write_char->writeValue("ATZ\r");
    delay(500);
    s_write_char->writeValue("ATE0\r");
    delay(100);
    s_write_char->writeValue("ATL1\r");
    delay(100);
    s_write_char->writeValue("010C\r");
    return true;
}

/* ================================================================
 *  连接 / 断开 / 回连（同步阻塞，由状态机调度）
 * ================================================================ */
static void do_disconnect(bool full_cleanup)
{
    s_write_char = NULL;
    s_read_char  = NULL;
    if (s_client) {
        if (s_client->isConnected()) s_client->disconnect();
        if (full_cleanup) { delete s_client; s_client = NULL; }
    }
    s_is_connected = false;
    s_conn_addr[0] = '\0';
    s_conn_name[0] = '\0';
    s_connecting_idx = -1;
}

static void do_connect(void)
{
    if (s_target_idx < 0 || s_target_idx >= s_device_count) {
        s_state = STATE_IDLE;
        return;
    }

    const char *addr = s_devices[s_target_idx].addr;
    const char *name = s_devices[s_target_idx].name;
    esp_ble_addr_type_t type = s_devices[s_target_idx].addr_type;

    Serial.printf("[BT] Connect to %s (%s) type=%d\n", name, addr, (int)type);

    do_disconnect(true);

    s_client = BLEDevice::createClient();
    s_client->setClientCallbacks(new BTClientCallbacks());

    if (s_client->connect(BLEAddress(addr), type)) {
        strncpy(s_conn_addr, addr, BT_ADDR_LEN - 1);
        s_conn_addr[BT_ADDR_LEN - 1] = '\0';
        strncpy(s_conn_name, name, BT_NAME_LEN - 1);
        s_conn_name[BT_NAME_LEN - 1] = '\0';
        save_last_device(addr, name, (uint8_t)type);
        Serial.printf("[BT] Connected: %s\n", name);
        discover_services();
    } else {
        Serial.println("[BT] Connect FAILED");
        s_connecting_idx = -1;
        if (s_client) { delete s_client; s_client = NULL; }
    }

    s_state = STATE_IDLE;
    refresh_device_list();
}

static void do_auto_reconnect(void)
{
    char addr[BT_ADDR_LEN], name[BT_NAME_LEN];
    uint8_t type = BLE_ADDR_TYPE_PUBLIC;
    if (!load_last_device(addr, name, &type)) {
        Serial.println("[BT] No last device, will scan");
        s_state = STATE_SCAN;
        return;
    }

    Serial.printf("[BT] Auto-reconnect to %s (type=%d)\n", name, (int)type);

    do_disconnect(true);
    delay(100);

    s_client = BLEDevice::createClient();
    s_client->setClientCallbacks(new BTClientCallbacks());

    if (s_client->connect(BLEAddress(addr), (esp_ble_addr_type_t)type)) {
        strncpy(s_conn_addr, addr, BT_ADDR_LEN - 1);
        s_conn_addr[BT_ADDR_LEN - 1] = '\0';
        strncpy(s_conn_name, name, BT_NAME_LEN - 1);
        s_conn_name[BT_NAME_LEN - 1] = '\0';
        s_is_connected = true;
        Serial.println("[BT] Auto-reconnect OK");
        discover_services();
        s_state = STATE_IDLE;
    } else {
        Serial.println("[BT] Auto-reconnect FAILED, will scan & match by name");
        if (s_client) { delete s_client; s_client = NULL; }
        /* 保存名称用于扫描后匹配 */
        strncpy(s_auto_conn_name, name, BT_NAME_LEN - 1);
        s_auto_conn_name[BT_NAME_LEN - 1] = '\0';
        s_auto_conn_after_scan = true;
        s_state = STATE_SCAN;   /* 扫描后自动匹配连接 */
    }

    refresh_device_list();
}

/* ================================================================
 *  扫描
 * ================================================================ */
class BTScanCallbacks : public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) override {
        if (s_device_count >= BT_MAX_DEVICES) return;
        bt_device_t *d = &s_devices[s_device_count];
        if (advertisedDevice.haveName()) {
            strncpy(d->name, advertisedDevice.getName().c_str(), BT_NAME_LEN - 1);
            d->name[BT_NAME_LEN - 1] = '\0';
        } else {
            String a = advertisedDevice.getAddress().toString();
            snprintf(d->name, BT_NAME_LEN, "[%s]", a.substring(0, 8).c_str());
        }
        strncpy(d->addr, advertisedDevice.getAddress().toString().c_str(), BT_ADDR_LEN - 1);
        d->addr[BT_ADDR_LEN - 1] = '\0';
        d->addr_type = advertisedDevice.getAddressType();
        s_device_count++;
    }
};

static void on_scan_done(BLEScanResults results) {
    (void)results;
    s_scanning = false;
    s_scan_done = true;
    Serial.printf("[BT] Scan done, found=%d\n", s_device_count);
}

static void start_scan(void)
{
    if (!s_ui) return;
    if (!s_ble_enabled) return;

    if (s_scanning && pBLEScan) {
        pBLEScan->stop();
        s_scanning = false;
        delay(50);
    }

    s_device_count = 0;
    s_scan_done = false;

    lv_obj_clean(s_ui->bluetooth_bt_list_devices);
    lv_obj_t *btn = lv_list_add_btn(s_ui->bluetooth_bt_list_devices,
                                    LV_SYMBOL_BLUETOOTH, "Scanning...");
    lv_obj_clear_flag(btn, LV_OBJ_FLAG_CLICKABLE);

    pBLEScan = BLEDevice::getScan();
    pBLEScan->setAdvertisedDeviceCallbacks(new BTScanCallbacks(), true);
    pBLEScan->setActiveScan(true);
    pBLEScan->setInterval(100);
    pBLEScan->setWindow(99);
    pBLEScan->start(5, on_scan_done, false);
    s_scanning = true;
    s_state = STATE_IDLE;
    Serial.println("[BT] Scan started");
}

/* ================================================================
 *  UI 列表
 * ================================================================ */
static void refresh_device_list(void)
{
    if (!s_ui || !s_ui->bluetooth_bt_list_devices) return;

    lv_obj_clean(s_ui->bluetooth_bt_list_devices);

    if (s_device_count == 0) {
        lv_obj_t *btn = lv_list_add_btn(s_ui->bluetooth_bt_list_devices,
                                        LV_SYMBOL_BLUETOOTH, "No devices found");
        lv_obj_clear_flag(btn, LV_OBJ_FLAG_CLICKABLE);
        return;
    }

    for (int i = 0; i < s_device_count; i++) {
        char buf[64];
        if (s_connecting_idx == i) {
            snprintf(buf, sizeof(buf), "... %s", s_devices[i].name);
        } else if (s_is_connected && strcmp(s_conn_addr, s_devices[i].addr) == 0) {
            snprintf(buf, sizeof(buf), "\xE2\x97\x8F %s", s_devices[i].name);
        } else {
            snprintf(buf, sizeof(buf), "  %s", s_devices[i].name);
        }

        lv_obj_t *btn = lv_list_add_btn(s_ui->bluetooth_bt_list_devices,
                                        LV_SYMBOL_BLUETOOTH, buf);
        lv_obj_add_event_cb(btn, on_list_item_clicked, LV_EVENT_CLICKED, (void *)(intptr_t)i);

        if (s_is_connected && strcmp(s_conn_addr, s_devices[i].addr) == 0) {
            lv_obj_set_style_text_color(btn, lv_color_hex(0x00e676), LV_PART_MAIN);
        }
    }
}

/* ================================================================
 *  事件回调
 * ================================================================ */
static void on_back(lv_event_t *e)
{
    (void)e;
    if (s_switch_cb) s_switch_cb(APP_SCREEN_SETTING, false);
}

static void on_switch(lv_event_t *e)
{
    lv_obj_t *sw = lv_event_get_target(e);
    bool checked = lv_obj_has_state(sw, LV_STATE_CHECKED);
    Serial.printf("[BT] Switch -> %d\n", (int)checked);

    if (checked == s_ble_enabled) return;

    if (checked) {
        s_ble_enabled = true;
        save_ble_state();
        s_state = STATE_INIT;   /* 状态机下一帧执行 init */
    } else {
        if (s_scanning && pBLEScan) {
            pBLEScan->stop();
            s_scanning = false;
        }
        do_disconnect(true);
        s_ble_enabled = false;
        s_state = STATE_IDLE;
        save_ble_state();

        if (s_ui && s_ui->bluetooth_bt_list_devices) {
            lv_obj_clean(s_ui->bluetooth_bt_list_devices);
            lv_obj_t *btn = lv_list_add_btn(s_ui->bluetooth_bt_list_devices,
                                              LV_SYMBOL_BLUETOOTH, "Bluetooth OFF");
            lv_obj_clear_flag(btn, LV_OBJ_FLAG_CLICKABLE);
        }
    }
}

static void on_scan_btn(lv_event_t *e)
{
    (void)e;
    if (!s_ble_enabled) {
        /* 蓝牙未开启时，先开启 */
        s_ble_enabled = true;
        save_ble_state();
        if (s_ui && s_ui->bluetooth_bt_sw_enable)
            lv_obj_add_state(s_ui->bluetooth_bt_sw_enable, LV_STATE_CHECKED);
        s_state = STATE_INIT;
        return;
    }
    s_state = STATE_SCAN;
}

static void on_list_item_clicked(lv_event_t *e)
{
    int idx = (int)(intptr_t)lv_event_get_user_data(e);
    if (idx < 0 || idx >= s_device_count) return;

    /* 点击已连接项 -> 断开 */
    if (s_is_connected && strcmp(s_conn_addr, s_devices[idx].addr) == 0) {
        Serial.println("[BT] Disconnect");
        do_disconnect(true);
        refresh_device_list();
        return;
    }

    /* 停止扫描 */
    if (s_scanning && pBLEScan) {
        pBLEScan->stop();
        s_scanning = false;
        delay(50);
    }

    s_connecting_idx = idx;
    s_target_idx = idx;
    s_state = STATE_CONNECT;   /* 状态机下一帧执行 connect */
    refresh_device_list();      /* 立即显示 ... */
}

/* ================================================================
 *  外部接口
 * ================================================================ */
void bluetooth_manager_init(lv_ui *ui)
{
    s_ui = ui;
    load_ble_state();

    if (ui->bluetooth_btn_back)
        lv_obj_add_event_cb(ui->bluetooth_btn_back, on_back, LV_EVENT_CLICKED, NULL);
    if (ui->bluetooth_bt_sw_enable) {
        lv_obj_add_event_cb(ui->bluetooth_bt_sw_enable, on_switch, LV_EVENT_VALUE_CHANGED, NULL);
        if (s_ble_enabled)
            lv_obj_add_state(ui->bluetooth_bt_sw_enable, LV_STATE_CHECKED);
        else
            lv_obj_clear_state(ui->bluetooth_bt_sw_enable, LV_STATE_CHECKED);
    }
    if (ui->bluetooth_bt_btn_scan)
        lv_obj_add_event_cb(ui->bluetooth_bt_btn_scan, on_scan_btn, LV_EVENT_CLICKED, NULL);

    /* 如果记忆是开启，标记状态机（等 update 执行，不在 init 中阻塞） */
    if (s_ble_enabled) {
        Serial.println("[BT] init: will auto-enable");
        s_ble_enabled = false;   /* 重置，让状态机走完整流程 */
        s_state = STATE_INIT;
    }
}

void bluetooth_manager_set_switch_cb(app_switch_cb_t cb)
{
    s_switch_cb = cb;
}

void bluetooth_manager_enter(void)
{
    if (!s_ui || !s_ui->bluetooth_bt_list_devices) return;
    lv_obj_clean(s_ui->bluetooth_bt_list_devices);

    if (s_ui->bluetooth_bt_sw_enable) {
        if (s_ble_enabled)
            lv_obj_add_state(s_ui->bluetooth_bt_sw_enable, LV_STATE_CHECKED);
        else
            lv_obj_clear_state(s_ui->bluetooth_bt_sw_enable, LV_STATE_CHECKED);
    }

    /* 有扫描结果 → 直接显示列表（含已连接标记） */
    if (s_device_count > 0) {
        refresh_device_list();
        return;
    }

    /* 无扫描结果 → 显示提示 */
    char addr[BT_ADDR_LEN], name[BT_NAME_LEN];
    uint8_t type = BLE_ADDR_TYPE_PUBLIC;
    if (s_ble_enabled && load_last_device(addr, name, &type)) {
        char buf[64];
        if (s_is_connected && strcmp(s_conn_addr, addr) == 0)
            snprintf(buf, sizeof(buf), "\xE2\x97\x8F %s [Connected]", name);
        else
            snprintf(buf, sizeof(buf), "Last: %s", name);
        lv_obj_t *btn = lv_list_add_btn(s_ui->bluetooth_bt_list_devices, LV_SYMBOL_BLUETOOTH, buf);
        lv_obj_clear_flag(btn, LV_OBJ_FLAG_CLICKABLE);
        if (s_is_connected)
            lv_obj_set_style_text_color(btn, lv_color_hex(0x00e676), LV_PART_MAIN);
    } else {
        lv_obj_t *btn = lv_list_add_btn(s_ui->bluetooth_bt_list_devices, LV_SYMBOL_BLUETOOTH,
                            s_ble_enabled ? "Press Scan to search" : "Enable Bluetooth first");
        lv_obj_clear_flag(btn, LV_OBJ_FLAG_CLICKABLE);
    }
}

void bluetooth_manager_exit(void)
{
    if (s_scanning && pBLEScan) {
        pBLEScan->stop();
        s_scanning = false;
    }
}

/* 主 loop 调用：状态机驱动 */
void bluetooth_manager_update(void)
{
    /* 扫描完成 -> 刷新列表 */
    if (s_scan_done) {
        s_scan_done = false;
        refresh_device_list();

        /* 扫描后自动匹配上次设备名 */
        if (s_auto_conn_after_scan) {
            s_auto_conn_after_scan = false;
            bool found = false;
            for (int i = 0; i < s_device_count; i++) {
                if (strcmp(s_devices[i].name, s_auto_conn_name) == 0) {
                    Serial.printf("[BT] Match %s idx=%d addr=%s", s_auto_conn_name, i, s_devices[i].addr);
                    s_connecting_idx = i;
                    s_target_idx = i;
                    s_state = STATE_CONNECT;
                    found = true;
                    refresh_device_list();
                    break;
                }
            }
            if (!found) {
                Serial.printf("[BT] Match %s not found", s_auto_conn_name);
            }
            s_auto_conn_name[0] = 0;
        }
    }

    /* 状态机：每帧最多执行一步 */
    switch (s_state) {
        case STATE_INIT:
            Serial.println("[BT] state: INIT");
            BLEDevice::init("HybridHUD");
            s_ble_enabled = true;
            s_state = STATE_RECONNECT;
            break;

        case STATE_RECONNECT:
            Serial.println("[BT] state: RECONNECT");
            do_auto_reconnect();
            break;

        case STATE_SCAN:
            Serial.println("[BT] state: SCAN");
            start_scan();
            break;

        case STATE_CONNECT:
            Serial.println("[BT] state: CONNECT");
            do_connect();
            break;

        default:
            break;
    }
}
