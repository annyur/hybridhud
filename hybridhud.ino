/*
 * hybridhud.ino
 * 4屏手势导航 + 动画 + 防连切 + Setting底部上滑关闭
 */

#include <lvgl.h>
#include <Arduino_GFX_Library.h>
#include <Wire.h>
#include <stdio.h>

#include "src/gui_guider.h"
#include "src/custom.h"
#include "TouchDrvCSTXXX.hpp"
#include <SensorPCF85063.hpp>
#include <SensorQMI8658.hpp>

#define LCD_SDIO0    4
#define LCD_SDIO1    5
#define LCD_SDIO2    6
#define LCD_SDIO3    7
#define LCD_SCLK    38
#define LCD_CS      12
#define LCD_RESET   39
#define LCD_WIDTH  466
#define LCD_HEIGHT 466

#define IIC_SDA     15
#define IIC_SCL     14
#define TP_INT      11
#define TP_RESET    40

Arduino_DataBus *bus = new Arduino_ESP32QSPI(
    LCD_CS, LCD_SCLK, LCD_SDIO0, LCD_SDIO1, LCD_SDIO2, LCD_SDIO3);
Arduino_GFX *gfx = new Arduino_CO5300(
    bus, LCD_RESET, 0, LCD_WIDTH, LCD_HEIGHT, 6, 0, 0, 0);

TouchDrvCST92xx touch;
SensorPCF85063 rtc;
SensorQMI8658 qmi;
lv_ui guider_ui;

int16_t touchX[5], touchY[5];
static lv_disp_draw_buf_t draw_buf;
static lv_color_t *buf1 = NULL;
static lv_color_t *buf2 = NULL;

unsigned long last_time_ms = 0;
unsigned long last_temp_ms = 0;
char time_buf[6];
char temp_buf[16];

/* ==================== 页面枚举 ==================== */
typedef enum {
    SCREEN_MAIN = 0,
    SCREEN_RANCE,
    SCREEN_DETAIL,
    SCREEN_SETTING
} screen_id_t;

static screen_id_t current_screen = SCREEN_MAIN;
static screen_id_t prev_screen    = SCREEN_MAIN;

/* ==================== 手势状态 ==================== */
#define GS_IDLE     0
#define GS_PRESSED  1
static int gs_state = GS_IDLE;
static int16_t gs_sx, gs_sy, gs_ex, gs_ey, gs_px, gs_py;

/* 动画锁定：切换期间忽略新手势 */
static bool is_switching = false;
static unsigned long switch_unlock_ms = 0;
#define SWITCH_LOCK_TIME  400   // 400ms 内禁止再次切换

/* Setting 关闭区域：只有从屏幕底部 40% 上滑才关闭 */
#define SETTING_CLOSE_Y_THRESHOLD  280   // y > 280 视为底部区域

/* ==================== 显示刷新 ==================== */
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);
#if (LV_COLOR_16_SWAP != 0)
    gfx->draw16bitBeRGBBitmap(area->x1, area->y1, (uint16_t *)&color_p->full, w, h);
#else
    gfx->draw16bitRGBBitmap(area->x1, area->y1, (uint16_t *)&color_p->full, w, h);
#endif
    lv_disp_flush_ready(disp);
}

/* ==================== 触摸回调 ==================== */
void my_touchpad_read(lv_indev_drv_t *indev, lv_indev_data_t *data)
{
    uint8_t n = touch.getPoint(touchX, touchY, touch.getSupportTouchPoint());
    if (n > 0) {
        data->point.x = touchX[0];
        data->point.y = touchY[0];
        data->state   = LV_INDEV_STATE_PRESSED;
    } else {
        data->state = LV_INDEV_STATE_RELEASED;
    }
}

