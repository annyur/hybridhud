/* general_manager.cpp — general界面数据绑定
 * 数据来源: obd_manager_get_data() (OBD实时数据)
 */
#include "general_manager.h"
#include "obd_manager.h"
#include "bluetooth_manager.h"
#include "src/gui_guider.h"
#include <Arduino.h>
#include <SensorQMI8658.hpp>
#include <SensorPCF85063.hpp>

/* 测试模式：1=模拟数据，0=真实OBD */
#define GENERAL_TEST_MODE 0

extern SensorQMI8658 qmi;
extern SensorPCF85063 rtc;
extern bool bluetooth_is_connected(void);

static lv_ui *s_ui = NULL;
static bool s_active = false;

/* 计时 */
static uint32_t s_last_time_ms = 0;
static uint32_t s_last_temp_ms = 0;
static uint32_t s_last_arc_ms = 0;
static uint32_t s_last_avg_ms = 0;
static uint32_t s_last_odometer_ms = 0;
static uint32_t s_last_blink_ms = 0;

/* 能量数据 */
static float s_slider_power = 0.0f;
static float s_arc_power = 0.0f;

/* 平均功率 / 平均油耗累加器 */
static float s_power_sum = 0.0f;
static long s_power_count = 0;
static long s_fuel_sum = 0;
static long s_fuel_count = 0;
static float s_avg_power = 0.0f;
static float s_avg_fuel = 0.0f;

/* 车辆总里程 / 行程里程 */
static float s_odometer = 0.0f;
static float s_trip_distance = 0.0f;

/* ================================================================
 *  蓝牙未连接闪烁效果 (slider_energy)
 * ================================================================ */
static void update_slider_blink(uint32_t now_ms)
{
    if (!s_ui || !s_ui->general_slider_energy) return;

    if (!bluetooth_is_connected()) {
        /* 三角波渐隐闪烁，周期 800ms，透明度 60~255 */
        uint32_t phase = now_ms % 800;
        lv_opa_t opa = (phase < 400)
            ? (lv_opa_t)(60 + 195 * phase / 400)
            : (lv_opa_t)(255 - 195 * (phase - 400) / 400);

        lv_obj_set_style_bg_color(s_ui->general_slider_energy,
                                  lv_color_hex(0xFF0000), LV_PART_INDICATOR);
        lv_obj_set_style_bg_opa(s_ui->general_slider_energy, opa, LV_PART_INDICATOR);
    } else {
        /* 已连接：恢复不透明，颜色由 update_slider_display 接管 */
        lv_obj_set_style_bg_opa(s_ui->general_slider_energy,
                                LV_OPA_COVER, LV_PART_INDICATOR);
    }
}

/* ================================================================
 *  能量显示更新
 * ================================================================ */
static void update_slider_display(void)
{
    if (!s_ui) return;
    char buf[16];
    snprintf(buf, sizeof(buf), "%.1f", s_slider_power);
    if (s_ui->general_label_energy_number) {
        lv_label_set_text(s_ui->general_label_energy_number, buf);
    }
    if (s_ui->general_slider_energy) {
        int center = 50;
        int mapped = (int)(s_slider_power + center);
        if (s_slider_power >= 0) {
            lv_bar_set_start_value(s_ui->general_slider_energy, center, LV_ANIM_OFF);
            lv_bar_set_value(s_ui->general_slider_energy, mapped, LV_ANIM_OFF);
        } else {
            lv_bar_set_start_value(s_ui->general_slider_energy, mapped, LV_ANIM_OFF);
            lv_bar_set_value(s_ui->general_slider_energy, center, LV_ANIM_OFF);
        }
        /* 未连接蓝牙时不覆盖颜色（由 update_slider_blink 控制红色闪烁） */
        if (bluetooth_is_connected()) {
            lv_color_t color;
            if (s_slider_power < 0) color = lv_color_hex(0x00FF00);
            else if (s_slider_power < 50.0f) color = lv_color_hex(0xFF6500);
            else color = lv_color_hex(0xFF0000);
            lv_obj_set_style_bg_color(s_ui->general_slider_energy, color, LV_PART_INDICATOR);
        }
    }
    s_power_sum += s_slider_power;
    s_power_count++;
}

static void update_arc_display(void)
{
    if (!s_ui) return;
    char buf[16];
    snprintf(buf, sizeof(buf), "%.1f", s_arc_power);
    if (s_ui->general_label_energy_number_2) {
        lv_label_set_text(s_ui->general_label_energy_number_2, buf);
    }
    if (s_ui->general_arc_energy) {
        lv_arc_set_value(s_ui->general_arc_energy, (int)s_arc_power);
    }
}

/* ================================================================
 *  平均功率 / 油耗 / 里程更新
 * ================================================================ */
