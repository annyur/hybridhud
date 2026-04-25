/*
 * hybridhud.ino
 * 4屏手势导航 + 串口调试
 * main | 左滑→rance | 右滑→detail | 下滑→setting | 上滑→返回
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

/* 调试开关：验证手势时打开，正常后注释 */
#define DEBUG_GESTURE 1

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

/* ==================== 页面枚举与状态 ==================== */
typedef enum {
    SCREEN_MAIN = 0,
    SCREEN_RANCE,
    SCREEN_DETAIL,
    SCREEN_SETTING
} screen_id_t;

static screen_id_t current_screen = SCREEN_MAIN;
static screen_id_t prev_screen    = SCREEN_MAIN;

/* ==================== 独立手势状态机 ==================== */
#define GS_IDLE     0
#define GS_PRESSED  1

static int  gs_state = GS_IDLE;
static int16_t gs_sx, gs_sy, gs_ex, gs_ey;
static int16_t gs_prev_x, gs_prev_y;   // 用于实时跟踪
static const int16_t GESTURE_THRESH = 30;  // 阈值30px，小屏更灵敏

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

/* ==================== 触摸回调（给 LVGL 用） ==================== */
void my_touchpad_read(lv_indev_drv_t *indev, lv_indev_data_t *data)
{
    uint8_t touched = touch.getPoint(touchX, touchY, touch.getSupportTouchPoint());
    if (touched > 0) {
        data->point.x = touchX[0];
        data->point.y = touchY[0];
        data->state   = LV_INDEV_STATE_PRESSED;
    } else {
        data->state = LV_INDEV_STATE_RELEASED;
    }
}

/* ==================== 手势处理 ==================== */
void do_page_switch(int16_t dx, int16_t dy)
{
    lv_obj_t *target = NULL;
    lv_scr_load_anim_t anim = LV_SCR_LOAD_ANIM_NONE;

    if (abs(dx) > abs(dy)) {
        /* 水平滑动 */
        if (dx < 0) { // 左滑
            if (current_screen == SCREEN_MAIN) {
                target = guider_ui.rance;  anim = LV_SCR_LOAD_ANIM_MOVE_LEFT;
            } else if (current_screen == SCREEN_DETAIL) {
                target = guider_ui.main;   anim = LV_SCR_LOAD_ANIM_MOVE_LEFT;
            }
        } else { // 右滑
            if (current_screen == SCREEN_MAIN) {
                target = guider_ui.detail; anim = LV_SCR_LOAD_ANIM_MOVE_RIGHT;
            } else if (current_screen == SCREEN_RANCE) {
                target = guider_ui.main;   anim = LV_SCR_LOAD_ANIM_MOVE_RIGHT;
            }
        }
    } else {
        /* 垂直滑动 */
        if (dy > 0) { // 下滑 → setting
            if (current_screen != SCREEN_SETTING) {
                prev_screen = current_screen;
                target = guider_ui.setting; anim = LV_SCR_LOAD_ANIM_MOVE_BOTTOM;
            }
        } else { // 上滑 → 返回
            if (current_screen == SCREEN_SETTING) {
                switch (prev_screen) {
                    case SCREEN_MAIN:   target = guider_ui.main;   break;
                    case SCREEN_RANCE:  target = guider_ui.rance;  break;
                    case SCREEN_DETAIL: target = guider_ui.detail; break;
                    default:            target = guider_ui.main;   break;
                }
                anim = LV_SCR_LOAD_ANIM_MOVE_TOP;
            } else if (current_screen != SCREEN_MAIN) {
                target = guider_ui.main; anim = LV_SCR_LOAD_ANIM_MOVE_TOP;
            }
        }
    }

    if (target && target != lv_scr_act()) {
        lv_scr_load_anim(target, anim, 300, 0, false);
        if (target == guider_ui.rance)        current_screen = SCREEN_RANCE;
        else if (target == guider_ui.detail)  current_screen = SCREEN_DETAIL;
        else if (target == guider_ui.setting) current_screen = SCREEN_SETTING;
        else if (target == guider_ui.main)    current_screen = SCREEN_MAIN;

#ifdef DEBUG_GESTURE
        Serial.print("[SWITCH] dx="); Serial.print(dx);
        Serial.print(" dy="); Serial.print(dy);
        Serial.print(" -> screen="); Serial.println(current_screen);
#endif
    }
}

/* ==================== 时间 / 温度 / Slider ==================== */
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
    int16_t value = lv_slider_get_value(guider_ui.detail_slider_energy);
    float t = (float)(value + 50) / 100.0f;
    if (t < 0.0f) t = 0.0f;
    if (t > 1.0f) t = 1.0f;
    uint8_t r = (uint8_t)(t * 255);
    uint8_t g = (uint8_t)((1.0f - t) * 255);
    uint8_t b = 0;
    lv_obj_set_style_bg_color(guider_ui.detail_slider_energy,
                              lv_color_make(r, g, b), LV_PART_INDICATOR);
}

static void slider_event_cb(lv_event_t *e)
{
    update_detail_slider();
}

/* ==================== Setup ==================== */
void setup()
{
    Serial.begin(115200);
    delay(1500);

    pinMode(TP_RESET, OUTPUT);
    digitalWrite(TP_RESET, LOW);
    delay(30);
    digitalWrite(TP_RESET, HIGH);
    delay(50);

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

    /* GUI Guider 创建所有屏幕 */
    setup_ui(&guider_ui);
    current_screen = SCREEN_MAIN;

    /* detail 页面 Slider 事件（如不需要可注释） */
    if (guider_ui.detail_slider_energy != NULL) {
        update_detail_slider();
        lv_obj_add_event_cb(guider_ui.detail_slider_energy, slider_event_cb,
                            LV_EVENT_VALUE_CHANGED, NULL);
    }
}

/* ==================== Loop ==================== */
void loop()
{
    /* ===== 独立手势检测（不依赖 LVGL indev 回调时序） ===== */
    uint8_t n = touch.getPoint(touchX, touchY, touch.getSupportTouchPoint());

    if (gs_state == GS_IDLE && n > 0) {
        /* 手指按下：记录起点 */
        gs_sx = touchX[0]; gs_sy = touchY[0];
        gs_prev_x = touchX[0]; gs_prev_y = touchY[0];
        gs_state = GS_PRESSED;
    }
    else if (gs_state == GS_PRESSED) {
        if (n > 0) {
            /* 手指移动中：实时更新终点 */
            gs_prev_x = touchX[0]; gs_prev_y = touchY[0];
        } else {
            /* 手指释放：计算总位移并触发 */
            gs_ex = gs_prev_x; gs_ey = gs_prev_y;
            int16_t dx = gs_ex - gs_sx;
            int16_t dy = gs_ey - gs_sy;

#ifdef DEBUG_GESTURE
            Serial.print("[GESTURE] dx="); Serial.print(dx);
            Serial.print(" dy="); Serial.println(dy);
#endif
            if (abs(dx) > GESTURE_THRESH || abs(dy) > GESTURE_THRESH) {
                do_page_switch(dx, dy);
            }
            gs_state = GS_IDLE;
        }
    }

    /* ===== 数据更新（只在 detail 页刷新） ===== */
    unsigned long now = millis();
    if (current_screen == SCREEN_DETAIL) {
        if (now - last_time_ms >= 1000) {
            last_time_ms = now;
            update_detail_time();
        }
        if (now - last_temp_ms >= 3000) {
            last_temp_ms = now;
            update_detail_temp();
        }
    }

    /* ===== LVGL 心跳 ===== */
    lv_tick_inc(5);
    lv_timer_handler();
    delay(5);
}