/* bluetooth_manager.h — BLE连接管理 (基础版)
 * 职责: 设备扫描、连接、断开、UI交互
 * 不处理OBD数据轮询 (由 obd_manager 负责)
 */
#ifndef BLUETOOTH_MANAGER_H
#define BLUETOOTH_MANAGER_H

#include "gui_guider.h"
#include <Arduino.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*app_switch_cb_t)(lv_ui* ui, int screen_id, bool forward);
typedef void (*bt_conn_cb_t)(bool connected);  // 连接状态变化回调

void bluetooth_manager_init(lv_ui* ui);
void bluetooth_manager_set_switch_cb(app_switch_cb_t cb);
void bluetooth_manager_set_conn_cb(bt_conn_cb_t cb);  // 设置连接状态回调 (供obd_manager使用)

void bluetooth_manager_enter(void);
void bluetooth_manager_exit(void);
void bluetooth_manager_update(void);

bool bluetooth_is_connected(void);
const char* bluetooth_connected_name(void);
const char* bluetooth_connected_addr(void);

/* 原始数据发送 (供 obd_manager 调用) */
bool bluetooth_manager_write(const char* data);
/* 检查是否有新数据 */
bool bluetooth_manager_rx_ready(void);
/* 获取接收缓冲区 */
const char* bluetooth_manager_rx_buf(void);
/* 清除接收标志 */
void bluetooth_manager_rx_clear(void);

#ifdef __cplusplus
}
#endif

#endif