static void update_avg_labels(void)
{
    if (!s_ui) return;
    char buf[16];
    if (s_power_count > 0) s_avg_power = s_power_sum / (float)s_power_count;
    snprintf(buf, sizeof(buf), "%.1f", s_avg_power);
    if (s_ui->general_label_2) lv_label_set_text(s_ui->general_label_2, buf);

    if (s_fuel_count > 0) s_avg_fuel = (float)s_fuel_sum / (float)s_fuel_count / 10.0f;
    snprintf(buf, sizeof(buf), "%.1f", s_avg_fuel);
    if (s_ui->general_label_3) lv_label_set_text(s_ui->general_label_3, buf);
}

static void update_odometer(void)
{
    if (!s_ui) return;
    uint32_t now_ms = millis();
    const struct OBDData* d = obd_manager_get_data();
    if (s_last_odometer_ms > 0 && d->speed > 0) {
        float dt = (now_ms - s_last_odometer_ms) / 1000.0f / 3600.0f;
        if (dt > 0.0f && dt < 1.0f) s_trip_distance += d->speed * dt;
    }
    s_last_odometer_ms = now_ms;

    float display_km = s_odometer + s_trip_distance;
    if (s_ui->general_label_1) {
        char buf[16];
        snprintf(buf, sizeof(buf), "%.1f", display_km);
        lv_label_set_text(s_ui->general_label_1, buf);
    }
}

/* ================================================================
 *  时间 / 温度更新
 * ================================================================ */
static void update_time(void)
{
    if (!s_ui || !s_ui->general_label_time) return;
    struct tm timeinfo;
    rtc.getDateTime(&timeinfo);
    char buf[6];
    snprintf(buf, sizeof(buf), "%02d:%02d", timeinfo.tm_hour, timeinfo.tm_min);
    lv_label_set_text(s_ui->general_label_time, buf);
}

static void update_temp(void)
{
    if (!s_ui || !s_ui->general_label_temp) return;
    float temp = 0.0f;
    bool ok = qmi.getDataReady() && (temp = qmi.getTemperature_C(), true);
    char buf[8];
    snprintf(buf, sizeof(buf), ok ? "%.1f" : "--.-", temp);
    lv_label_set_text(s_ui->general_label_temp, buf);
}

/* ================================================================
 *  OBD 数据驱动 Arc / Label（RPM / Speed / Oil）
 *  30fps，严格变化检测：值没变时不重绘，避免无意义刷新
 * ================================================================ */
static int s_last_rpm = -1;
static int s_last_speed = -1;
static int s_last_oil = -9999;

static void update_obd_display(uint32_t now_ms)
{
    if (!s_ui) return;
    if (now_ms - s_last_arc_ms < 33) return;  /* 30fps */
    s_last_arc_ms = now_ms;

    const struct OBDData* d = obd_manager_get_data();

#if GENERAL_TEST_MODE
    static int test_rpm = 0;
    static int test_speed = 0;
    test_rpm = (test_rpm + 80) % 8000;
    test_speed = (test_speed + 2) % 240;
    int rpm = test_rpm;
    int speed = test_speed;
    int oil = 75 + (int)(15.0f * sinf((float)now_ms / 2000.0f));
#else
    int rpm = d->rpm;
    int speed = d->speed;
    int oil = d->oil;
    if (oil < -40) oil = d->coolant;
#endif

    bool changed = false;

    if (rpm != s_last_rpm) {
        s_last_rpm = rpm;
        changed = true;
        if (s_ui->general_arc_rpm)     lv_arc_set_value(s_ui->general_arc_rpm, rpm);
        if (s_ui->general_label_rpm_number) {
            if (rpm == 0) lv_label_set_text(s_ui->general_label_rpm_number, "");
            else { char buf[8]; snprintf(buf, sizeof(buf), "%d", rpm); lv_label_set_text(s_ui->general_label_rpm_number, buf); }
        }
    }

    if (speed != s_last_speed) {
        s_last_speed = speed;
        changed = true;
        if (s_ui->general_arc_speed)   lv_arc_set_value(s_ui->general_arc_speed, speed);
        if (s_ui->general_label_speed_number) {
            char buf[8]; snprintf(buf, sizeof(buf), "%d", speed);
            lv_label_set_text(s_ui->general_label_speed_number, buf);
        }
    }

    if (oil != s_last_oil) {
        s_last_oil = oil;
        changed = true;
        if (s_ui->general_arc_oil)     lv_arc_set_value(s_ui->general_arc_oil, oil);
        if (s_ui->general_label_oil_number) {
            char buf[8];
            if (oil >= -40) snprintf(buf, sizeof(buf), "%d", oil);
            else snprintf(buf, sizeof(buf), "--");
            lv_label_set_text(s_ui->general_label_oil_number, buf);
        }
    }

    /* 只有数据变化时才整屏刷新 — 防止花屏+减少卡顿 */
    if (changed) {
        lv_obj_invalidate(s_ui->general);
    }
}

