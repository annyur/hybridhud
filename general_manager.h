#ifndef GENERAL_MANAGER_H
#define GENERAL_MANAGER_H

#include "src/gui_guider.h"

#ifdef __cplusplus
extern "C" {
#endif

void general_manager_init(lv_ui *ui);
void general_manager_enter(void);
void general_manager_exit(void);
bool general_manager_is_active(void);
void general_manager_update(uint32_t now_ms);

#ifdef __cplusplus
}
#endif

#endif
