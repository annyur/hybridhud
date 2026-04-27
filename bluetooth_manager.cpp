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

/* ================================================================
 *  iCar Pro 2S GATT UUID
 * ================================================================ */
#define ICAR_SERVICE_UUID        "000018f0-0000-1000-8000-00805f9b34fb"
#define ICAR_READ_CHAR_UUID      "00002af0-0000-1000-8000-00805f9b34fb"
#define ICAR_WRITE_CHAR_UUID     "00002af1-0000-1000-8000-00805f9b34fb"

/* ================================================================
 *  扫描结果缓存
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

/* 连接状态 */
static BLEClient   *s_client = NULL;
static bool         s_is_connected = false;
static bool         s_ble_enabled = false;
static char         s_conn_addr[BT_ADDR_LEN] = "";
static char         s_conn_name[BT_NAME_LEN] = "";
static int          s_connecting_idx = -1;

/* 异步连接请求 */
static volatile int s_pending_action = 0;   /* 0=无, 1=connect, 2=auto_reconnect */
static int          s_pending_idx = -1;

/* GATT 特征 */
static BLERemoteCharacteristic *s_write_char = NULL;
static BLERemoteCharacteristic *s_read_char  = NULL;

/* 后台任务句柄 */
static TaskHandle_t s_bt_task = NULL;

/* ================================================================
 *  前置声明
 * ================================================================ */
static void refresh_device_list(void);
static void do_disconnect(bool full_cleanup);
static void do_connect_real(int idx);
static void do_auto_reconnect_real(void);
static void set_ble_enabled(bool on);
static void load_ble_state(void);
static void save_ble_state(void);
static void on_scan(lv_event_t *e);
static void start_advertising(void);

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

static void save_last_device(const char *addr, const char *name)
{
    bt_prefs.begin("bt_prefs", false);
    bt_prefs.putString("last_addr", addr);
    bt_prefs.putString("last_name", name);
    bt_prefs.end();
}

static bool load_last_device(char *addr_out, char *name_out)
{
    bt_prefs.begin("bt_prefs", true);
    String a = bt_prefs.getString("last_addr", "");
    String n = bt_prefs.getString("last_name", "");
    bt_prefs.end();
    if (a.length() == 0) return false;
    strncpy(addr_out, a.c_str(), BT_ADDR_LEN - 1);
    addr_out[BT_ADDR_LEN - 1] = '\0';
    strncpy(name_out, n.c_str(), BT_NAME_LEN - 1);
    name_out[BT_NAME_LEN - 1] = '\0';
    return true;
}

/* ================================================================
 *  BLE 客户端回调
 * ================================================================ */
class BTClientCallbacks : public BLEClientCallbacks {
    void onConnect(BLEClient* pclient) override {
        (void)pclient;
        s_is_connected = true;
        s_connecting_idx = -1;
        Serial.println("[BT] onConnect callback");
    }
    void onDisconnect(BLEClient* pclient) override {
        (void)pclient;
        s_is_connected = false;
        s_conn_addr[0] = '\0';
        s_conn_name[0] = '\0';
        s_write_char = NULL;
        s_read_char  = NULL;
        Serial.println("[BT] onDisconnect callback");
    }
};

/* ================================================================
 *  GATT 发现 + ELM327 初始化
 * ================================================================ */
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
    Serial.println("[BT-ELM] Service found OK");

    s_write_char = svc->getCharacteristic(ICAR_WRITE_CHAR_UUID);
    s_read_char  = svc->getCharacteristic(ICAR_READ_CHAR_UUID);

    if (!s_write_char || !s_read_char) {
        Serial.println("[BT-ELM] Characteristic missing");
        return false;
    }

    if (s_read_char->canNotify()) {
        s_read_char->registerForNotify(on_notify);
    }

    Serial.println("[BT-ELM] GATT ready, send ATZ...");
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
 *  统一 BLE 开关控制
 * ================================================================ */
