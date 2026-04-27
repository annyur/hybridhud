#ifndef BLUETOOTH_MANAGER_H
#define BLUETOOTH_MANAGER_H

#include "lvgl.h"
#include "src/gui_guider.h"
#include "setting_manager.h"

#ifdef __cplusplus
extern "C" {
#endif

void bluetooth_manager_init(lv_ui *ui);
void bluetooth_manager_set_switch_cb(app_switch_cb_t cb);
void bluetooth_manager_enter(void);
void bluetooth_manager_exit(void);
void bluetooth_manager_update(void);  /* 在 loop 中调用，刷新扫描结果 */

#ifdef __cplusplus
}
#endif

#endif
