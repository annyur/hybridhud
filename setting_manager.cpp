#include "setting_manager.h"
#include <Arduino.h>

static app_switch_cb_t s_switch_cb = NULL;
static uint32_t last_btn_ms = 0;
static const uint32_t BTN_DEBOUNCE_MS = 200;

// ---- Button click handlers ----
static void on_general(lv_event_t *e)
{
    (void)e;
    if (millis() - last_btn_ms < BTN_DEBOUNCE_MS) return;
    last_btn_ms = millis();
    Serial.println("[BTN] General");
    if (s_switch_cb) s_switch_cb(APP_SCREEN_GENERAL, false);
}

static void on_race(lv_event_t *e)
{
    (void)e;
    if (millis() - last_btn_ms < BTN_DEBOUNCE_MS) return;
    last_btn_ms = millis();
    Serial.println("[BTN] Race");
    if (s_switch_cb) s_switch_cb(APP_SCREEN_RACE, false);
}

static void on_bluetooth(lv_event_t *e)
{
    (void)e;
    if (millis() - last_btn_ms < BTN_DEBOUNCE_MS) return;
    last_btn_ms = millis();
    Serial.println("[BTN] Bluetooth");
    if (s_switch_cb) s_switch_cb(APP_SCREEN_BLUETOOTH, false);
}

// ---- Helpers ----
static void disable_parent_scroll(lv_obj_t *obj)
{
    lv_obj_t *parent = lv_obj_get_parent(obj);
    while (parent && parent != lv_scr_act()) {
        lv_obj_clear_flag(parent, LV_OBJ_FLAG_SCROLLABLE |
                                  LV_OBJ_FLAG_SCROLL_CHAIN |
                                  LV_OBJ_FLAG_CLICK_FOCUSABLE);
        parent = lv_obj_get_parent(parent);
    }
}

static void register_button(lv_obj_t *btn, lv_event_cb_t cb)
{
    if (!btn) return;
    disable_parent_scroll(btn);
    lv_obj_add_flag(btn, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_clear_flag(btn, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_event_cb(btn, cb, LV_EVENT_CLICKED, NULL);
}

static void clear_label_clickable(lv_obj_t *label)
{
    if (label) lv_obj_clear_flag(label, LV_OBJ_FLAG_CLICKABLE);
}

// ---- Public API ----
void setting_manager_init(lv_ui *ui)
{
    // Register screen-switch buttons
    register_button(ui->setting_btn_General,   on_general);
    register_button(ui->setting_btn_Race,      on_race);
    register_button(ui->setting_btn_Bluetooth, on_bluetooth);

    // Disable scrolling on setting screen itself
    if (ui->setting) {
        lv_obj_clear_flag(ui->setting, LV_OBJ_FLAG_SCROLLABLE |
                                       LV_OBJ_FLAG_SCROLL_CHAIN |
                                       LV_OBJ_FLAG_CLICK_FOCUSABLE);
    }

    // Prevent child labels from intercepting clicks
    clear_label_clickable(ui->setting_btn_General_label);
    clear_label_clickable(ui->setting_btn_Race_label);
    clear_label_clickable(ui->setting_btn_Bluetooth_label);
}

void setting_manager_set_switch_cb(app_switch_cb_t cb)
{
    s_switch_cb = cb;
}