static void set_ble_enabled(bool on)
{
    if (on == s_ble_enabled) return;

    if (on) {
        BLEDevice::init("HybridHUD");
        s_ble_enabled = true;
        save_ble_state();
        Serial.println("[BT] BLE ON");
        if (s_ui && s_ui->bluetooth_bt_sw_enable) {
            lv_obj_add_state(s_ui->bluetooth_bt_sw_enable, LV_STATE_CHECKED);
        }

        /* 开启后尝试回连上次设备 */
        char addr[BT_ADDR_LEN], name[BT_NAME_LEN];
        if (load_last_device(addr, name)) {
            Serial.printf("[BT] Queue auto-reconnect to %s\n", name);
            s_pending_action = 2;
        } else {
            if (s_ui && s_ui->bluetooth_bt_list_devices) {
                lv_obj_clean(s_ui->bluetooth_bt_list_devices);
                lv_obj_t *btn = lv_list_add_btn(s_ui->bluetooth_bt_list_devices,
                                                LV_SYMBOL_BLUETOOTH, "Scanning...");
                lv_obj_clear_flag(btn, LV_OBJ_FLAG_CLICKABLE);
            }
            on_scan(NULL);
        }
    } else {
        if (s_scanning && pBLEScan) {
            pBLEScan->stop();
            s_scanning = false;
        }
        do_disconnect(true);
        s_ble_enabled = false;
        save_ble_state();
        Serial.println("[BT] BLE OFF");

        if (s_ui && s_ui->bluetooth_bt_list_devices) {
            lv_obj_clean(s_ui->bluetooth_bt_list_devices);
            lv_obj_t *btn = lv_list_add_btn(s_ui->bluetooth_bt_list_devices,
                                              LV_SYMBOL_BLUETOOTH, "Bluetooth OFF");
            lv_obj_clear_flag(btn, LV_OBJ_FLAG_CLICKABLE);
        }
        if (s_ui && s_ui->bluetooth_bt_sw_enable) {
            lv_obj_clear_state(s_ui->bluetooth_bt_sw_enable, LV_STATE_CHECKED);
        }
    }
}

/* ================================================================
 *  连接 / 断开（在后台任务中执行，不阻塞主 loop）
 * ================================================================ */
static void do_disconnect(bool full_cleanup)
{
    s_write_char = NULL;
    s_read_char  = NULL;

    if (s_client) {
        if (s_client->isConnected()) {
            s_client->disconnect();
        }
        if (full_cleanup) {
            delete s_client;
            s_client = NULL;
        }
    }
    s_is_connected = false;
    s_conn_addr[0] = '\0';
    s_conn_name[0] = '\0';
    s_connecting_idx = -1;
}

static void do_connect_real(int idx)
{
    if (idx < 0 || idx >= s_device_count) return;

    const char *target_addr = s_devices[idx].addr;
    const char *target_name = s_devices[idx].name;
    esp_ble_addr_type_t target_type = s_devices[idx].addr_type;

    Serial.printf("[BT] Connect to idx=%d name=%s addr=%s type=%d\n",
                  idx, target_name, target_addr, (int)target_type);

    do_disconnect(true);

    s_client = BLEDevice::createClient();
    s_client->setClientCallbacks(new BTClientCallbacks());

    if (s_client->connect(BLEAddress(target_addr), target_type)) {
        strncpy(s_conn_addr, target_addr, BT_ADDR_LEN - 1);
        s_conn_addr[BT_ADDR_LEN - 1] = '\0';
        strncpy(s_conn_name, target_name, BT_NAME_LEN - 1);
        s_conn_name[BT_NAME_LEN - 1] = '\0';
        save_last_device(target_addr, target_name);
        Serial.printf("[BT] Connected: %s\n", target_name);
        discover_services();
    } else {
        Serial.println("[BT] Connect FAILED");
        s_connecting_idx = -1;
        if (s_client) {
            delete s_client;
            s_client = NULL;
        }
    }

    /* 后台任务中不能直接操作 LVGL，设标志让 update() 刷新 */
    s_scan_done = true;
}

static void do_auto_reconnect_real(void)
{
    char addr[BT_ADDR_LEN], name[BT_NAME_LEN];
    if (!load_last_device(addr, name)) return;

    Serial.printf("[BT] Auto-reconnect to %s\n", name);

    do_disconnect(true);
    delay(100);

    s_client = BLEDevice::createClient();
    s_client->setClientCallbacks(new BTClientCallbacks());

    if (s_client->connect(BLEAddress(addr))) {
        strncpy(s_conn_addr, addr, BT_ADDR_LEN - 1);
        s_conn_addr[BT_ADDR_LEN - 1] = '\0';
        strncpy(s_conn_name, name, BT_NAME_LEN - 1);
        s_conn_name[BT_NAME_LEN - 1] = '\0';
        s_is_connected = true;
        Serial.println("[BT] Auto-reconnect OK");
        discover_services();
    } else {
        Serial.println("[BT] Auto-reconnect FAILED");
        if (s_client) {
            delete s_client;
            s_client = NULL;
        }
    }

    s_scan_done = true;
}

/* ================================================================
 *  后台 FreeRTOS 任务 — 处理连接，不阻塞主 loop
 * ================================================================ */
static void bt_task(void *pv)
{
    (void)pv;
    while (1) {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);  /* 等待通知 */

        if (s_pending_action == 1) {
            s_pending_action = 0;
            do_connect_real(s_pending_idx);
        } else if (s_pending_action == 2) {
            s_pending_action = 0;
            do_auto_reconnect_real();
        }
    }
}

static void notify_bt_task(void)
{
    if (s_bt_task) {
        xTaskNotifyGive(s_bt_task);
    }
}

/* ================================================================
 *  BLE 扫描回调
 * ================================================================ */
