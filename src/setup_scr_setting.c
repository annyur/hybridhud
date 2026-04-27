/*
* Copyright 2026 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#include "lvgl.h"
#include <stdio.h>
#include "gui_guider.h"
#include "events_init.h"
#include "widgets_init.h"
#include "custom.h"



void setup_scr_setting(lv_ui *ui)
{
    //Write codes setting
    ui->setting = lv_obj_create(NULL);
    lv_obj_set_size(ui->setting, 466, 466);
    lv_obj_set_scrollbar_mode(ui->setting, LV_SCROLLBAR_MODE_OFF);

    //Write style for setting, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->setting, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->setting, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->setting, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes setting_label_1
    ui->setting_label_1 = lv_label_create(ui->setting);
    lv_label_set_text(ui->setting_label_1, "setting");
    lv_label_set_long_mode(ui->setting_label_1, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->setting_label_1, 133, 39);
    lv_obj_set_size(ui->setting_label_1, 200, 38);

    //Write style for setting_label_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->setting_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->setting_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->setting_label_1, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->setting_label_1, &lv_font_montserratMedium_36, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->setting_label_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->setting_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->setting_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->setting_label_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->setting_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->setting_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->setting_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->setting_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->setting_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->setting_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes setting_btn_Bluetooth
    ui->setting_btn_Bluetooth = lv_btn_create(ui->setting);
    ui->setting_btn_Bluetooth_label = lv_label_create(ui->setting_btn_Bluetooth);
    lv_label_set_text(ui->setting_btn_Bluetooth_label, "Bluetooth");
    lv_label_set_long_mode(ui->setting_btn_Bluetooth_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->setting_btn_Bluetooth_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->setting_btn_Bluetooth, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->setting_btn_Bluetooth_label, LV_PCT(100));
    lv_obj_set_pos(ui->setting_btn_Bluetooth, 158, 348);
    lv_obj_set_size(ui->setting_btn_Bluetooth, 150, 80);

    //Write style for setting_btn_Bluetooth, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->setting_btn_Bluetooth, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->setting_btn_Bluetooth, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->setting_btn_Bluetooth, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->setting_btn_Bluetooth, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->setting_btn_Bluetooth, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->setting_btn_Bluetooth, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->setting_btn_Bluetooth, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->setting_btn_Bluetooth, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->setting_btn_Bluetooth, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->setting_btn_Bluetooth, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes setting_btn_Race
    ui->setting_btn_Race = lv_btn_create(ui->setting);
    ui->setting_btn_Race_label = lv_label_create(ui->setting_btn_Race);
    lv_label_set_text(ui->setting_btn_Race_label, "Race");
    lv_label_set_long_mode(ui->setting_btn_Race_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->setting_btn_Race_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->setting_btn_Race, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->setting_btn_Race_label, LV_PCT(100));
    lv_obj_set_pos(ui->setting_btn_Race, 249, 105);
    lv_obj_set_size(ui->setting_btn_Race, 150, 80);

    //Write style for setting_btn_Race, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->setting_btn_Race, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->setting_btn_Race, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->setting_btn_Race, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->setting_btn_Race, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->setting_btn_Race, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->setting_btn_Race, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->setting_btn_Race, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->setting_btn_Race, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->setting_btn_Race, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->setting_btn_Race, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes setting_btn_General
    ui->setting_btn_General = lv_btn_create(ui->setting);
    ui->setting_btn_General_label = lv_label_create(ui->setting_btn_General);
    lv_label_set_text(ui->setting_btn_General_label, "General");
    lv_label_set_long_mode(ui->setting_btn_General_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->setting_btn_General_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->setting_btn_General, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->setting_btn_General_label, LV_PCT(100));
    lv_obj_set_pos(ui->setting_btn_General, 76, 105);
    lv_obj_set_size(ui->setting_btn_General, 150, 80);

    //Write style for setting_btn_General, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->setting_btn_General, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->setting_btn_General, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->setting_btn_General, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->setting_btn_General, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->setting_btn_General, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->setting_btn_General, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->setting_btn_General, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->setting_btn_General, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->setting_btn_General, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->setting_btn_General, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //The custom code of setting.


    //Update current screen layout.
    lv_obj_update_layout(ui->setting);

}
