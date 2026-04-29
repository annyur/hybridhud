/* bluetooth_manager.h — BLE连接管理 (基础版)
 * 职责: 设备扫描、连接、断开、UI交互
 * 不处理OBD数据轮询 (由 obd_manager 负责)
 */
#ifndef BLUETOOTH_MANAGER_H
#define BLUETOOTH_MANAGER_H

#include <Arduino.h>

#ifdef __cplusplus
extern "C" {
#endif

/* 前置声明 lv_ui，避免包含 gui_guider.h */
struct _lv_ui;
typedef struct _lv_ui lv_ui;

typedef void (*app_switch_cb_t)(lv_ui* ui, int screen_id, bool forward);
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