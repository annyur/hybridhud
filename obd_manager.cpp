/* obd_manager.cpp — OBD数据轮询与缓存模块 v1.0
 * 职责: PID循环查询 + ECU DID扫描 + 数据解析 + 缓存池
 * 依赖: bluetooth_manager (提供BLE连接和原始数据收发)
 */
#include "obd_manager.h"
#include "bluetooth_manager.h"
#include <Arduino.h>

/* ================================================================
 *  配置常量
 * ================================================================ */
#define PID_COUNT 15
#define PID_INTERVAL_MS  300   // PID轮询间隔
#define BURST_INTERVAL_MS 5000 // ECU扫描间隔
#define RX_TIMEOUT_MS    1200  // 响应超时
#define INIT_TIMEOUT_MS  3000  // 初始化超时

/* ================================================================
 *  PID命令表
 * ================================================================ */
static const char* s_pid_cmds[PID_COUNT] = {
    "010C", "010D", "0105", "013C", "015B",
    "0101", "0142", "0131", "010B", "010F",
    "0111", "0143", "0162", "0163", "01A6"
};
static const char* s_pid_names[PID_COUNT] = {
    "RPM", "Speed", "Coolant", "OilTemp", "EPA_SOC",
    "MIL", "BattV", "Dist", "MAP", "IAT",
    "Throttle", "AbsLoad", "Torque%", "RefTorque", "Odometer"
};

/* ================================================================
 *  ECU DID扫描配置
 * ================================================================ */
typedef void (*did_parser_t)(uint16_t did, const uint8_t* d, int len);
struct EcuConfig {
    const char* label;
    uint16_t tx_addr, rx_addr;
    const uint16_t* dids;
    int did_count;
    did_parser_t parser;
    bool needs_session;
};

static const uint16_t s_bms_dids[] = {0xF250,0xF251,0xF252,0xF253,0xF254,0xF255,0xF229,0xF228};
static const uint16_t s_sgcm_dids[] = {0x0808,0x0809,0x0812,0x080A};
static const uint16_t s_hpcm_scan_dids[] = {0xF100,0xF101,0xF102,0xF103};
static const uint16_t s_becm_scan_dids[] = {0xF220,0xF221,0xF222,0xF223};
static const uint16_t s_bms_scan_dids[] = {0xF210,0xF211,0xF212,0xF213,0xF214,0xF215,0xF216,0xF217};

static void parse_bms_did(uint16_t did,const uint8_t*d,int len);
static void parse_sgcm_did(uint16_t did,const uint8_t*d,int len);
static void parse_hpcm_scan_did(uint16_t did,const uint8_t*d,int len);
static void parse_becm_scan_did(uint16_t did,const uint8_t*d,int len);
static void parse_bms_scan_did(uint16_t did,const uint8_t*d,int len);

static const EcuConfig s_ecu_list[] = {
    {"BMS",      0x7A1, 0x7A9, s_bms_dids, 8, parse_bms_did, false},
    {"SGCM",     0x7E7, 0x7EF, s_sgcm_dids, 4, parse_sgcm_did, false},
    {"HPCM",     0x7E0, 0x7E8, s_hpcm_scan_dids, 4, parse_hpcm_scan_did, false},
    {"BECM",     0x7E4, 0x7EC, s_becm_scan_dids, 4, parse_becm_scan_did, false},
    {"BMS_SCAN", 0x7A1, 0x7A9, s_bms_scan_dids, 8, parse_bms_scan_did, false},
};
#define ECU_COUNT (sizeof(s_ecu_list)/sizeof(s_ecu_list[0]))

/* ================================================================
 *  模块状态
 * ================================================================ */
static OBDData s_data;
static bool s_running = false;

/* ---- ELM327初始化 ---- */
static int s_init_step = 0;
static bool s_init_done = false;
static uint32_t s_init_last_ms = 0;

