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

/* 动态零偏持续校准 */
static int s_stationary_cnt = 0;

/* 缓存旧位置，用于变化检测 */
static int16_t s_last_px = -1;
static int16_t s_last_py = -1;

/* race_label_G_point 移动参数（父容器 = race_cont_G） */
#define METER_CX        233
#define METER_CY        233
#define POINT_HALF      10

/* G-force 映射参数 */
#define OUTER_RADIUS    160.0f  /* 直径320 */
#define INNER_RADIUS    90.0f   /* 直径180 中心夸张区 */
#define G_MAX           2.0f
#define G_THRESHOLD     0.25f   /* 夸张/线性切换阈值 */
#define G_DEADZONE      0.05f   /* 静止死区 */

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
        lv_obj_set_style_text_color(s_ui->race_label_G_point,
                                    lv_color_hex(0xFFFFFF), LV_PART_MAIN);
        lv_obj_set_style_text_opa(s_ui->race_label_G_point,
                                  LV_OPA_COVER, LV_PART_MAIN);
    }
}

/* ================================================================
 *  G-force 计算与 label_G_point 位置更新
 *
 *  Waveshare SensorLib getAccelerometer() 实测轴映射：
 *    QMI8658 在 PCB 上相对丝印有 90° 旋转，库返回的：
 *      ax → 前后方向（车头/惯性前后）
 *      ay → 左右方向
 *
 *  分段映射：
 *    |G| < 0.25  : sqrt 夸张映射到 INNER_RADIUS（直径180中心圆）
 *    |G| >= 0.25 : 线性映射 INNER_RADIUS → OUTER_RADIUS（直径320外圈）
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

    /* ---- 动态零偏校准：静止时缓慢收敛 ---- */
    float raw_gx = ax / 9.80665f;
    float raw_gy = ay / 9.80665f;
    float raw_abs = sqrtf(raw_gx * raw_gx + raw_gy * raw_gy);

    if (raw_abs < 0.05f) {
        s_stationary_cnt++;
        if (s_stationary_cnt > 50) {  /* 约 1s 静止后锁定零偏 */
            s_g_offset_x = s_g_offset_x * 0.95f + ax * 0.05f;
            s_g_offset_y = s_g_offset_y * 0.95f + ay * 0.05f;
        }
    } else {
        s_stationary_cnt = 0;
    }

    /* ---- 应用零偏 ---- */
    float ax_c = ax - s_g_offset_x;
    float ay_c = ay - s_g_offset_y;

    const float G = 9.80665f;

    /* ---- 方向映射（ax=前后, ay=左右，取负转为惯性力方向） ---- */
    float g_front = -ax_c / G;   /* ax = 前后（车头方向），负号=加速度→惯性力 */
    float g_right =  ay_c / G;   /* ay = 左右 */

    /* ---- 统一计算总G值并限制 ---- */
    float g_abs = sqrtf(g_right * g_right + g_front * g_front);
    if (g_abs > G_MAX && g_abs > 0.0f) {
        float s = G_MAX / g_abs;
        g_right *= s;
        g_front *= s;
        g_abs = G_MAX;
    }

    /* ---- 静止死区：|G| < 0.05 强制归零 ---- */
    if (g_abs < G_DEADZONE) {
        g_right = 0.0f;
        g_front = 0.0f;
        g_abs = 0.0f;
    }

    /* ---- 方向归一化 ---- */
    float nx = 0.0f, ny = 0.0f;
    if (g_abs > 0.0f) {
        nx = g_right / g_abs;
        ny = g_front / g_abs;
    }

    /* ---- 归一化距离 0.0 ~ 1.0 ---- */
    float n_dist = g_abs / G_MAX;

    /* ---- 分段映射到像素距离 ---- */
    float pixel_dist;
    const float threshold_norm = G_THRESHOLD / G_MAX;  /* = 0.125 */
    if (n_dist < threshold_norm) {
        /* 夸张区：0~0.25G 映射到 0~90px，sqrt 曲线让小G更敏感 */
        float t = n_dist / threshold_norm;  /* 0.0 ~ 1.0 */
        pixel_dist = INNER_RADIUS * sqrtf(t);
    } else {
        /* 正常区：0.25~2.0G 线性映射 90~160px */
        float t = (n_dist - threshold_norm) / (1.0f - threshold_norm);
        pixel_dist = INNER_RADIUS + (OUTER_RADIUS - INNER_RADIUS) * t;
    }

    /* ---- 像素位移（屏幕Y向下，dy 负号使向上为正） ---- */
    float dx = nx * pixel_dist;
    float dy = -ny * pixel_dist;

    /* ---- 安全圆限制（双保险） ---- */
    float final_dist = sqrtf(dx * dx + dy * dy);
    if (final_dist > OUTER_RADIUS && final_dist > 0.0f) {
        float s = OUTER_RADIUS / final_dist;
        dx *= s;
        dy *= s;
    }

    /* ---- 设置位置 ---- */
    int16_t px = METER_CX + (int16_t)dx - POINT_HALF;
    int16_t py = METER_CY + (int16_t)dy - POINT_HALF;

    /* ---- 变化检测：只有位置变化时才重绘，防花屏+减卡顿 ---- */
    if (px != s_last_px || py != s_last_py) {
        s_last_px = px;
        s_last_py = py;
        lv_obj_set_pos(s_ui->race_label_G_point, px, py);
        lv_obj_invalidate(s_ui->race);
    }

    /* ---- 四个方向 G 值（只在变化时更新 label） ---- */
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
 *  G 值校准（enter 时初始值）
 * ================================================================ */
static void calibrate_g(void)
{
    if (!qmi.getDataReady()) return;
    float ax, ay, az;
    qmi.getAccelerometer(ax, ay, az);
    s_g_offset_x = ax;
    s_g_offset_y = ay;
    s_stationary_cnt = 0;
    s_last_px = -1;
    s_last_py = -1;
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

    /* ---- 禁用 Race 界面所有 arc 和按钮的触摸 ---- */
    if (s_ui->race_arc_1)         lv_obj_clear_flag(s_ui->race_arc_1,         LV_OBJ_FLAG_CLICKABLE);
    if (s_ui->race_arc_2)         lv_obj_clear_flag(s_ui->race_arc_2,         LV_OBJ_FLAG_CLICKABLE);
    if (s_ui->race_arc_3)         lv_obj_clear_flag(s_ui->race_arc_3,         LV_OBJ_FLAG_CLICKABLE);
    if (s_ui->race_arc_4)         lv_obj_clear_flag(s_ui->race_arc_4,         LV_OBJ_FLAG_CLICKABLE);
    if (s_ui->race_btn_G)         lv_obj_clear_flag(s_ui->race_btn_G,         LV_OBJ_FLAG_CLICKABLE);
    if (s_ui->race_label_G_point) lv_obj_clear_flag(s_ui->race_label_G_point, LV_OBJ_FLAG_CLICKABLE);

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