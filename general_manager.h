/* general_manager.h */
#ifndef GENERAL_MANAGER_H
#define GENERAL_MANAGER_H

#include <Arduino.h>

#ifdef __cplusplus
extern "C" {
#endif

/* 前置声明 lv_ui，避免包含 gui_guider.h */
struct _lv_ui;
typedef struct _lv_ui lv_ui;

void general_manager_init(lv_ui *ui);
void general_manager_enter(void);
void general_manager_exit(void);
bool general_manager_is_active(void);
void general_manager_update(uint32_t now_ms);

/* 外部数据接口（保留兼容，但数据优先从obd_manager读取） */
void general_manager_set_obd_data(int rpm, int speed);
void general_manager_set_oil(int oil_temp);
void general_manager_set_energy(float power);
void general_manager_set_odometer(float km);
void general_manager_set_fuel(int fuel_x10);

#ifdef __cplusplus
}
#endif

#endif