/* ==================== 带动画的页面切换 ==================== */
void switch_screen(screen_id_t target_id, lv_scr_load_anim_t anim)
{
    if (is_switching) return;   // 动画期间锁定

    lv_obj_t *target = NULL;
    switch (target_id) {
        case SCREEN_MAIN:    target = guider_ui.main;    break;
        case SCREEN_RANCE:   target = guider_ui.rance;   break;
        case SCREEN_DETAIL:  target = guider_ui.detail;  break;
        case SCREEN_SETTING: target = guider_ui.setting; break;
    }
    if (target == NULL || target == lv_scr_act()) return;

    /* 执行动画切换 */
    lv_scr_load_anim(target, anim, 30, 0, false);
    current_screen = target_id;

    /* 锁定 400ms，防止连续切换 */
    is_switching = true;
    switch_unlock_ms = millis() + SWITCH_LOCK_TIME;
}

/* ==================== 手势处理 ==================== */
void process_gesture(int16_t dx, int16_t dy)
{
    /* 先检查解锁 */
    if (is_switching && millis() >= switch_unlock_ms) {
        is_switching = false;
    }
    if (is_switching) return;

    const int16_t THRESH = 30;

    /* 无效滑动 */
    if (abs(dx) < THRESH && abs(dy) < THRESH) return;

    if (abs(dx) > abs(dy)) {
        /* ========== 水平滑动 ========== */
        if (dx < 0) {   // 左滑 → 新页面从右边进入
            if (current_screen == SCREEN_MAIN) {
                switch_screen(SCREEN_RANCE, LV_SCR_LOAD_ANIM_MOVE_LEFT);
            } else if (current_screen == SCREEN_DETAIL) {
                switch_screen(SCREEN_MAIN, LV_SCR_LOAD_ANIM_MOVE_LEFT);
            }
        } else {        // 右滑 → 新页面从左边进入
            if (current_screen == SCREEN_MAIN) {
                switch_screen(SCREEN_DETAIL, LV_SCR_LOAD_ANIM_MOVE_RIGHT);
            } else if (current_screen == SCREEN_RANCE) {
                switch_screen(SCREEN_MAIN, LV_SCR_LOAD_ANIM_MOVE_RIGHT);
            }
        }
    } else {
        /* ========== 垂直滑动 ========== */
        if (dy > 0) {   // 下滑 → Setting 从底部升起
            if (current_screen != SCREEN_SETTING) {
                prev_screen = current_screen;
                switch_screen(SCREEN_SETTING, LV_SCR_LOAD_ANIM_MOVE_BOTTOM);
            }
        } else {        // 上滑
            if (current_screen == SCREEN_SETTING) {
                /* 关键：只有从屏幕底部区域上滑才关闭 Setting */
                if (gs_sy > SETTING_CLOSE_Y_THRESHOLD) {
                    switch_screen(prev_screen, LV_SCR_LOAD_ANIM_MOVE_TOP);
                }
                /* 否则忽略此次上滑（不执行任何操作） */
            } else if (current_screen != SCREEN_MAIN) {
                switch_screen(SCREEN_MAIN, LV_SCR_LOAD_ANIM_MOVE_TOP);
            }
        }
    }
}

/* ==================== 数据更新 ==================== */
void update_detail_time()
{
    if (guider_ui.detail_label_time == NULL) return;
    struct tm timeinfo;
    rtc.getDateTime(&timeinfo);
    snprintf(time_buf, sizeof(time_buf), "%02d:%02d",
             timeinfo.tm_hour, timeinfo.tm_min);
    lv_label_set_text(guider_ui.detail_label_time, time_buf);
}

void update_detail_temp()
{
    if (guider_ui.detail_label_temp == NULL) return;
    float temp = 0.0;
    bool ok = false;
    if (qmi.getDataReady()) {
        temp = qmi.getTemperature_C();
        ok = true;
    }
    if (ok) {
        snprintf(temp_buf, sizeof(temp_buf), "%.1fC", temp);
    } else {
        snprintf(temp_buf, sizeof(temp_buf), "--.-C");
    }
    lv_label_set_text(guider_ui.detail_label_temp, temp_buf);
}

