/* obd_manager.h — OBD数据轮询与缓存模块 */
#ifndef OBD_MANAGER_H
#define OBD_MANAGER_H

#include <Arduino.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ================================================================
 *  数据缓存结构 (所有界面共享)
 * ================================================================ */
struct OBDData {
    /* ---- 标准PID数据 ---- */
    int rpm;
    int speed;
    int coolant;
    int oil;
    float epa_soc;
    int mil_on;
    int dtc_count;
    float batt_v;
    int dist;
    int map;
    int iat;
    int throttle;
    float abs_load;
    int torque_pct;
    int torque_ref;
    float odometer;

    /* ---- SGCM (7E7) ---- */
    int sgcm_rpm;
    float sgcm_current;
    int sgcm_rated;
    float sgcm_current_b;

    /* ---- BMS (7A1) ---- */
    float bms_total_v;
    float bms_max_cell;
    float bms_min_cell;
    int bms_max_temp;
    int bms_min_temp;
    int bms_max_pos;
    int bms_min_pos;
    int bms_remain_wh;

    /* ---- 计算值 ---- */
    float power_kw;
};

/* ================================================================ */
void obd_manager_init(void);
void obd_manager_update(void);
void obd_manager_start(void);
void obd_manager_stop(void);

const struct OBDData* obd_manager_get_data(void);

/* 调试接口: 发送原始AT命令, 返回响应 */
bool obd_manager_send_raw(const char* cmd, char* out, int out_len, int timeout_ms);

#ifdef __cplusplus
}
#endif

#endif