/* ================================================================
 *  外部接口（保留兼容）
 * ================================================================ */
void general_manager_set_energy(float power)
{
    s_slider_power = constrain(power, -50.0f, 50.0f);
    s_arc_power = s_slider_power;
    update_slider_display();
    update_arc_display();
}

void general_manager_set_odometer(float km)
{
    s_odometer = km;
    update_odometer();
}

void general_manager_set_fuel(int fuel_x10)
{
    s_fuel_sum += fuel_x10;
    s_fuel_count++;
}

void general_manager_set_obd_data(int rpm, int speed)
{
    (void)rpm; (void)speed;
}

void general_manager_set_oil(int oil_temp)
{
    (void)oil_temp;
}

/* ================================================================
 *  生命周期
 * ================================================================ */
void general_manager_init(lv_ui *ui)
{
    s_ui = ui;
    s_last_rpm = -1; s_last_speed = -1; s_last_oil = -9999;
    if (s_ui->general_slider_energy) {
        lv_slider_set_range(s_ui->general_slider_energy, 0, 100);
    }
    update_slider_display();
    update_arc_display();
    update_avg_labels();
    update_odometer();
}

void general_manager_enter(void)
{
    s_active = true;
    s_last_rpm = -1; s_last_speed = -1; s_last_oil = -9999;
    update_time();
    update_temp();
    update_slider_display();
    update_arc_display();
    update_avg_labels();
    update_odometer();

    /* ---- 禁用 arc / slider 手动触摸滑动 ---- */
    if (s_ui->general_arc_rpm)        lv_obj_clear_flag(s_ui->general_arc_rpm,        LV_OBJ_FLAG_CLICKABLE);
    if (s_ui->general_arc_speed)      lv_obj_clear_flag(s_ui->general_arc_speed,      LV_OBJ_FLAG_CLICKABLE);
    if (s_ui->general_arc_oil)        lv_obj_clear_flag(s_ui->general_arc_oil,        LV_OBJ_FLAG_CLICKABLE);
    if (s_ui->general_arc_energy)     lv_obj_clear_flag(s_ui->general_arc_energy,     LV_OBJ_FLAG_CLICKABLE);
    if (s_ui->general_slider_energy)  lv_obj_clear_flag(s_ui->general_slider_energy,  LV_OBJ_FLAG_CLICKABLE);

    /* 数字标签移到最上层 */
    if (s_ui->general_label_rpm_number)      lv_obj_move_foreground(s_ui->general_label_rpm_number);
    if (s_ui->general_label_speed_number)    lv_obj_move_foreground(s_ui->general_label_speed_number);
    if (s_ui->general_label_oil_number)      lv_obj_move_foreground(s_ui->general_label_oil_number);
    if (s_ui->general_label_energy_number)   lv_obj_move_foreground(s_ui->general_label_energy_number);
    if (s_ui->general_label_energy_number_2) lv_obj_move_foreground(s_ui->general_label_energy_number_2);
}

void general_manager_exit(void)
{
    s_active = false;
}

bool general_manager_is_active(void)
{
    return s_active;
}

void general_manager_update(uint32_t now_ms)
{
    if (!s_active || !s_ui) return;

    /* 时间: 1秒 */
    if (now_ms - s_last_time_ms >= 1000) {
        s_last_time_ms = now_ms;
        update_time();
    }

    /* 温度: 3秒 */
    if (now_ms - s_last_temp_ms >= 3000) {
        s_last_temp_ms = now_ms;
        update_temp();
    }

    /* OBD 数据驱动 RPM / Speed / Oil（30fps，变化检测） */
    update_obd_display(now_ms);

    /* 平均功率 / 油耗 / 里程: 1秒 */
    if (now_ms - s_last_avg_ms >= 1000) {
        s_last_avg_ms = now_ms;
        update_avg_labels();
        update_odometer();
    }

    /* 蓝牙未连接闪烁 */
    update_slider_blink(now_ms);

#if GENERAL_TEST_MODE
    static int test_energy = 0;
    static int test_energy_dir = 1;
    if ((now_ms / 50) % 2 == 0) {
        test_energy += test_energy_dir * 2;
        if (test_energy >= 45) test_energy_dir = -1;
        if (test_energy <= -45) test_energy_dir = 1;
        s_slider_power = constrain(test_energy, -50, 50);
        s_arc_power = s_slider_power;
        update_slider_display();
        update_arc_display();
    }
#endif
}