/*
 * hybridhud.ino - 终极极简测试版
 * 完全不依赖 gui_guider.h，只显示一个红色 "OK"
 */

#include <lvgl.h>
#include <Arduino_GFX_Library.h>
#include <Wire.h>

/* 引脚 */
#define LCD_SDIO0  4
#define LCD_SDIO1  5
#define LCD_SDIO2  6
#define LCD_SDIO3  7
#define LCD_SCLK  38
#define LCD_CS    12
#define LCD_RESET 39
#define LCD_W    466
#define LCD_H    466

Arduino_DataBus *bus = new Arduino_ESP32QSPI(LCD_CS, LCD_SCLK, LCD_SDIO0, LCD_SDIO1, LCD_SDIO2, LCD_SDIO3);
Arduino_GFX *gfx = new Arduino_CO5300(bus, LCD_RESET, 0, LCD_W, LCD_H, 6, 0, 0, 0);

static lv_disp_draw_buf_t draw_buf;
static lv_color_t *buf1 = NULL;
static lv_color_t *buf2 = NULL;

void my_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
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

void setup()
{
    Serial.begin(115200);
    delay(1500);

    gfx->begin();

    lv_init();

    buf1 = (lv_color_t *)heap_caps_malloc(LCD_W * 40 * sizeof(lv_color_t), MALLOC_CAP_DMA);
    buf2 = (lv_color_t *)heap_caps_malloc(LCD_W * 40 * sizeof(lv_color_t), MALLOC_CAP_DMA);
    lv_disp_draw_buf_init(&draw_buf, buf1, buf2, LCD_W * 40);

    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = LCD_W;
    disp_drv.ver_res = LCD_H;
    disp_drv.flush_cb = my_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);

    /* 创建一个红色背景的屏幕 */
    lv_obj_t *scr = lv_obj_create(NULL);
    lv_obj_set_size(scr, LCD_W, LCD_H);
    lv_obj_set_style_bg_color(scr, lv_color_hex(0xFF0000), 0);  // 红色背景
    lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, 0);

    /* 创建一个白色 Label */
    lv_obj_t *label = lv_label_create(scr);
    lv_label_set_text(label, "OK");
    lv_obj_set_style_text_color(label, lv_color_hex(0xFFFFFF), 0);
    lv_obj_center(label);

    lv_scr_load(scr);

    /* 强制刷新 */
    for (int i = 0; i < 20; i++) {
        lv_tick_inc(5);
        lv_timer_handler();
        delay(5);
    }

    Serial.println("Done");
}

void loop()
{
    lv_tick_inc(5);
    lv_timer_handler();
    delay(5);
}