class BTScanCallbacks : public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) override {
        if (s_device_count >= BT_MAX_DEVICES) return;
        bt_device_t *d = &s_devices[s_device_count];

        if (advertisedDevice.haveName()) {
            String n = advertisedDevice.getName().c_str();
            if (n.indexOf("Vlink") >= 0 || n.indexOf("OBD") >= 0 ||
                n.indexOf("iCar") >= 0 || n.indexOf("vgate") >= 0) {
                strncpy(d->name, advertisedDevice.getName().c_str(), BT_NAME_LEN - 1);
            } else {
                strncpy(d->name, advertisedDevice.getName().c_str(), BT_NAME_LEN - 1);
            }
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
}

/* ================================================================
 *  列表项点击 — 只设标志，唤醒后台任务
 * ================================================================ */
static void on_list_item_clicked(lv_event_t *e)
{
    int idx = (int)(intptr_t)lv_event_get_user_data(e);
    if (idx < 0 || idx >= s_device_count) return;

    if (s_is_connected && strcmp(s_conn_addr, s_devices[idx].addr) == 0) {
        Serial.println("[BT] Clicked connected -> disconnect");
        do_disconnect(true);
        refresh_device_list();
        return;
    }

    s_connecting_idx = idx;
    s_pending_action = 1;
    s_pending_idx = idx;
    Serial.printf("[BT] Queue connect idx=%d\n", idx);
    notify_bt_task();  /* 唤醒后台任务 */
    refresh_device_list();  /* 立即显示 ... */
}

/* ================================================================
 *  UI 刷新列表
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
    set_ble_enabled(checked);
}

static void on_scan(lv_event_t *e)
{
    (void)e;
    if (!s_ui) return;

    if (!s_ble_enabled) {
        set_ble_enabled(true);
        return;
    }

    if (s_scanning && pBLEScan) {
        pBLEScan->stop();
        s_scanning = false;
        delay(50);
    }

    s_device_count = 0;
    s_scan_done    = false;

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
    Serial.println("[BT] Scan started (5s)");
}

/* ================================================================
 *  外部接口
 * ================================================================ */
void bluetooth_manager_init(lv_ui *ui)
{
    s_ui = ui;

    load_ble_state();

    if (ui->bluetooth_btn_back) {
        lv_obj_add_event_cb(ui->bluetooth_btn_back, on_back, LV_EVENT_CLICKED, NULL);
    }
    if (ui->bluetooth_bt_sw_enable) {
        lv_obj_add_event_cb(ui->bluetooth_bt_sw_enable, on_switch, LV_EVENT_VALUE_CHANGED, NULL);
        if (s_ble_enabled) {
            lv_obj_add_state(ui->bluetooth_bt_sw_enable, LV_STATE_CHECKED);
        } else {
            lv_obj_clear_state(ui->bluetooth_bt_sw_enable, LV_STATE_CHECKED);
        }
    }
    if (ui->bluetooth_bt_btn_scan) {
        lv_obj_add_event_cb(ui->bluetooth_bt_btn_scan, on_scan, LV_EVENT_CLICKED, NULL);
    }

    /* 创建后台连接任务 */
    if (s_bt_task == NULL) {
        xTaskCreatePinnedToCore(bt_task, "bt_task", 4096, NULL, 1, &s_bt_task, 0);
    }

    /* 如果记忆是开启，直接初始化 BLE 并回连/扫描 */
    if (s_ble_enabled) {
        s_ble_enabled = false;
        set_ble_enabled(true);
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

    /* 同步开关视觉状态 */
    if (s_ui->bluetooth_bt_sw_enable) {
        if (s_ble_enabled) {
            lv_obj_add_state(s_ui->bluetooth_bt_sw_enable, LV_STATE_CHECKED);
        } else {
            lv_obj_clear_state(s_ui->bluetooth_bt_sw_enable, LV_STATE_CHECKED);
        }
    }

    char addr[BT_ADDR_LEN], name[BT_NAME_LEN];
    if (s_ble_enabled && load_last_device(addr, name)) {
        char buf[64];
        if (s_is_connected && strcmp(s_conn_addr, addr) == 0) {
            snprintf(buf, sizeof(buf), "\xE2\x97\x8F %s  [Connected]", name);
        } else {
            snprintf(buf, sizeof(buf), "Last: %s", name);
        }
        lv_obj_t *btn = lv_list_add_btn(s_ui->bluetooth_bt_list_devices,
                                        LV_SYMBOL_BLUETOOTH, buf);
        lv_obj_clear_flag(btn, LV_OBJ_FLAG_CLICKABLE);
        if (s_is_connected) {
            lv_obj_set_style_text_color(btn, lv_color_hex(0x00e676), LV_PART_MAIN);
        }
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

/* 在主 loop 中轮询：处理扫描结果 + 连接完成刷新 */
void bluetooth_manager_update(void)
{
    if (s_scan_done) {
        s_scan_done = false;
        refresh_device_list();
    }
}