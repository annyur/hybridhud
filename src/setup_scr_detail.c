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



void setup_scr_detail(lv_ui *ui)
{
    //Write codes detail
    ui->detail = lv_obj_create(NULL);
    lv_obj_set_size(ui->detail, 466, 466);
    lv_obj_set_scrollbar_mode(ui->detail, LV_SCROLLBAR_MODE_OFF);

    //Write style for detail, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->detail, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->detail, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->detail, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes detail_arc_rpm
    ui->detail_arc_rpm = lv_arc_create(ui->detail);
    lv_arc_set_mode(ui->detail_arc_rpm, LV_ARC_MODE_NORMAL);
    lv_arc_set_range(ui->detail_arc_rpm, 0, 8000);
    lv_arc_set_bg_angles(ui->detail_arc_rpm, 140, 315);
    lv_arc_set_value(ui->detail_arc_rpm, 5600);
    lv_arc_set_rotation(ui->detail_arc_rpm, 0);
    lv_obj_set_pos(ui->detail_arc_rpm, 30, 30);
    lv_obj_set_size(ui->detail_arc_rpm, 405, 405);

    //Write style for detail_arc_rpm, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->detail_arc_rpm, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->detail_arc_rpm, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_opa(ui->detail_arc_rpm, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->detail_arc_rpm, 330, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->detail_arc_rpm, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->detail_arc_rpm, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->detail_arc_rpm, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->detail_arc_rpm, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->detail_arc_rpm, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for detail_arc_rpm, Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_arc_width(ui->detail_arc_rpm, 10, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_opa(ui->detail_arc_rpm, 255, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_color(ui->detail_arc_rpm, lv_color_hex(0xff6500), LV_PART_INDICATOR|LV_STATE_DEFAULT);

    //Write style for detail_arc_rpm, Part: LV_PART_KNOB, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->detail_arc_rpm, 0, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_all(ui->detail_arc_rpm, 5, LV_PART_KNOB|LV_STATE_DEFAULT);

    //Write codes detail_arc_speed
    ui->detail_arc_speed = lv_arc_create(ui->detail);
    lv_arc_set_mode(ui->detail_arc_speed, LV_ARC_MODE_NORMAL);
    lv_arc_set_range(ui->detail_arc_speed, 0, 200);
    lv_arc_set_bg_angles(ui->detail_arc_speed, 140, 315);
    lv_arc_set_value(ui->detail_arc_speed, 120);
    lv_arc_set_rotation(ui->detail_arc_speed, 0);
    lv_obj_set_pos(ui->detail_arc_speed, 8, 8);
    lv_obj_set_size(ui->detail_arc_speed, 450, 450);

    //Write style for detail_arc_speed, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->detail_arc_speed, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->detail_arc_speed, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_width(ui->detail_arc_speed, 15, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_opa(ui->detail_arc_speed, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_color(ui->detail_arc_speed, lv_color_hex(0x222222), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->detail_arc_speed, 330, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->detail_arc_speed, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->detail_arc_speed, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->detail_arc_speed, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->detail_arc_speed, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->detail_arc_speed, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for detail_arc_speed, Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_arc_width(ui->detail_arc_speed, 15, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_opa(ui->detail_arc_speed, 255, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_color(ui->detail_arc_speed, lv_color_hex(0x008dff), LV_PART_INDICATOR|LV_STATE_DEFAULT);

    //Write style for detail_arc_speed, Part: LV_PART_KNOB, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->detail_arc_speed, 0, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_all(ui->detail_arc_speed, 5, LV_PART_KNOB|LV_STATE_DEFAULT);

    //Write codes detail_arc_energy
    ui->detail_arc_energy = lv_arc_create(ui->detail);
    lv_arc_set_mode(ui->detail_arc_energy, LV_ARC_MODE_NORMAL);
    lv_arc_set_range(ui->detail_arc_energy, 0, 100);
    lv_arc_set_bg_angles(ui->detail_arc_energy, 2, 40);
    lv_arc_set_value(ui->detail_arc_energy, 70);
    lv_arc_set_rotation(ui->detail_arc_energy, 0);
    lv_obj_set_pos(ui->detail_arc_energy, 8, 8);
    lv_obj_set_size(ui->detail_arc_energy, 450, 450);

    //Write style for detail_arc_energy, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->detail_arc_energy, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->detail_arc_energy, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_width(ui->detail_arc_energy, 15, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_opa(ui->detail_arc_energy, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_color(ui->detail_arc_energy, lv_color_hex(0x222222), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->detail_arc_energy, 330, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->detail_arc_energy, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->detail_arc_energy, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->detail_arc_energy, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->detail_arc_energy, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->detail_arc_energy, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for detail_arc_energy, Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_arc_width(ui->detail_arc_energy, 15, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_opa(ui->detail_arc_energy, 255, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_color(ui->detail_arc_energy, lv_color_hex(0x00ff55), LV_PART_INDICATOR|LV_STATE_DEFAULT);

    //Write style for detail_arc_energy, Part: LV_PART_KNOB, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->detail_arc_energy, 0, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_all(ui->detail_arc_energy, 5, LV_PART_KNOB|LV_STATE_DEFAULT);

    //Write codes detail_arc_oil
    ui->detail_arc_oil = lv_arc_create(ui->detail);
    lv_arc_set_mode(ui->detail_arc_oil, LV_ARC_MODE_REVERSE);
    lv_arc_set_range(ui->detail_arc_oil, 0, 100);
    lv_arc_set_bg_angles(ui->detail_arc_oil, 320, 357);
    lv_arc_set_value(ui->detail_arc_oil, 70);
    lv_arc_set_rotation(ui->detail_arc_oil, 0);
    lv_obj_set_pos(ui->detail_arc_oil, 8, 8);
    lv_obj_set_size(ui->detail_arc_oil, 450, 450);

    //Write style for detail_arc_oil, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->detail_arc_oil, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->detail_arc_oil, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_width(ui->detail_arc_oil, 15, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_opa(ui->detail_arc_oil, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_color(ui->detail_arc_oil, lv_color_hex(0x222222), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->detail_arc_oil, 330, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->detail_arc_oil, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->detail_arc_oil, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->detail_arc_oil, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->detail_arc_oil, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->detail_arc_oil, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for detail_arc_oil, Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_arc_width(ui->detail_arc_oil, 15, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_opa(ui->detail_arc_oil, 255, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_color(ui->detail_arc_oil, lv_color_hex(0x00ff55), LV_PART_INDICATOR|LV_STATE_DEFAULT);

    //Write style for detail_arc_oil, Part: LV_PART_KNOB, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->detail_arc_oil, 0, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_all(ui->detail_arc_oil, 5, LV_PART_KNOB|LV_STATE_DEFAULT);

    //Write codes detail_cont_1
    ui->detail_cont_1 = lv_obj_create(ui->detail);
    lv_obj_set_pos(ui->detail_cont_1, 125, 125);
    lv_obj_set_size(ui->detail_cont_1, 215, 215);
    lv_obj_set_scrollbar_mode(ui->detail_cont_1, LV_SCROLLBAR_MODE_OFF);

    //Write style for detail_cont_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->detail_cont_1, 1, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->detail_cont_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->detail_cont_1, lv_color_hex(0x2a2a2a), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->detail_cont_1, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->detail_cont_1, 215, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->detail_cont_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->detail_cont_1, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->detail_cont_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->detail_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->detail_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->detail_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->detail_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->detail_cont_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes detail_label_rpm_number
    ui->detail_label_rpm_number = lv_label_create(ui->detail);
    lv_label_set_text(ui->detail_label_rpm_number, "3000");
    lv_label_set_long_mode(ui->detail_label_rpm_number, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->detail_label_rpm_number, 48, 219);
    lv_obj_set_size(ui->detail_label_rpm_number, 104, 27);

    //Write style for detail_label_rpm_number, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->detail_label_rpm_number, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->detail_label_rpm_number, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->detail_label_rpm_number, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->detail_label_rpm_number, &lv_font_montserratMedium_26, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->detail_label_rpm_number, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->detail_label_rpm_number, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->detail_label_rpm_number, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->detail_label_rpm_number, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->detail_label_rpm_number, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->detail_label_rpm_number, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->detail_label_rpm_number, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->detail_label_rpm_number, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->detail_label_rpm_number, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->detail_label_rpm_number, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes detail_label_time
    ui->detail_label_time = lv_label_create(ui->detail);
    lv_label_set_text(ui->detail_label_time, "23：00");
    lv_label_set_long_mode(ui->detail_label_time, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->detail_label_time, 173, 69);
    lv_obj_set_size(ui->detail_label_time, 120, 32);

    //Write style for detail_label_time, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->detail_label_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->detail_label_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->detail_label_time, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->detail_label_time, &lv_font_montserratMedium_30, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->detail_label_time, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->detail_label_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->detail_label_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->detail_label_time, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->detail_label_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->detail_label_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->detail_label_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->detail_label_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->detail_label_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->detail_label_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes detail_label_temp
    ui->detail_label_temp = lv_label_create(ui->detail);
    lv_label_set_text(ui->detail_label_temp, "32.5");
    lv_label_set_long_mode(ui->detail_label_temp, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->detail_label_temp, 176, 106);
    lv_obj_set_size(ui->detail_label_temp, 100, 32);

    //Write style for detail_label_temp, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->detail_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->detail_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->detail_label_temp, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->detail_label_temp, &lv_font_montserratMedium_24, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->detail_label_temp, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->detail_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->detail_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->detail_label_temp, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->detail_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->detail_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->detail_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->detail_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->detail_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->detail_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes detail_slider_energy
    ui->detail_slider_energy = lv_slider_create(ui->detail);
    lv_slider_set_range(ui->detail_slider_energy, -50, 50);
    lv_slider_set_mode(ui->detail_slider_energy, LV_SLIDER_MODE_RANGE);
    lv_slider_set_value(ui->detail_slider_energy, 30, LV_ANIM_OFF);
    lv_slider_set_left_value(ui->detail_slider_energy, 0, LV_ANIM_OFF);
    lv_obj_set_pos(ui->detail_slider_energy, 138, 230);
    lv_obj_set_size(ui->detail_slider_energy, 190, 5);

    //Write style for detail_slider_energy, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->detail_slider_energy, 60, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->detail_slider_energy, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->detail_slider_energy, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->detail_slider_energy, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_outline_width(ui->detail_slider_energy, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->detail_slider_energy, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for detail_slider_energy, Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->detail_slider_energy, 255, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->detail_slider_energy, lv_color_hex(0xff0000), LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->detail_slider_energy, LV_GRAD_DIR_NONE, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->detail_slider_energy, 8, LV_PART_INDICATOR|LV_STATE_DEFAULT);

    //Write style for detail_slider_energy, Part: LV_PART_KNOB, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->detail_slider_energy, 0, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->detail_slider_energy, 8, LV_PART_KNOB|LV_STATE_DEFAULT);

    //Write codes detail_label_speed_number
    ui->detail_label_speed_number = lv_label_create(ui->detail);
    lv_label_set_text(ui->detail_label_speed_number, "120");
    lv_label_set_long_mode(ui->detail_label_speed_number, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->detail_label_speed_number, 88, 158);
    lv_obj_set_size(ui->detail_label_speed_number, 289, 71);

    //Write style for detail_label_speed_number, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->detail_label_speed_number, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->detail_label_speed_number, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->detail_label_speed_number, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->detail_label_speed_number, &lv_font_montserratMedium_70, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->detail_label_speed_number, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->detail_label_speed_number, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->detail_label_speed_number, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->detail_label_speed_number, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->detail_label_speed_number, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->detail_label_speed_number, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->detail_label_speed_number, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->detail_label_speed_number, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->detail_label_speed_number, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->detail_label_speed_number, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes detail_label_energy_number
    ui->detail_label_energy_number = lv_label_create(ui->detail);
    lv_label_set_text(ui->detail_label_energy_number, "+24.8");
    lv_label_set_long_mode(ui->detail_label_energy_number, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->detail_label_energy_number, 3, 235);
    lv_obj_set_size(ui->detail_label_energy_number, 289, 71);

    //Write style for detail_label_energy_number, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->detail_label_energy_number, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->detail_label_energy_number, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->detail_label_energy_number, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->detail_label_energy_number, &lv_font_montserratMedium_60, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->detail_label_energy_number, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->detail_label_energy_number, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->detail_label_energy_number, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->detail_label_energy_number, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->detail_label_energy_number, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->detail_label_energy_number, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->detail_label_energy_number, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->detail_label_energy_number, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->detail_label_energy_number, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->detail_label_energy_number, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes detail_label_energy_number_2
    ui->detail_label_energy_number_2 = lv_label_create(ui->detail);
    lv_label_set_text(ui->detail_label_energy_number_2, "800");
    lv_label_set_long_mode(ui->detail_label_energy_number_2, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->detail_label_energy_number_2, 320, 232);
    lv_obj_set_size(ui->detail_label_energy_number_2, 100, 32);

    //Write style for detail_label_energy_number_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->detail_label_energy_number_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->detail_label_energy_number_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->detail_label_energy_number_2, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->detail_label_energy_number_2, &lv_font_montserratMedium_24, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->detail_label_energy_number_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->detail_label_energy_number_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->detail_label_energy_number_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->detail_label_energy_number_2, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->detail_label_energy_number_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->detail_label_energy_number_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->detail_label_energy_number_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->detail_label_energy_number_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->detail_label_energy_number_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->detail_label_energy_number_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes detail_label_oil_number
    ui->detail_label_oil_number = lv_label_create(ui->detail);
    lv_label_set_text(ui->detail_label_oil_number, "160");
    lv_label_set_long_mode(ui->detail_label_oil_number, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->detail_label_oil_number, 320, 207);
    lv_obj_set_size(ui->detail_label_oil_number, 100, 32);

    //Write style for detail_label_oil_number, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->detail_label_oil_number, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->detail_label_oil_number, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->detail_label_oil_number, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->detail_label_oil_number, &lv_font_montserratMedium_24, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->detail_label_oil_number, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->detail_label_oil_number, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->detail_label_oil_number, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->detail_label_oil_number, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->detail_label_oil_number, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->detail_label_oil_number, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->detail_label_oil_number, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->detail_label_oil_number, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->detail_label_oil_number, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->detail_label_oil_number, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes detail_label_trip
    ui->detail_label_trip = lv_label_create(ui->detail);
    lv_label_set_text(ui->detail_label_trip, "trip\n");
    lv_label_set_long_mode(ui->detail_label_trip, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->detail_label_trip, 179, 350);
    lv_obj_set_size(ui->detail_label_trip, 100, 32);

    //Write style for detail_label_trip, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->detail_label_trip, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->detail_label_trip, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->detail_label_trip, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->detail_label_trip, &lv_font_montserratMedium_20, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->detail_label_trip, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->detail_label_trip, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->detail_label_trip, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->detail_label_trip, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->detail_label_trip, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->detail_label_trip, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->detail_label_trip, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->detail_label_trip, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->detail_label_trip, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->detail_label_trip, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes detail_img_temp
    ui->detail_img_temp = lv_img_create(ui->detail);
    lv_obj_add_flag(ui->detail_img_temp, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->detail_img_temp, &_tempunit_alpha_20x19);
    lv_img_set_pivot(ui->detail_img_temp, 50,50);
    lv_img_set_angle(ui->detail_img_temp, 0);
    lv_obj_set_pos(ui->detail_img_temp, 258, 107);
    lv_obj_set_size(ui->detail_img_temp, 20, 19);

    //Write style for detail_img_temp, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->detail_img_temp, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_recolor(ui->detail_img_temp, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->detail_img_temp, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->detail_img_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->detail_img_temp, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes detail_label_1
    ui->detail_label_1 = lv_label_create(ui->detail);
    lv_label_set_text(ui->detail_label_1, "999999");
    lv_label_set_long_mode(ui->detail_label_1, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->detail_label_1, 154, 414);
    lv_obj_set_size(ui->detail_label_1, 100, 32);

    //Write style for detail_label_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->detail_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->detail_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->detail_label_1, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->detail_label_1, &lv_font_montserratMedium_20, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->detail_label_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->detail_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->detail_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->detail_label_1, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->detail_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->detail_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->detail_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->detail_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->detail_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->detail_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes detail_label_2
    ui->detail_label_2 = lv_label_create(ui->detail);
    lv_label_set_text(ui->detail_label_2, "14.9");
    lv_label_set_long_mode(ui->detail_label_2, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->detail_label_2, 55, 382);
    lv_obj_set_size(ui->detail_label_2, 100, 32);

    //Write style for detail_label_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->detail_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->detail_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->detail_label_2, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->detail_label_2, &lv_font_montserratMedium_20, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->detail_label_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->detail_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->detail_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->detail_label_2, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->detail_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->detail_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->detail_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->detail_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->detail_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->detail_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes detail_label_3
    ui->detail_label_3 = lv_label_create(ui->detail);
    lv_label_set_text(ui->detail_label_3, "5.6");
    lv_label_set_long_mode(ui->detail_label_3, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->detail_label_3, 179, 382);
    lv_obj_set_size(ui->detail_label_3, 100, 32);

    //Write style for detail_label_3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->detail_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->detail_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->detail_label_3, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->detail_label_3, &lv_font_montserratMedium_20, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->detail_label_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->detail_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->detail_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->detail_label_3, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->detail_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->detail_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->detail_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->detail_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->detail_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->detail_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes detail_label_4
    ui->detail_label_4 = lv_label_create(ui->detail);
    lv_label_set_text(ui->detail_label_4, "kw/H");
    lv_label_set_long_mode(ui->detail_label_4, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->detail_label_4, 141, 382);
    lv_obj_set_size(ui->detail_label_4, 100, 32);

    //Write style for detail_label_4, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->detail_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->detail_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->detail_label_4, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->detail_label_4, &lv_font_montserratMedium_20, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->detail_label_4, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->detail_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->detail_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->detail_label_4, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->detail_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->detail_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->detail_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->detail_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->detail_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->detail_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes detail_label_5
    ui->detail_label_5 = lv_label_create(ui->detail);
    lv_label_set_text(ui->detail_label_5, "L/km");
    lv_label_set_long_mode(ui->detail_label_5, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->detail_label_5, 268, 382);
    lv_obj_set_size(ui->detail_label_5, 100, 32);

    //Write style for detail_label_5, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->detail_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->detail_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->detail_label_5, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->detail_label_5, &lv_font_montserratMedium_20, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->detail_label_5, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->detail_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->detail_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->detail_label_5, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->detail_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->detail_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->detail_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->detail_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->detail_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->detail_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes detail_label_6
    ui->detail_label_6 = lv_label_create(ui->detail);
    lv_label_set_text(ui->detail_label_6, "km");
    lv_label_set_long_mode(ui->detail_label_6, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->detail_label_6, 231, 414);
    lv_obj_set_size(ui->detail_label_6, 100, 32);

    //Write style for detail_label_6, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->detail_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->detail_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->detail_label_6, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->detail_label_6, &lv_font_montserratMedium_20, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->detail_label_6, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->detail_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->detail_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->detail_label_6, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->detail_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->detail_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->detail_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->detail_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->detail_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->detail_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes detail_label_7
    ui->detail_label_7 = lv_label_create(ui->detail);
    lv_label_set_text(ui->detail_label_7, "kw");
    lv_label_set_long_mode(ui->detail_label_7, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->detail_label_7, 263, 269);
    lv_obj_set_size(ui->detail_label_7, 100, 32);

    //Write style for detail_label_7, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->detail_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->detail_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->detail_label_7, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->detail_label_7, &lv_font_montserratMedium_20, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->detail_label_7, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->detail_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->detail_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->detail_label_7, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->detail_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->detail_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->detail_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->detail_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->detail_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->detail_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //The custom code of detail.


    //Update current screen layout.
    lv_obj_update_layout(ui->detail);

}