/* ---- PID轮询 ---- */
static int s_pid_idx = 0;
static bool s_pid_sent = false;
static uint32_t s_last_pid_ms = 0;

/* ---- ECU Burst ---- */
static bool s_in_burst = false;
static int s_burst_ecu = 0, s_burst_did = 0, s_burst_phase = 0;
static uint32_t s_burst_cmd_ms = 0, s_last_burst_ms = 0;

/* ================================================================
 *  工具函数
 * ================================================================ */
static int hex_bytes_from_str(const char* hex_str, uint8_t* out, int max_out) {
    int len = strlen(hex_str);
    int count = 0;
    for (int i = 0; i < len - 1 && count < max_out; i += 2) {
        char h = hex_str[i], l = hex_str[i+1];
        if (!isxdigit(h) || !isxdigit(l)) break;
        char buf[3] = {h, l, '\0'};
        out[count++] = (uint8_t)strtol(buf, NULL, 16);
    }
    return count;
}

static void elm_tx(const char* cmd) {
    bluetooth_manager_write(cmd);
    bluetooth_manager_rx_clear();
    s_init_last_ms = millis();
}

static bool elm_w(int t) { 
    return bluetooth_manager_rx_ready() || (int)(millis() - s_init_last_ms) > t; 
}
static bool elm_ok() { 
    const char* rx = bluetooth_manager_rx_buf();
    return rx && strstr(rx, "OK") != nullptr; 
}
static bool elm_h(const char* p) { 
    const char* rx = bluetooth_manager_rx_buf();
    return rx && strstr(rx, p) != nullptr; 
}

static void lrx(const char* tag, int s) {
    if (!bluetooth_manager_rx_ready()) return;
    const char* rx = bluetooth_manager_rx_buf();
    if (!rx || !rx[0]) return;
    char t[256]; strncpy(t, rx, sizeof(t)-1); t[sizeof(t)-1] = 0;
    for (int i = 0; t[i]; i++) if (t[i] == '\r' || t[i] == '\n') t[i] = ' ';
    Serial.printf("%s RX(%02d): [%s]\n", tag, s, t);
}

/* ================================================================
 *  PID解析
 * ================================================================ */
static bool parse_pid_response(const char* rx, int pid_idx) {
    if (!rx || !rx[0]) return false;

    const char* pid_str = s_pid_cmds[pid_idx];
    uint8_t mode = 0x41;
    if (pid_str[0] == '0' && pid_str[1] == '2') mode = 0x42;
    else if (pid_str[0] == '0' && pid_str[1] == '3') mode = 0x43;

    char mode_pid[8];
    snprintf(mode_pid, sizeof(mode_pid), "%02X%c%c", mode, pid_str[2], pid_str[3]);

    char buf[512];
    strncpy(buf, rx, sizeof(buf)-1); buf[sizeof(buf)-1] = '\0';

    char* tok = strtok(buf, " \r\n>");
    while (tok) {
        if (strlen(tok) < 8) { tok = strtok(NULL, " \r\n>"); continue; }

        char* mp = strstr(tok, mode_pid);
        if (!mp) { tok = strtok(NULL, " \r\n>"); continue; }

        uint8_t bytes[8] = {0};
        int bc = hex_bytes_from_str(mp + 4, bytes, 8);
        bool found = false;

        switch (pid_idx) {
            case 0: if (bc >= 2) { s_data.rpm = (((uint16_t)bytes[0] << 8) | bytes[1]) / 4; found = true; } break;
            case 1: if (bc >= 1) { s_data.speed = bytes[0]; found = true; } break;
            case 2: if (bc >= 1) { s_data.coolant = (int)bytes[0] - 40; found = true; } break;
            case 3: if (bc >= 1) { s_data.oil = (int)bytes[0] - 40; found = true; } break;
            case 4: if (bc >= 1) { s_data.epa_soc = bytes[0] * 100.0f / 255.0f; found = true; } break;
            case 5: if (bc >= 2) { s_data.mil_on = (bytes[0] >> 7) & 1; s_data.dtc_count = bytes[0] & 0x7F; found = true; } break;
            case 6: if (bc >= 2) { s_data.batt_v = ((uint16_t)bytes[0] << 8 | bytes[1]) * 0.001f; found = true; } break;
            case 7: if (bc >= 2) { s_data.dist = ((uint16_t)bytes[0] << 8) | bytes[1]; found = true; } break;
            case 8: if (bc >= 1) { s_data.map = bytes[0]; found = true; } break;
            case 9: if (bc >= 1) { s_data.iat = (int)bytes[0] - 40; found = true; } break;
            case 10: if (bc >= 1) { s_data.throttle = (int)(bytes[0] * 100.0f / 255.0f); found = true; } break;
            case 11: if (bc >= 2) { s_data.abs_load = (((uint16_t)bytes[0] << 8) | bytes[1]) * 100.0f / 255.0f; found = true; } break;
            case 12: if (bc >= 1) { s_data.torque_pct = (int)bytes[0] - 125; found = true; } break;
            case 13: if (bc >= 2) { s_data.torque_ref = ((uint16_t)bytes[0] << 8) | bytes[1]; found = true; } break;
            case 14: if (bc >= 4) {
                uint32_t raw = ((uint32_t)bytes[0] << 24) | ((uint32_t)bytes[1] << 16) | ((uint32_t)bytes[2] << 8) | bytes[3];
                s_data.odometer = raw / 10.0f; found = true;
            } break;
        }

        if (found) return true;
        tok = strtok(NULL, " \r\n>");
    }
    return false;
}

