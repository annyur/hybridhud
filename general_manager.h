/* general_manager.h */
#ifndef GENERAL_MANAGER_H
#define GENERAL_MANAGER_H

#include <Arduino.h>
#include "src/gui_guider.h"
#include <SensorQMI8658.hpp>   // 添加
#include <SensorPCF85063.hpp>  // 添加

#ifdef __cplusplus
extern "C" {
#endif

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