#include "general_manager.h"
#include <Arduino.h>
#include <SensorQMI8658.hpp>
#include <SensorPCF85063.hpp>

/* 外部全局硬件 */
extern SensorQMI8658 qmi;
extern SensorPCF85063 rtc;

static lv_ui *s_ui = NULL;
static bool   s_active = false;

/* 计时 */
static uint32_t s_last_time_ms = 0;
static uint32_t s_last_temp_ms = 0;
static uint32_t s_last_arc_ms  = 0;

/* ================================================================
 *  滑块与能量 Arc / Label 绑定
 * ================================================================ */
/* 滑块与 Arc/Label 绑定 — 仅初始化时设置一次，后续由真实数据驱动 */
static void init_energy_display(void)
{
    if (!s_ui || !s_ui->general_slider_energy) return;

    int16_t v = lv_slider_get_value(s_ui->general_slider_energy);

    /* 初始化滑块渐变色 */
    float t = constrain((float)(v + 50) / 100.0f, 0.0f, 1.0f);
    lv_color_t color = lv_color_make((uint8_t)(t * 255), (uint8_t)((1.0f - t) * 255), 0);
    lv_obj_set_style_bg_color(s_ui->general_slider_energy, color, LV_PART_INDICATOR);

    /* 同步能量 Arc */
    if (s_ui->general_arc_energy) {
        lv_arc_set_value(s_ui->general_arc_energy, v);
    }

    /* 同步能量数值标签 */
    char buf[8];
    snprintf(buf, sizeof(buf), "%d", v);
    if (s_ui->general_label_energy_number) {
        lv_label_set_text(s_ui->general_label_energy_number, buf);
    }
    if (s_ui->general_label_energy_number_2) {
        lv_label_set_text(s_ui->general_label_energy_number_2, buf);
    }
}

/* 滑块事件 — 仅更新颜色，不动 Arc/Label（后续由真实数据驱动） */
static void slider_event_cb(lv_event_t *e)
{
    (void)e;
    if (!s_ui || !s_ui->general_slider_energy) return;

    int16_t v = lv_slider_get_value(s_ui->general_slider_energy);
    float t = constrain((float)(v + 50) / 100.0f, 0.0f, 1.0f);
    lv_color_t color = lv_color_make((uint8_t)(t * 255), (uint8_t)((1.0f - t) * 255), 0);
    lv_obj_set_style_bg_color(s_ui->general_slider_energy, color, LV_PART_INDICATOR);
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
 *  模拟数据驱动 Arc（RPM / Speed / Oil）
 * ================================================================ */
static void update_arcs(uint32_t now_ms)
{
    if (!s_ui) return;

    /* 每 100ms 更新一次，制造平滑动画 */
    if (now_ms - s_last_arc_ms < 100) return;
    s_last_arc_ms = now_ms;

    static uint16_t rpm   = 0;
    static uint16_t speed = 0;
    static uint16_t oil   = 75;

    /* RPM: 0~8000 */
    rpm = (rpm + 80) % 8000;
    if (s_ui->general_arc_rpm) {
        lv_arc_set_value(s_ui->general_arc_rpm, rpm);
    }
    if (s_ui->general_label_rpm_number) {
        char buf[8];
        snprintf(buf, sizeof(buf), "%d", rpm);
        lv_label_set_text(s_ui->general_label_rpm_number, buf);
    }

    /* Speed: 0~240 */
    speed = (speed + 2) % 240;
    if (s_ui->general_arc_speed) {
        lv_arc_set_value(s_ui->general_arc_speed, speed);
    }
    if (s_ui->general_label_speed_number) {
        char buf[8];
        snprintf(buf, sizeof(buf), "%d", speed);
        lv_label_set_text(s_ui->general_label_speed_number, buf);
    }

    /* Oil: 50~100 轻微波动 */
    oil = 75 + (int)(15.0f * sinf((float)now_ms / 2000.0f));
    if (s_ui->general_arc_oil) {
        lv_arc_set_value(s_ui->general_arc_oil, oil);
    }
    if (s_ui->general_label_oil_number) {
        char buf[8];
        snprintf(buf, sizeof(buf), "%d", oil);
        lv_label_set_text(s_ui->general_label_oil_number, buf);
    }
}

/* ================================================================
 *  外部接口
 * ================================================================ */
void general_manager_init(lv_ui *ui)
{
    s_ui = ui;

    /* 滑块事件绑定 */
    if (ui->general_slider_energy) {
        init_energy_display();   /* 初始化颜色、arc、label */
        lv_obj_add_event_cb(ui->general_slider_energy, slider_event_cb,
                            LV_EVENT_VALUE_CHANGED, NULL);
    }
}

void general_manager_enter(void)
{
    s_active = true;
    /* 进入时立即刷新一次 */
    update_time();
    update_temp();
    init_energy_display();
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

    /* Arc 暂不做动态更新，后续绑定真实数据
    update_arcs(now_ms);
    */
}