/* ================================================================
 *  DID解析器
 * ================================================================ */
static void parse_sgcm_did(uint16_t did, const uint8_t* d, int len) {
    switch(did) {
        case 0x0808: if (len >= 2) { s_data.sgcm_rpm = ((uint16_t)d[0]<<8)|d[1]; } break;
        case 0x0809: if (len >= 2) { s_data.sgcm_current = (((uint16_t)d[0]<<8)|d[1]) * 0.01f; } break;
        case 0x0812: if (len >= 2) { s_data.sgcm_rated = ((uint16_t)d[0]<<8)|d[1]; } break;
        case 0x080A: if (len >= 2) { s_data.sgcm_current_b = (((uint16_t)d[0]<<8)|d[1]) * 0.01f; } break;
    }
}

static void parse_bms_did(uint16_t did, const uint8_t* d, int len) {
    switch(did) {
        case 0xF228: if (len >= 2) { s_data.bms_total_v = (((uint16_t)d[0]<<8)|d[1]) * 0.1f; } break;
        case 0xF250: if (len >= 2) { s_data.bms_max_cell = (((uint16_t)d[0]<<8)|d[1]) * 0.001f; } break;
        case 0xF251: if (len >= 2) { s_data.bms_min_cell = (((uint16_t)d[0]<<8)|d[1]) * 0.001f; } break;
        case 0xF252: if (len >= 1) { s_data.bms_max_temp = (int)d[0] - 40; } break;
        case 0xF253: if (len >= 1) { s_data.bms_min_temp = (int)d[0] - 40; } break;
        case 0xF254: if (len >= 1) { s_data.bms_max_pos = d[0]; } break;
        case 0xF255: if (len >= 1) { s_data.bms_min_pos = d[0]; } break;
        case 0xF229: if (len >= 2) { s_data.bms_remain_wh = ((uint16_t)d[0]<<8)|d[1]; } break;
    }
}

static void parse_hpcm_scan_did(uint16_t did, const uint8_t* d, int len) {
    Serial.printf("[HPCM] DID=0x%04X len=%d raw=0x", did, len);
    for (int i = 0; i < len; i++) Serial.printf("%02X", d[i]);
    Serial.println();
}

static void parse_becm_scan_did(uint16_t did, const uint8_t* d, int len) {
    Serial.printf("[BECM] DID=0x%04X len=%d raw=0x", did, len);
    for (int i = 0; i < len; i++) Serial.printf("%02X", d[i]);
    Serial.println();
}