void update_detail_slider()
{
    if (guider_ui.detail_slider_energy == NULL) return;
    int16_t v = lv_slider_get_value(guider_ui.detail_slider_energy);
    float t = (float)(v + 50) / 100.0f;
    if (t < 0) t = 0; if (t > 1) t = 1;
    lv_obj_set_style_bg_color(guider_ui.detail_slider_energy,
        lv_color_make((uint8_t)(t*255), (uint8_t)((1-t)*255), 0), LV_PART_INDICATOR);
}

static void slider_event_cb(lv_event_t *e) { update_detail_slider(); }

/* ==================== Setup ==================== */
void setup()
{
    Serial.begin(115200);
    delay(1500);

    pinMode(TP_RESET, OUTPUT);
    digitalWrite(TP_RESET, LOW); delay(30);
    digitalWrite(TP_RESET, HIGH); delay(50);

    Wire.begin(IIC_SDA, IIC_SCL);
    touch.setPins(TP_RESET, TP_INT);
    touch.begin(Wire, 0x5A, IIC_SDA, IIC_SCL);
    touch.setMaxCoordinates(LCD_WIDTH, LCD_HEIGHT);
    touch.setMirrorXY(true, true);

    rtc.begin(Wire, IIC_SDA, IIC_SCL);
    // rtc.setDateTime(2026, 4, 25, 17, 0, 0);

    if (!qmi.begin(Wire, 0x6B, IIC_SDA, IIC_SCL)) {
    } else {
        qmi.configAccelerometer(SensorQMI8658::ACC_RANGE_4G,
                                SensorQMI8658::ACC_ODR_1000Hz,
                                SensorQMI8658::LPF_MODE_0);
        qmi.enableAccelerometer();
    }

    gfx->begin();
    lv_init();

    uint32_t buf_size = (LCD_WIDTH * LCD_HEIGHT) / 4;
    buf1 = (lv_color_t *)heap_caps_malloc(buf_size * sizeof(lv_color_t), MALLOC_CAP_DMA);
    buf2 = (lv_color_t *)heap_caps_malloc(buf_size * sizeof(lv_color_t), MALLOC_CAP_DMA);
    lv_disp_draw_buf_init(&draw_buf, buf1, buf2, buf_size);

    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = LCD_WIDTH;
    disp_drv.ver_res = LCD_HEIGHT;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);

    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = my_touchpad_read;
    lv_indev_drv_register(&indev_drv);

    /* 创建所有屏幕 */
    setup_ui(&guider_ui);
    setup_scr_rance(&guider_ui);
    setup_scr_detail(&guider_ui);
    setup_scr_setting(&guider_ui);

    current_screen = SCREEN_MAIN;

    /* detail Slider 事件 */
    if (guider_ui.detail_slider_energy != NULL) {
        update_detail_slider();
        lv_obj_add_event_cb(guider_ui.detail_slider_energy,
                            slider_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
    }
}

/* ==================== Loop ==================== */
void loop()
{
    /* 检查动画锁是否到期 */
    if (is_switching && millis() >= switch_unlock_ms) {
        is_switching = false;
    }

    /* 手势检测 */
    uint8_t n = touch.getPoint(touchX, touchY, touch.getSupportTouchPoint());

    if (gs_state == GS_IDLE && n > 0) {
        gs_sx = gs_px = touchX[0];
        gs_sy = gs_py = touchY[0];
        gs_state = GS_PRESSED;
    }
    else if (gs_state == GS_PRESSED) {
        if (n > 0) {
            gs_px = touchX[0];
            gs_py = touchY[0];
        } else {
            gs_ex = gs_px; gs_ey = gs_py;
            int16_t dx = gs_ex - gs_sx;
            int16_t dy = gs_ey - gs_sy;
            process_gesture(dx, dy);
            gs_state = GS_IDLE;
        }
    }

    /* 数据更新（只在 detail 页） */
    unsigned long now = millis();
    if (current_screen == SCREEN_DETAIL) {
        if (now - last_time_ms >= 1000) { last_time_ms = now; update_detail_time(); }
        if (now - last_temp_ms >= 3000) { last_temp_ms = now; update_detail_temp(); }
    }

    lv_tick_inc(5);
    lv_timer_handler();
    delay(5);
}