/*
 * hybridhud.ino - 过渡修复版
 * 用极简方式初始化 LVGL，手动调用 setup_scr_main，绕过 setup_ui
 */

#include <lvgl.h>
#include <Arduino_GFX_Library.h>
#include <Wire.h>
#include <stdio.h>

#include "gui_guider.h"
#include "custom.h"
#include "TouchDrvCSTXXX.hpp"
#include <SensorPCF85063.hpp>
#include <SensorQMI8658.hpp>

/* ==================== 引脚定义 ==================== */
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

/* ==================== 全局对象 ==================== */
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

lv_obj_t *time_label = NULL;
lv_obj_t *temp_label = NULL;
unsigned long last_time_ms = 0;
unsigned long last_temp_ms = 0;
char time_buf[6];
char temp_buf[16];

/* ==================== 显示刷新回调 ==================== */
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
    uint8_t touched = touch.getPoint(touchX, touchY, touch.getSupportTouchPoint());
    if (touched > 0) {
        data->point.x = touchX[0];
        data->point.y = touchY[0];
        data->state = LV_INDEV_STATE_PRESSED;
    } else {
        data->state = LV_INDEV_STATE_RELEASED;
    }
}

/* ==================== 时间更新 ==================== */
void update_digital_clock(lv_obj_t *label)
{
    if (label == NULL) return;
    struct tm timeinfo;
    rtc.getDateTime(&timeinfo);
    snprintf(time_buf, sizeof(time_buf), "%02d:%02d",
             timeinfo.tm_hour, timeinfo.tm_min);
    lv_label_set_text(label, time_buf);
}

/* ==================== 温度更新 ==================== */
void update_temperature(lv_obj_t *label)
{
    if (label == NULL) return;
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
    lv_label_set_text(label, temp_buf);
}

/* ==================== Slider 颜色渐变 ==================== */
void update_slider_energy_color(lv_obj_t *slider)
{
    if (slider == NULL) return;
    int16_t value = lv_slider_get_value(slider);
    float t = (float)(value + 50) / 100.0f;
    if (t < 0.0f) t = 0.0f;
    if (t > 1.0f) t = 1.0f;
    uint8_t r = (uint8_t)(t * 255);
    uint8_t g = (uint8_t)((1.0f - t) * 255);
    uint8_t b = 0;
    lv_color_t color = lv_color_make(r, g, b);
    lv_obj_set_style_bg_color(slider, color, LV_PART_INDICATOR);
}

static void slider_energy_event_cb(lv_event_t * e)
{
    lv_obj_t * slider = lv_event_get_target(e);
    update_slider_energy_color(slider);
}

/* ==================== Setup ==================== */
void setup()
{
    Serial.begin(115200);
    delay(1500);

    /* 触摸 */
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

    /* RTC */
    rtc.begin(Wire, IIC_SDA, IIC_SCL);
    // rtc.setDateTime(2026, 4, 25, 17, 0, 0);

    /* QMI8658 */
    if (!qmi.begin(Wire, 0x6B, IIC_SDA, IIC_SCL)) {
    } else {
        qmi.configAccelerometer(SensorQMI8658::ACC_RANGE_4G,
                                SensorQMI8658::ACC_ODR_1000Hz,
                                SensorQMI8658::LPF_MODE_0);
        qmi.enableAccelerometer();
    }

    /* 显示 - 极简方式初始化（已验证有效） */
    gfx->begin();

    lv_init();

    /* 用较小的缓冲区（极简版验证过的参数） */
    buf1 = (lv_color_t *)heap_caps_malloc(LCD_WIDTH * 40 * sizeof(lv_color_t), MALLOC_CAP_DMA);
    buf2 = (lv_color_t *)heap_caps_malloc(LCD_WIDTH * 40 * sizeof(lv_color_t), MALLOC_CAP_DMA);
    lv_disp_draw_buf_init(&draw_buf, buf1, buf2, LCD_WIDTH * 40);

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

    /* ===== 关键：直接调用 setup_scr_main，绕过 setup_ui ===== */
    Serial.println("[1] About to setup_scr_main...");
    setup_scr_main(&guider_ui);
    Serial.println("[2] setup_scr_main DONE");

    /* 手动加载屏幕（无动画） */
    lv_scr_load(guider_ui.main);
    Serial.println("[3] Screen loaded");

    /* 绑定控件 */
    time_label = guider_ui.main_label_time;
    if (time_label != NULL) {
        update_digital_clock(time_label);
    }

    temp_label = guider_ui.main_label_temp;
    if (temp_label != NULL) {
        update_temperature(temp_label);
    }

    if (guider_ui.main_slider_energy != NULL) {
        update_slider_energy_color(guider_ui.main_slider_energy);
        lv_obj_add_event_cb(guider_ui.main_slider_energy, slider_energy_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
    }

    /* 首次刷新 */
    for (int i = 0; i < 20; i++) {
        lv_tick_inc(5);
        lv_timer_handler();
        delay(5);
    }

    Serial.println("[OK] Done");
}

/* ==================== Loop ==================== */
void loop()
{
    lv_tick_inc(5);

    unsigned long now = millis();
    if (now - last_time_ms >= 1000) {
        last_time_ms = now;
        update_digital_clock(time_label);
    }
    if (now - last_temp_ms >= 3000) {
        last_temp_ms = now;
        update_temperature(temp_label);
    }

    lv_timer_handler();
    delay(5);
}