static void parse_bms_scan_did(uint16_t did, const uint8_t* d, int len) {
    Serial.printf("[BMS-SCAN] DID=0x%04X len=%d raw=0x", did, len);
    for (int i = 0; i < len; i++) Serial.printf("%02X", d[i]);
    Serial.println();
}

/* ================================================================
 *  帧解析 (从CAN响应中提取DID数据)
 * ================================================================ */
static void parse_did_frames(const char* rx, uint16_t expect_did, uint16_t expect_rx, did_parser_t parser) {
    char buf[512]; strncpy(buf, rx, sizeof(buf)-1); buf[sizeof(buf)-1] = '\0';
    char* tok = strtok(buf, " \r\n>");
    while (tok) {
        int len = strlen(tok);
        if (len < 8) { tok = strtok(NULL, " \r\n>"); continue; }
        char idstr[4] = {tok[0], tok[1], tok[2], '\0'};
        uint16_t cid = (uint16_t)strtol(idstr, NULL, 16);
        if (cid != expect_rx) { tok = strtok(NULL, " \r\n>"); continue; }
        const char* pay = tok + 3;
        int pl = strlen(pay);
        if (pl < 4 || pl % 2 != 0) { tok = strtok(NULL, " \r\n>"); continue; }
        uint8_t bytes[8]; int dlc = pl / 2;
        for (int i = 0; i < dlc; i++) { char b[3] = {pay[i*2], pay[i*2+1], '\0'}; bytes[i] = (uint8_t)strtol(b, NULL, 16); }
        if (dlc < 4) { tok = strtok(NULL, " \r\n>"); continue; }
        uint8_t pci = bytes[0];
        int data_len = pci - 3;
        if (data_len <= 0) { tok = strtok(NULL, " \r\n>"); continue; }
        if (bytes[1] == 0x62) {
            uint16_t rdid = ((uint16_t)bytes[2] << 8) | bytes[3];
            if (rdid == expect_did) { parser(rdid, &bytes[4], data_len); return; }
        }
        tok = strtok(NULL, " \r\n>");
    }
}

/* ================================================================
 *  ELM327初始化状态机
 * ================================================================ */
static void elm_init_poll(void) {
    if (s_init_done) return;
    switch (s_init_step) {
        case 0: elm_tx("ATZ\r"); s_init_step = 1; break;
        case 1: if (elm_w(2000)) { lrx("[ELM]", 1); if (elm_h("ELM327") || elm_h(">")) { elm_tx("ATE0\r"); s_init_step = 2; } else { s_init_step = 0; elm_tx("ATZ\r"); } } break;
        case 2: if (elm_w(800)) { lrx("[ELM]", 2); elm_tx("ATH1\r"); s_init_step = 3; } break;
        case 3: if (elm_w(800)) { lrx("[ELM]", 3); elm_tx("ATSP0\r"); s_init_step = 4; } break;
        case 4: if (elm_w(800)) { lrx("[ELM]", 4); elm_tx("ATE0\r"); s_init_step = 5; } break;
        case 5: if (elm_w(800)) { lrx("[ELM]", 5); elm_tx("ATH1\r"); s_init_step = 6; } break;
        case 6: if (elm_w(800)) { lrx("[ELM]", 6); elm_tx("ATM0\r"); s_init_step = 7; } break;
        case 7: if (elm_w(800)) { lrx("[ELM]", 7); elm_tx("ATS0\r"); s_init_step = 8; } break;
        case 8: if (elm_w(800)) { lrx("[ELM]", 8); elm_tx("ATAT1\r"); s_init_step = 9; } break;
        case 9: if (elm_w(800)) { lrx("[ELM]", 9); elm_tx("ATAL\r"); s_init_step = 10; } break;
        case 10: if (elm_w(800)) { lrx("[ELM]", 10); elm_tx("ATST64\r"); s_init_step = 11; } break;
        case 11: if (elm_w(800)) { lrx("[ELM]", 11); elm_tx("0100\r"); s_init_step = 12; } break;
        case 12: if (!elm_w(3000)) return; lrx("[ELM]", 12); if (elm_h("SEARCHING") && !elm_h("7E")) { s_init_last_ms = millis(); bluetooth_manager_rx_clear(); return; } s_init_last_ms = millis(); s_init_step = 13; break;
        case 13: if ((int)(millis() - s_init_last_ms) < 800) return; Serial.println("[ELM] Init complete"); s_init_done = true; s_last_burst_ms = millis(); break;
        default: s_init_done = true; break;
    }
}

