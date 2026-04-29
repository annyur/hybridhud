/* ecu.cpp — ECU DID 配置与解析器
 * 存放位置: 项目根目录 (与 obd_manager.cpp 同级)
 */
#include "obd_manager.h"

/* ================================================================
 *  DID 列表
 * ================================================================ */
static const uint16_t s_bms_dids[] = {
    0xF250, 0xF251, 0xF252, 0xF253, 0xF254, 0xF255, 0xF229, 0xF228
};

static const uint16_t s_sgcm_dids[] = {
    0x0808, 0x0809, 0x0812, 0x080A
};

static const uint16_t s_hpcm_scan_dids[] = {
    0xF100, 0xF101, 0xF102, 0xF103
};

static const uint16_t s_becm_scan_dids[] = {
    0xF220, 0xF221, 0xF222, 0xF223
};

static const uint16_t s_bms_scan_dids[] = {
    0xF210, 0xF211, 0xF212, 0xF213,
    0xF214, 0xF215, 0xF216, 0xF217
};

/* ================================================================
 *  DID 解析器实现
 * ================================================================ */
void parse_sgcm_did(uint16_t did, const uint8_t* d, int len, struct OBDData* out)
{
    switch (did) {
        case 0x0808: if (len >= 2) { out->sgcm_rpm = ((uint16_t)d[0] << 8) | d[1]; } break;
        case 0x0809: if (len >= 2) { out->sgcm_current = (((uint16_t)d[0] << 8) | d[1]) * 0.01f; } break;
        case 0x0812: if (len >= 2) { out->sgcm_rated = ((uint16_t)d[0] << 8) | d[1]; } break;
        case 0x080A: if (len >= 2) { out->sgcm_current_b = (((uint16_t)d[0] << 8) | d[1]) * 0.01f; } break;
    }
}

void parse_bms_did(uint16_t did, const uint8_t* d, int len, struct OBDData* out)
{
    switch (did) {
        case 0xF228: if (len >= 2) { out->bms_total_v = (((uint16_t)d[0] << 8) | d[1]) * 0.1f; } break;
        case 0xF250: if (len >= 2) { out->bms_max_cell = (((uint16_t)d[0] << 8) | d[1]) * 0.001f; } break;
        case 0xF251: if (len >= 2) { out->bms_min_cell = (((uint16_t)d[0] << 8) | d[1]) * 0.001f; } break;
        case 0xF252: if (len >= 1) { out->bms_max_temp = (int)d[0] - 40; } break;
        case 0xF253: if (len >= 1) { out->bms_min_temp = (int)d[0] - 40; } break;
        case 0xF254: if (len >= 1) { out->bms_max_pos = d[0]; } break;
        case 0xF255: if (len >= 1) { out->bms_min_pos = d[0]; } break;
        case 0xF229: if (len >= 2) { out->bms_remain_wh = ((uint16_t)d[0] << 8) | d[1]; } break;
    }
}

void parse_hpcm_scan_did(uint16_t did, const uint8_t* d, int len, struct OBDData* out)
{
    (void)out;  /* 扫描模式不写入数据，仅打印 */
    Serial.printf("[HPCM] DID=0x%04X len=%d raw=0x", did, len);
    for (int i = 0; i < len; i++) Serial.printf("%02X", d[i]);
    Serial.println();
}

void parse_becm_scan_did(uint16_t did, const uint8_t* d, int len, struct OBDData* out)
{
    (void)out;
    Serial.printf("[BECM] DID=0x%04X len=%d raw=0x", did, len);
    for (int i = 0; i < len; i++) Serial.printf("%02X", d[i]);
    Serial.println();
}

void parse_bms_scan_did(uint16_t did, const uint8_t* d, int len, struct OBDData* out)
{
    (void)out;
    Serial.printf("[BMS-SCAN] DID=0x%04X len=%d raw=0x", did, len);
    for (int i = 0; i < len; i++) Serial.printf("%02X", d[i]);
    Serial.println();
}

/* ================================================================
 *  ECU 配置表
 * ================================================================ */
const EcuConfig s_ecu_list[] = {
    {"BMS",      0x7A1, 0x7A9, s_bms_dids,       8, parse_bms_did,       false},
    {"SGCM",     0x7E7, 0x7EF, s_sgcm_dids,      4, parse_sgcm_did,      false},
    {"HPCM",     0x7E0, 0x7E8, s_hpcm_scan_dids, 4, parse_hpcm_scan_did, false},
    {"BECM",     0x7E4, 0x7EC, s_becm_scan_dids, 4, parse_becm_scan_did, false},
    {"BMS_SCAN", 0x7A1, 0x7A9, s_bms_scan_dids,  8, parse_bms_scan_did,  false},
};

const int ECU_COUNT = sizeof(s_ecu_list) / sizeof(s_ecu_list[0]);