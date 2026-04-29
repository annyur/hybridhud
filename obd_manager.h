/* obd_manager.h — OBD数据轮询与缓存模块
 * 职责: PID循环查询 + ECU DID扫描 + 数据解析 + 缓存池
 * general/race 界面通过 get_data() 只读访问缓存
 */
#ifndef OBD_MANAGER_H
#define OBD_MANAGER_H

#include <Arduino.h>

/* ================================================================
 *  数据缓存结构 (所有界面共享)
 * ================================================================ */
struct OBDData {
    /* ---- 标准PID数据 ---- */
    int rpm = 0;              // 010C
    int speed = 0;            // 010D (km/h)
    int coolant = -40;        // 0105 (°C)
    int oil = -40;          // 013C (°C)
    float epa_soc = 0;      // 015B (%)
    int mil_on = 0;         // 0101 bit7
    int dtc_count = 0;      // 0101 low7
    float batt_v = 0;       // 0142 (V)
    int dist = 0;           // 0131 (km, since cleared)
    int map = 0;            // 010B (kPa)
    int iat = -40;          // 010F (°C)
    int throttle = 0;       // 0111 (%)
    float abs_load = 0;     // 0143 (%)
    int torque_pct = 0;     // 0162 (%)
    int torque_ref = 0;     // 0163 (Nm)
    float odometer = 0;     // 01A6 (km)

    /* ---- SGCM (7E7) ---- */
    int sgcm_rpm = 0;       // DID 0808
    float sgcm_current = 0; // DID 0809 (A)
    int sgcm_rated = 0;     // DID 0812
    float sgcm_current_b = 0; // DID 080A (A)

    /* ---- BMS (7A1) ---- */
    float bms_total_v = 0;  // F228 (V)
    float bms_max_cell = 0; // F250 (V)
    float bms_min_cell = 0; // F251 (V)
    int bms_max_temp = 0;   // F252 (°C)
    int bms_min_temp = 0;   // F253 (°C)
    int bms_max_pos = 0;    // F254
    int bms_min_pos = 0;    // F255
    int bms_remain_wh = 0;  // F229 (Wh)

    /* ---- 计算值 ---- */
    float power_kw = 0;     // 估算电机功率 (V*I/1000)
};

/* ================================================================ */
void obd_manager_init(void);
void obd_manager_update(void);      // 每帧调用 (放loop()里)
void obd_manager_start(void);       // BLE连接成功后调用
void obd_manager_stop(void);        // BLE断开时调用

const OBDData& obd_manager_get_data(void);

/* 调试/扩展: 发送原始AT命令, 返回响应 */
bool obd_manager_send_raw(const char* cmd, char* out, int out_len, int timeout_ms);

#endif