/* ================================================================
 *  ECU DID Burst轮询
 * ================================================================ */
static void did_burst_poll(uint32_t now) {
    if (!s_in_burst) return;
    const EcuConfig* ecu = &s_ecu_list[s_burst_ecu];
    switch (s_burst_phase) {
        case 0: {
            if (!s_pid_sent) {
                bluetooth_manager_rx_clear();
                char c[16]; snprintf(c, sizeof(c), "ATSH%03X\r", ecu->tx_addr);
                bluetooth_manager_write(c);
                s_pid_sent = true; s_burst_cmd_ms = now;
            }
            if (bluetooth_manager_rx_ready() || (int)(now - s_burst_cmd_ms) > 800) {
                lrx("[ECU]", s_burst_ecu);
                s_burst_phase = ecu->needs_session ? 10 : 1;
                s_pid_sent = false;
            }
            break;
        }
        case 10: {
            if (!s_pid_sent) {
                bluetooth_manager_rx_clear();
                bluetooth_manager_write("1003\r");
                s_pid_sent = true; s_burst_cmd_ms = now;
            }
            if (bluetooth_manager_rx_ready() || (int)(now - s_burst_cmd_ms) > 1200) {
                lrx("[ECU]", s_burst_ecu);
                s_burst_phase = 1; s_burst_did = 0; s_pid_sent = false;
            }
            break;
        }
        case 1: {
            if (s_burst_did < ecu->did_count) {
                if (!s_pid_sent) {
                    bluetooth_manager_rx_clear();
                    char c[16]; snprintf(c, sizeof(c), "22%04X\r", ecu->dids[s_burst_did]);
                    bluetooth_manager_write(c);
                    s_pid_sent = true; s_burst_cmd_ms = now;
                }
                if (bluetooth_manager_rx_ready() || (int)(now - s_burst_cmd_ms) > 1200) {
                    lrx("[ECU]", s_burst_did);
                    if (bluetooth_manager_rx_ready()) 
                        parse_did_frames(bluetooth_manager_rx_buf(), ecu->dids[s_burst_did], ecu->rx_addr, ecu->parser);
                    s_burst_did++; s_pid_sent = false;
                }
            } else {
                s_burst_ecu++; s_pid_sent = false;
                if (s_burst_ecu < (int)ECU_COUNT) s_burst_phase = 0;
                else s_burst_phase = 2;
            }
            break;
        }
        case 2: {
            if (!s_pid_sent) {
                bluetooth_manager_rx_clear();
                bluetooth_manager_write("ATSH7DF\r");
                s_pid_sent = true; s_burst_cmd_ms = now;
            }
            if (bluetooth_manager_rx_ready() || (int)(now - s_burst_cmd_ms) > 800) {
                lrx("[OBD]", 99);
                s_in_burst = false; s_last_burst_ms = now; s_pid_sent = false; s_burst_ecu = 0;
                Serial.println("[BURST] === end ===");
            }
            break;
        }
    }
}

/* ================================================================
 *  PID轮询
 * ================================================================ */
