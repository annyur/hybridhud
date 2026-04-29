/* race_manager.cpp — Race 界面 G-Force + 蓝牙闪烁 */
#include "race_manager.h"
#include <Arduino.h>
#include <math.h>
#include <SensorQMI8658.hpp>
#include "bluetooth_manager.h"

extern SensorQMI8658 qmi;
extern bool bluetooth_is_connected(void);

static lv_ui *s_ui = NULL;
static bool   s_active = false;
static uint32_t s_last_g_ms = 0;

/* G 值校准零偏 */
static float s_g_offset_x = 0.0f;
static float s_g_offset_y = 0.0f;

/* race_label_G_point 移动参数（父容器 = race_cont_G） */
#define METER_CX        233
#define METER_CY        233
#define METER_RADIUS    145
#define POINT_SIZE      20
#define POINT_HALF      10

/* ================================================================
 *  蓝牙未连接闪烁效果 (label_G_point)
 * ================================================================ */
static void update_point_blink(uint32_t now_ms)
{
    if (!s_ui || !s_ui->race_label_G_point) return;

    if (!bluetooth_is_connected()) {
        uint32_t phase = now_ms % 800;
        lv_opa_t opa = (phase < 400)
            ? (lv_opa_t)(60 + 195 * phase / 400)
            : (lv_opa_t)(255 - 195 * (phase - 400) / 400);

        lv_obj_set_style_text_color(s_ui->race_label_G_point,
                                    lv_color_hex(0xFF0000), LV_PART_MAIN);
        lv_obj_set_style_text_opa(s_ui->race_label_G_point, opa, LV_PART_MAIN);
    } else {
        /* 已连接：恢复白色不透明 */
        lv_obj_set_style_text_color(s_ui->race_label_G_point,
                                    lv_color_hex(0xFFFFFF), LV_PART_MAIN);
        lv_obj_set_style_text_opa(s_ui->race_label_G_point,
                                  LV_OPA_COVER, LV_PART_MAIN);
    }
}

/* ================================================================
 *  G-force 计算与 label_G_point 位置更新
 * ================================================================ */
static void update_g_force(uint32_t now_ms)
{
    if (!s_ui || !s_ui->race_label_G_point) return;

    /* 50Hz */
    if (now_ms - s_last_g_ms < 20) return;
    s_last_g_ms = now_ms;

    if (!qmi.getDataReady()) return;

    float ax, ay, az;
    qmi.getAccelerometer(ax, ay, az);

    float ax_c = ax - s_g_offset_x;
    float ay_c = ay - s_g_offset_y;

    const float G = 9.80665f;
    float g_right =  ax_c / G;
    float g_front = -ay_c / G;

    g_right = constrain(g_right, -2.0f, 2.0f);
    g_front = constrain(g_front, -2.0f, 2.0f);

    float dx = (g_right / 2.0f) * METER_RADIUS;
    float dy = -(g_front / 2.0f) * METER_RADIUS;

    float dist = sqrtf(dx * dx + dy * dy);
    if (dist > METER_RADIUS && dist > 0.0f) {
        float s = METER_RADIUS / dist;
        dx *= s;
        dy *= s;
    }

    int16_t px = METER_CX + (int16_t)dx - POINT_HALF;
    int16_t py = METER_CY + (int16_t)dy - POINT_HALF;

    lv_obj_set_pos(s_ui->race_label_G_point, px, py);
    lv_obj_invalidate(s_ui->race);

    /* 四个方向 G 值 */
    char buf[8];
    static char s_buf_top[8]   = "";
    static char s_buf_down[8]  = "";
    static char s_buf_right[8] = "";
    static char s_buf_left[8]  = "";

    snprintf(buf, sizeof(buf), "%.2f", g_front < 0 ? -g_front : 0.0f);
    if (strcmp(buf, s_buf_top) != 0) {
        strcpy(s_buf_top, buf);
        if (s_ui->race_label_G_top) lv_label_set_text(s_ui->race_label_G_top, buf);
    }

    snprintf(buf, sizeof(buf), "%.2f", g_front > 0 ?  g_front : 0.0f);
    if (strcmp(buf, s_buf_down) != 0) {
        strcpy(s_buf_down, buf);
        if (s_ui->race_label_G_down) lv_label_set_text(s_ui->race_label_G_down, buf);
    }

    snprintf(buf, sizeof(buf), "%.2f", g_right > 0 ?  g_right : 0.0f);
    if (strcmp(buf, s_buf_right) != 0) {
        strcpy(s_buf_right, buf);
        if (s_ui->race_label_G_right) lv_label_set_text(s_ui->race_label_G_right, buf);
    }

    snprintf(buf, sizeof(buf), "%.2f", g_right < 0 ? -g_right : 0.0f);
    if (strcmp(buf, s_buf_left) != 0) {
        strcpy(s_buf_left, buf);
        if (s_ui->race_label_left) lv_label_set_text(s_ui->race_label_left, buf);
    }
}

/* ================================================================
 *  G 值校准
 * ================================================================ */
static void calibrate_g(void)
{
    if (!qmi.getDataReady()) return;
    float ax, ay, az;
    qmi.getAccelerometer(ax, ay, az);
    s_g_offset_x = ax;
    s_g_offset_y = ay;
}

/* ================================================================
 *  外部接口
 * ================================================================ */
void race_manager_init(lv_ui *ui)
{
    s_ui = ui;
    calibrate_g();
}

void race_manager_enter(void)
{
    s_active = true;
    calibrate_g();

    if (s_ui->race_img_3) {
        lv_obj_set_style_clip_corner(s_ui->race_img_3, false, LV_PART_MAIN);
    }

    if (s_ui->race_label_G_point) {
        lv_obj_move_foreground(s_ui->race_label_G_point);
    }
}

void race_manager_exit(void)
{
    s_active = false;
}

bool race_manager_is_active(void)
{
    return s_active;
}

void race_manager_update(uint32_t now_ms)
{
    if (!s_active || !s_ui) return;
    update_g_force(now_ms);
    update_point_blink(now_ms);
}