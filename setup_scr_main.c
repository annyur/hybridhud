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



void setup_scr_main(lv_ui *ui)
{
    //Write codes main
    ui->main = lv_obj_create(NULL);
    lv_obj_set_size(ui->main, 466, 466);
    lv_obj_set_scrollbar_mode(ui->main, LV_SCROLLBAR_MODE_OFF);

    //Write style for main, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->main, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->main, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->main, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes main_arc_rpm
    ui->main_arc_rpm = lv_arc_create(ui->main);
    lv_arc_set_mode(ui->main_arc_rpm, LV_ARC_MODE_NORMAL);
    lv_arc_set_range(ui->main_arc_rpm, 0, 8000);
    lv_arc_set_bg_angles(ui->main_arc_rpm, 140, 315);
    lv_arc_set_value(ui->main_arc_rpm, 5600);
    lv_arc_set_rotation(ui->main_arc_rpm, 0);
    lv_obj_set_style_arc_rounded(ui->main_arc_rpm, 0,  LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_rounded(ui->main_arc_rpm, 0, LV_STATE_DEFAULT);
    lv_obj_set_pos(ui->main_arc_rpm, 18, 18);
    lv_obj_set_size(ui->main_arc_rpm, 430, 430);

    //Write style for main_arc_rpm, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->main_arc_rpm, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->main_arc_rpm, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_opa(ui->main_arc_rpm, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_arc_rpm, 330, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->main_arc_rpm, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->main_arc_rpm, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->main_arc_rpm, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->main_arc_rpm, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_arc_rpm, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for main_arc_rpm, Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_arc_width(ui->main_arc_rpm, 10, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_opa(ui->main_arc_rpm, 255, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_color(ui->main_arc_rpm, lv_color_hex(0x00ff55), LV_PART_INDICATOR|LV_STATE_DEFAULT);

    //Write style for main_arc_rpm, Part: LV_PART_KNOB, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->main_arc_rpm, 0, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_all(ui->main_arc_rpm, 5, LV_PART_KNOB|LV_STATE_DEFAULT);

    //Write codes main_meter_speed
    ui->main_meter_speed = lv_meter_create(ui->main);
    // add scale ui->main_meter_speed_scale_0
    ui->main_meter_speed_scale_0 = lv_meter_add_scale(ui->main_meter_speed);
    lv_meter_set_scale_ticks(ui->main_meter_speed, ui->main_meter_speed_scale_0, 41, 2, 20, lv_color_hex(0x999999));
    lv_meter_set_scale_major_ticks(ui->main_meter_speed, ui->main_meter_speed_scale_0, 5, 4, 30, lv_color_hex(0xbbbbbb), 13);
    lv_meter_set_scale_range(ui->main_meter_speed, ui->main_meter_speed_scale_0, 0, 220, 175, 140);

    // add arc for ui->main_meter_speed_scale_0
    ui->main_meter_speed_scale_0_arc_0 = lv_meter_add_arc(ui->main_meter_speed, ui->main_meter_speed_scale_0, 10, lv_color_hex(0x0071ff), 0);
    lv_meter_set_indicator_start_value(ui->main_meter_speed, ui->main_meter_speed_scale_0_arc_0, 0);
    lv_meter_set_indicator_end_value(ui->main_meter_speed, ui->main_meter_speed_scale_0_arc_0, 220);

    // add needle line for ui->main_meter_speed_scale_0.
    ui->main_meter_speed_scale_0_ndline_0 = lv_meter_add_needle_line(ui->main_meter_speed, ui->main_meter_speed_scale_0, 5, lv_color_hex(0xffffff), -25);
    lv_meter_set_indicator_value(ui->main_meter_speed, ui->main_meter_speed_scale_0_ndline_0, 200);
    lv_obj_set_pos(ui->main_meter_speed, 8, 8);
    lv_obj_set_size(ui->main_meter_speed, 450, 450);

    //Write style for main_meter_speed, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->main_meter_speed, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_meter_speed, 400, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->main_meter_speed, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->main_meter_speed, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->main_meter_speed, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->main_meter_speed, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->main_meter_speed, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_meter_speed, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for main_meter_speed, Part: LV_PART_TICKS, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->main_meter_speed, lv_color_hex(0x000000), LV_PART_TICKS|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->main_meter_speed, &lv_font_LiberationSans_20, LV_PART_TICKS|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->main_meter_speed, 255, LV_PART_TICKS|LV_STATE_DEFAULT);

    //Write style for main_meter_speed, Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->main_meter_speed, 0, LV_PART_INDICATOR|LV_STATE_DEFAULT);

    //Write codes main_arc_energy
    ui->main_arc_energy = lv_arc_create(ui->main);
    lv_arc_set_mode(ui->main_arc_energy, LV_ARC_MODE_NORMAL);
    lv_arc_set_range(ui->main_arc_energy, 0, 100);
    lv_arc_set_bg_angles(ui->main_arc_energy, 2, 40);
    lv_arc_set_value(ui->main_arc_energy, 70);
    lv_arc_set_rotation(ui->main_arc_energy, 0);
    lv_obj_set_pos(ui->main_arc_energy, 8, 8);
    lv_obj_set_size(ui->main_arc_energy, 450, 450);

    //Write style for main_arc_energy, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->main_arc_energy, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->main_arc_energy, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_width(ui->main_arc_energy, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_opa(ui->main_arc_energy, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_color(ui->main_arc_energy, lv_color_hex(0x555555), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_arc_energy, 330, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->main_arc_energy, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->main_arc_energy, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->main_arc_energy, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->main_arc_energy, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_arc_energy, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for main_arc_energy, Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_arc_width(ui->main_arc_energy, 8, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_opa(ui->main_arc_energy, 255, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_color(ui->main_arc_energy, lv_color_hex(0x00ff55), LV_PART_INDICATOR|LV_STATE_DEFAULT);

    //Write style for main_arc_energy, Part: LV_PART_KNOB, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->main_arc_energy, 0, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_all(ui->main_arc_energy, 5, LV_PART_KNOB|LV_STATE_DEFAULT);

    //Write codes main_arc_oil
    ui->main_arc_oil = lv_arc_create(ui->main);
    lv_arc_set_mode(ui->main_arc_oil, LV_ARC_MODE_REVERSE);
    lv_arc_set_range(ui->main_arc_oil, 0, 100);
    lv_arc_set_bg_angles(ui->main_arc_oil, 320, 358);
    lv_arc_set_value(ui->main_arc_oil, 70);
    lv_arc_set_rotation(ui->main_arc_oil, 0);
    lv_obj_set_pos(ui->main_arc_oil, 8, 8);
    lv_obj_set_size(ui->main_arc_oil, 450, 450);

    //Write style for main_arc_oil, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->main_arc_oil, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->main_arc_oil, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_width(ui->main_arc_oil, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_opa(ui->main_arc_oil, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_color(ui->main_arc_oil, lv_color_hex(0x555555), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_arc_oil, 330, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->main_arc_oil, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->main_arc_oil, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->main_arc_oil, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->main_arc_oil, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_arc_oil, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for main_arc_oil, Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_arc_width(ui->main_arc_oil, 8, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_opa(ui->main_arc_oil, 255, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_color(ui->main_arc_oil, lv_color_hex(0x00ff55), LV_PART_INDICATOR|LV_STATE_DEFAULT);

    //Write style for main_arc_oil, Part: LV_PART_KNOB, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->main_arc_oil, 0, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_all(ui->main_arc_oil, 5, LV_PART_KNOB|LV_STATE_DEFAULT);

    //Write codes main_img_temp
    ui->main_img_temp = lv_img_create(ui->main);
    lv_obj_add_flag(ui->main_img_temp, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->main_img_temp, &_tempunit_alpha_20x19);
    lv_img_set_pivot(ui->main_img_temp, 50,50);
    lv_img_set_angle(ui->main_img_temp, 0);
    lv_obj_set_pos(ui->main_img_temp, 259, 106);
    lv_obj_set_size(ui->main_img_temp, 20, 19);

    //Write style for main_img_temp, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->main_img_temp, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_recolor(ui->main_img_temp, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->main_img_temp, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_img_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->main_img_temp, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes main_label_rpm_number
    ui->main_label_rpm_number = lv_label_create(ui->main);
    lv_label_set_text(ui->main_label_rpm_number, "3000");
    lv_label_set_long_mode(ui->main_label_rpm_number, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->main_label_rpm_number, 46, 219);
    lv_obj_set_size(ui->main_label_rpm_number, 104, 27);

    //Write style for main_label_rpm_number, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->main_label_rpm_number, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_label_rpm_number, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->main_label_rpm_number, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->main_label_rpm_number, &lv_font_montserratMedium_26, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->main_label_rpm_number, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->main_label_rpm_number, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->main_label_rpm_number, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->main_label_rpm_number, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->main_label_rpm_number, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->main_label_rpm_number, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->main_label_rpm_number, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->main_label_rpm_number, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->main_label_rpm_number, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_label_rpm_number, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes main_label_time
    ui->main_label_time = lv_label_create(ui->main);
    lv_label_set_text(ui->main_label_time, "23：00");
    lv_label_set_long_mode(ui->main_label_time, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->main_label_time, 173, 65);
    lv_obj_set_size(ui->main_label_time, 120, 32);

    //Write style for main_label_time, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->main_label_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_label_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->main_label_time, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->main_label_time, &lv_font_montserratMedium_30, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->main_label_time, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->main_label_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->main_label_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->main_label_time, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->main_label_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->main_label_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->main_label_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->main_label_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->main_label_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_label_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes main_label_temp
    ui->main_label_temp = lv_label_create(ui->main);
    lv_label_set_text(ui->main_label_temp, "32.5");
    lv_label_set_long_mode(ui->main_label_temp, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->main_label_temp, 176, 106);
    lv_obj_set_size(ui->main_label_temp, 100, 32);

    //Write style for main_label_temp, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->main_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->main_label_temp, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->main_label_temp, &lv_font_montserratMedium_24, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->main_label_temp, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->main_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->main_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->main_label_temp, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->main_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->main_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->main_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->main_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->main_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes main_slider_energy
    ui->main_slider_energy = lv_slider_create(ui->main);
    lv_slider_set_range(ui->main_slider_energy, -50, 50);
    lv_slider_set_mode(ui->main_slider_energy, LV_SLIDER_MODE_RANGE);
    lv_slider_set_value(ui->main_slider_energy, 30, LV_ANIM_OFF);
    lv_slider_set_left_value(ui->main_slider_energy, 0, LV_ANIM_OFF);
    lv_obj_set_pos(ui->main_slider_energy, 138, 230);
    lv_obj_set_size(ui->main_slider_energy, 190, 5);

    //Write style for main_slider_energy, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->main_slider_energy, 60, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->main_slider_energy, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->main_slider_energy, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_slider_energy, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_outline_width(ui->main_slider_energy, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_slider_energy, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for main_slider_energy, Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->main_slider_energy, 255, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->main_slider_energy, lv_color_hex(0xff0000), LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->main_slider_energy, LV_GRAD_DIR_NONE, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_slider_energy, 8, LV_PART_INDICATOR|LV_STATE_DEFAULT);

    //Write style for main_slider_energy, Part: LV_PART_KNOB, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->main_slider_energy, 0, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_slider_energy, 8, LV_PART_KNOB|LV_STATE_DEFAULT);

    //Write codes main_label_speed_number
    ui->main_label_speed_number = lv_label_create(ui->main);
    lv_label_set_text(ui->main_label_speed_number, "120");
    lv_label_set_long_mode(ui->main_label_speed_number, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->main_label_speed_number, 88, 158);
    lv_obj_set_size(ui->main_label_speed_number, 289, 71);

    //Write style for main_label_speed_number, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->main_label_speed_number, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_label_speed_number, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->main_label_speed_number, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->main_label_speed_number, &lv_font_montserratMedium_70, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->main_label_speed_number, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->main_label_speed_number, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->main_label_speed_number, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->main_label_speed_number, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->main_label_speed_number, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->main_label_speed_number, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->main_label_speed_number, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->main_label_speed_number, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->main_label_speed_number, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_label_speed_number, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes main_label_energy_number
    ui->main_label_energy_number = lv_label_create(ui->main);
    lv_label_set_text(ui->main_label_energy_number, "+24.8");
    lv_label_set_long_mode(ui->main_label_energy_number, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->main_label_energy_number, 88, 235);
    lv_obj_set_size(ui->main_label_energy_number, 289, 71);

    //Write style for main_label_energy_number, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->main_label_energy_number, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_label_energy_number, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->main_label_energy_number, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->main_label_energy_number, &lv_font_montserratMedium_60, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->main_label_energy_number, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->main_label_energy_number, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->main_label_energy_number, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->main_label_energy_number, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->main_label_energy_number, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->main_label_energy_number, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->main_label_energy_number, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->main_label_energy_number, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->main_label_energy_number, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_label_energy_number, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes main_label_energy_number_2
    ui->main_label_energy_number_2 = lv_label_create(ui->main);
    lv_label_set_text(ui->main_label_energy_number_2, "Label");
    lv_label_set_long_mode(ui->main_label_energy_number_2, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->main_label_energy_number_2, 330, 237);
    lv_obj_set_size(ui->main_label_energy_number_2, 100, 32);

    //Write style for main_label_energy_number_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->main_label_energy_number_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_label_energy_number_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->main_label_energy_number_2, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->main_label_energy_number_2, &lv_font_montserratMedium_24, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->main_label_energy_number_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->main_label_energy_number_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->main_label_energy_number_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->main_label_energy_number_2, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->main_label_energy_number_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->main_label_energy_number_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->main_label_energy_number_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->main_label_energy_number_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->main_label_energy_number_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_label_energy_number_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes main_label_oil_number
    ui->main_label_oil_number = lv_label_create(ui->main);
    lv_label_set_text(ui->main_label_oil_number, "Label");
    lv_label_set_long_mode(ui->main_label_oil_number, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->main_label_oil_number, 330, 197);
    lv_obj_set_size(ui->main_label_oil_number, 100, 32);

    //Write style for main_label_oil_number, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->main_label_oil_number, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_label_oil_number, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->main_label_oil_number, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->main_label_oil_number, &lv_font_montserratMedium_24, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->main_label_oil_number, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->main_label_oil_number, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->main_label_oil_number, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->main_label_oil_number, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->main_label_oil_number, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->main_label_oil_number, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->main_label_oil_number, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->main_label_oil_number, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->main_label_oil_number, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_label_oil_number, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes main_label_trip
    ui->main_label_trip = lv_label_create(ui->main);
    lv_label_set_text(ui->main_label_trip, "trip\n");
    lv_label_set_long_mode(ui->main_label_trip, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->main_label_trip, 183, 369);
    lv_obj_set_size(ui->main_label_trip, 100, 32);

    //Write style for main_label_trip, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->main_label_trip, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->main_label_trip, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->main_label_trip, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->main_label_trip, &lv_font_montserratMedium_20, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->main_label_trip, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->main_label_trip, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->main_label_trip, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->main_label_trip, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->main_label_trip, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->main_label_trip, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->main_label_trip, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->main_label_trip, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->main_label_trip, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->main_label_trip, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //The custom code of main.


    //Update current screen layout.
    lv_obj_update_layout(ui->main);

}