static void pid_poll(uint32_t now) {
    if (!s_pid_sent) {
        if ((int)(now - s_last_pid_ms) < PID_INTERVAL_MS) return;
        s_last_pid_ms = now;
        bluetooth_manager_rx_clear();
        char cmd[8]; snprintf(cmd, sizeof(cmd), "%s\r", s_pid_cmds[s_pid_idx]);
        bluetooth_manager_write(cmd);
        Serial.printf("[OBD] TX:%s (%s)\n", s_pid_cmds[s_pid_idx], s_pid_names[s_pid_idx]);
        s_pid_sent = true; return;
    }
    if (!bluetooth_manager_rx_ready() && (int)(now - s_last_pid_ms) < RX_TIMEOUT_MS) return;

    if (bluetooth_manager_rx_ready()) {
        const char* rx = bluetooth_manager_rx_buf();
        if (rx && rx[0]) {
            if (parse_pid_response(rx, s_pid_idx)) {
                // PID解析成功
            } else {
                char t[256]; strncpy(t, rx, sizeof(t)-1); t[sizeof(t)-1] = 0;
                for (int i = 0; t[i]; i++) if (t[i] == '\r' || t[i] == '\n') t[i] = ' ';
                if (strlen(t) > 3) Serial.printf("[OBD] unhandled:[%s]\n", t);
            }
        }
    }
    s_pid_sent = false;
    s_pid_idx = (s_pid_idx + 1) % PID_COUNT;
}

/* ================================================================
 *  连接状态回调
 * ================================================================ */
static void on_bt_conn(bool connected) {
    if (connected) {
        obd_manager_start();
    } else {
        obd_manager_stop();
    }
}

/* ================================================================
 *  公共API
 * ================================================================ */
void obd_manager_init(void) {
    s_running = false;
    s_init_done = false; s_init_step = 0;
    s_pid_idx = 0; s_pid_sent = false;
    s_in_burst = false; s_burst_phase = 0;
    memset(&s_data, 0, sizeof(s_data));
    // 注册蓝牙连接回调
    bluetooth_manager_set_conn_cb(on_bt_conn);
}

void obd_manager_start(void) {
    s_running = true;
    s_init_done = false; s_init_step = 0;
    s_pid_idx = 0; s_pid_sent = false;
    s_in_burst = false; s_burst_phase = 0;
    s_last_pid_ms = millis();
    s_last_burst_ms = millis();
    Serial.println("[OBD] Started");
}

void obd_manager_stop(void) {
    s_running = false;
    s_init_done = false;
    Serial.println("[OBD] Stopped");
}

void obd_manager_update(void) {
    if (!s_running || !bluetooth_is_connected()) return;

    uint32_t now = millis();

    // 先完成ELM初始化
    if (!s_init_done) {
        elm_init_poll();
        return;
    }

    // ECU DID扫描优先（每5秒一次）
    if (!s_in_burst && !s_pid_sent && (int)(now - s_last_burst_ms) > BURST_INTERVAL_MS) {
        s_in_burst = true; s_burst_phase = 0; s_burst_ecu = 0; s_burst_did = 0; s_pid_sent = false;
        Serial.println("[BURST] === start ===");
    }

    if (s_in_burst) {
        did_burst_poll(now);
        return;
    }

    // 标准PID轮询
    pid_poll(now);
}

const struct OBDData* obd_manager_get_data(void) {
    return &s_data;
}

/* 调试接口: 发送原始命令并等待响应 */
bool obd_manager_send_raw(const char* cmd, char* out, int out_len, int timeout_ms) {
    if (!bluetooth_is_connected()) return false;
    bluetooth_manager_rx_clear();
    if (!bluetooth_manager_write(cmd)) return false;

    uint32_t start = millis();
    while ((int)(millis() - start) < timeout_ms) {
        if (bluetooth_manager_rx_ready()) {
            const char* rx = bluetooth_manager_rx_buf();
            if (rx && out) {
                strncpy(out, rx, out_len-1);
                out[out_len-1] = '\0';
            }
            return true;
        }
        delay(10);
    }
    return false;
}