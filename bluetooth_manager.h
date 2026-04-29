/* bluetooth_manager.h — BLE连接管理 */
#ifndef BLUETOOTH_MANAGER_H
#define BLUETOOTH_MANAGER_H

#include <Arduino.h>
#include "src/gui_guider.h"
#include "setting_manager.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*bt_conn_cb_t)(bool connected);

void bluetooth_manager_init(lv_ui* ui);
void bluetooth_manager_set_switch_cb(app_switch_cb_t cb);
void bluetooth_manager_set_conn_cb(bt_conn_cb_t cb);

void bluetooth_manager_enter(void);
void bluetooth_manager_exit(void);
void bluetooth_manager_update(void);

bool bluetooth_is_connected(void);
const char* bluetooth_connected_name(void);
const char* bluetooth_connected_addr(void);

/* 原始数据接口 (供 obd_manager 调用) */
bool bluetooth_manager_write(const char* data);
bool bluetooth_manager_rx_ready(void);
const char* bluetooth_manager_rx_buf(void);
void bluetooth_manager_rx_clear(void);

#ifdef __cplusplus
}
#endif

#endif