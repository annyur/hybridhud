/* obd_manager.h — OBD 数据轮询与缓存模块 */
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
    /* ---- 标准 PID 数据 ---- */
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

/* ================================================================
 *  ECU DID 接口 (由 ecu.cpp 实现)
 * ================================================================ */
typedef void (*did_parser_t)(uint16_t did, const uint8_t* d, int len, struct OBDData* out);

typedef struct {
    const char* label;
    uint16_t tx_addr;
    uint16_t rx_addr;
    const uint16_t* dids;
    int did_count;
    did_parser_t parser;
    bool needs_session;
} EcuConfig;

extern const EcuConfig s_ecu_list[];
extern const int ECU_COUNT;

/* 车型解析器 */
void parse_bms_did(uint16_t did, const uint8_t* d, int len, struct OBDData* out);
void parse_sgcm_did(uint16_t did, const uint8_t* d, int len, struct OBDData* out);
void parse_hpcm_scan_did(uint16_t did, const uint8_t* d, int len, struct OBDData* out);
void parse_becm_scan_did(uint16_t did, const uint8_t* d, int len, struct OBDData* out);
void parse_bms_scan_did(uint16_t did, const uint8_t* d, int len, struct OBDData* out);

/* ================================================================
 *  OBD 管理器 API
 * ================================================================ */
void obd_manager_init(void);
void obd_manager_update(void);
void obd_manager_start(void);
void obd_manager_stop(void);

const struct OBDData* obd_manager_get_data(void);

/* 调试接口: 发送原始 AT 命令, 返回响应 */
bool obd_manager_send_raw(const char* cmd, char* out, int out_len, int timeout_ms);

#ifdef __cplusplus
}
#endif

#endif