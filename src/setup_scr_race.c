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



void setup_scr_race(lv_ui *ui)
{
    //Write codes race
    ui->race = lv_obj_create(NULL);
    lv_obj_set_size(ui->race, 466, 466);
    lv_obj_set_scrollbar_mode(ui->race, LV_SCROLLBAR_MODE_OFF);

    //Write style for race, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->race, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->race, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->race, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes race_cont_G
    ui->race_cont_G = lv_obj_create(ui->race);
    lv_obj_set_pos(ui->race_cont_G, 0, 0);
    lv_obj_set_size(ui->race_cont_G, 466, 466);
    lv_obj_set_scrollbar_mode(ui->race_cont_G, LV_SCROLLBAR_MODE_OFF);

    //Write style for race_cont_G, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->race_cont_G, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->race_cont_G, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->race_cont_G, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->race_cont_G, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->race_cont_G, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->race_cont_G, 466, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->race_cont_G, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->race_cont_G, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->race_cont_G, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->race_cont_G, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->race_cont_G, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes race_meter_G
    ui->race_meter_G = lv_meter_create(ui->race_cont_G);
    // add scale ui->race_meter_G_scale_0
    ui->race_meter_G_scale_0 = lv_meter_add_scale(ui->race_meter_G);
    lv_meter_set_scale_ticks(ui->race_meter_G, ui->race_meter_G_scale_0, 150, 2, 10, lv_color_hex(0xffffff));
    lv_meter_set_scale_range(ui->race_meter_G, ui->race_meter_G_scale_0, 0, 20, 360, 90);
    lv_obj_set_pos(ui->race_meter_G, 73, 73);
    lv_obj_set_size(ui->race_meter_G, 320, 320);

    //Write style for race_meter_G, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->race_meter_G, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->race_meter_G, 300, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->race_meter_G, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->race_meter_G, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->race_meter_G, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->race_meter_G, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->race_meter_G, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->race_meter_G, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for race_meter_G, Part: LV_PART_TICKS, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->race_meter_G, lv_color_hex(0xff0000), LV_PART_TICKS|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->race_meter_G, &lv_font_montserratMedium_12, LV_PART_TICKS|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->race_meter_G, 255, LV_PART_TICKS|LV_STATE_DEFAULT);

    //Write style for race_meter_G, Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->race_meter_G, 255, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->race_meter_G, lv_color_hex(0x000000), LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->race_meter_G, LV_GRAD_DIR_NONE, LV_PART_INDICATOR|LV_STATE_DEFAULT);

    //Write codes race_label_7
    ui->race_label_7 = lv_label_create(ui->race_cont_G);
    lv_label_set_text(ui->race_label_7, "");
    lv_label_set_long_mode(ui->race_label_7, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->race_label_7, 378, 199);
    lv_obj_set_size(ui->race_label_7, 16, 67);

    //Write style for race_label_7, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->race_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->race_label_7, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->race_label_7, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->race_label_7, &lv_font_montserratMedium_18, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->race_label_7, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->race_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->race_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->race_label_7, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->race_label_7, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->race_label_7, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->race_label_7, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->race_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->race_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->race_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->race_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->race_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes race_label_6
    ui->race_label_6 = lv_label_create(ui->race_cont_G);
    lv_label_set_text(ui->race_label_6, "");
    lv_label_set_long_mode(ui->race_label_6, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->race_label_6, 72, 199);
    lv_obj_set_size(ui->race_label_6, 16, 67);

    //Write style for race_label_6, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->race_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->race_label_6, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->race_label_6, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->race_label_6, &lv_font_montserratMedium_18, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->race_label_6, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->race_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->race_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->race_label_6, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->race_label_6, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->race_label_6, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->race_label_6, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->race_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->race_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->race_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->race_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->race_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes race_label_G_top
    ui->race_label_G_top = lv_label_create(ui->race_cont_G);
    lv_label_set_text(ui->race_label_G_top, "0.00");
    lv_label_set_long_mode(ui->race_label_G_top, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->race_label_G_top, 198, 73);
    lv_obj_set_size(ui->race_label_G_top, 69, 16);

    //Write style for race_label_G_top, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->race_label_G_top, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->race_label_G_top, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->race_label_G_top, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->race_label_G_top, &lv_font_montserratMedium_18, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->race_label_G_top, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->race_label_G_top, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->race_label_G_top, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->race_label_G_top, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->race_label_G_top, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->race_label_G_top, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->race_label_G_top, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->race_label_G_top, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->race_label_G_top, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->race_label_G_top, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->race_label_G_top, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->race_label_G_top, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes race_label_G_down
    ui->race_label_G_down = lv_label_create(ui->race_cont_G);
    lv_label_set_text(ui->race_label_G_down, "0.00");
    lv_label_set_long_mode(ui->race_label_G_down, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->race_label_G_down, 196, 377);
    lv_obj_set_size(ui->race_label_G_down, 73, 16);

    //Write style for race_label_G_down, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->race_label_G_down, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->race_label_G_down, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->race_label_G_down, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->race_label_G_down, &lv_font_montserratMedium_18, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->race_label_G_down, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->race_label_G_down, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->race_label_G_down, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->race_label_G_down, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->race_label_G_down, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->race_label_G_down, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->race_label_G_down, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->race_label_G_down, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->race_label_G_down, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->race_label_G_down, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->race_label_G_down, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->race_label_G_down, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes race_label_G_right
    ui->race_label_G_right = lv_label_create(ui->race_cont_G);
    lv_label_set_text(ui->race_label_G_right, "0.00");
    lv_label_set_long_mode(ui->race_label_G_right, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->race_label_G_right, 351, 225);
    lv_obj_set_size(ui->race_label_G_right, 74, 16);

    //Write style for race_label_G_right, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->race_label_G_right, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->race_label_G_right, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->race_label_G_right, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->race_label_G_right, &lv_font_montserratMedium_18, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->race_label_G_right, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->race_label_G_right, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->race_label_G_right, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->race_label_G_right, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->race_label_G_right, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->race_label_G_right, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->race_label_G_right, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->race_label_G_right, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->race_label_G_right, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->race_label_G_right, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes race_label_left
    ui->race_label_left = lv_label_create(ui->race_cont_G);
    lv_label_set_text(ui->race_label_left, "0.00");
    lv_label_set_long_mode(ui->race_label_left, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->race_label_left, 40, 225);
    lv_obj_set_size(ui->race_label_left, 74, 16);

    //Write style for race_label_left, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->race_label_left, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->race_label_left, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->race_label_left, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->race_label_left, &lv_font_montserratMedium_18, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->race_label_left, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->race_label_left, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->race_label_left, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->race_label_left, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->race_label_left, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->race_label_left, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->race_label_left, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->race_label_left, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->race_label_left, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->race_label_left, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes race_arc_4
    ui->race_arc_4 = lv_arc_create(ui->race);
    lv_arc_set_mode(ui->race_arc_4, LV_ARC_MODE_NORMAL);
    lv_arc_set_range(ui->race_arc_4, 0, 200);
    lv_arc_set_bg_angles(ui->race_arc_4, 180, 230);
    lv_arc_set_value(ui->race_arc_4, 200);
    lv_arc_set_rotation(ui->race_arc_4, 180);
    lv_obj_set_pos(ui->race_arc_4, 8, 8);
    lv_obj_set_size(ui->race_arc_4, 450, 450);

    //Write style for race_arc_4, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->race_arc_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->race_arc_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->race_arc_4, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->race_arc_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->race_arc_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->race_arc_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->race_arc_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->race_arc_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_width(ui->race_arc_4, 15, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_opa(ui->race_arc_4, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_color(ui->race_arc_4, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for race_arc_4, Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_arc_width(ui->race_arc_4, 15, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_opa(ui->race_arc_4, 255, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_color(ui->race_arc_4, lv_color_hex(0xff6500), LV_PART_INDICATOR|LV_STATE_DEFAULT);

    //Write style for race_arc_4, Part: LV_PART_KNOB, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->race_arc_4, 0, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_all(ui->race_arc_4, 0, LV_PART_KNOB|LV_STATE_DEFAULT);

    //Write codes race_arc_3
    ui->race_arc_3 = lv_arc_create(ui->race);
    lv_arc_set_mode(ui->race_arc_3, LV_ARC_MODE_NORMAL);
    lv_arc_set_range(ui->race_arc_3, 0, 200);
    lv_arc_set_bg_angles(ui->race_arc_3, 180, 230);
    lv_arc_set_value(ui->race_arc_3, 200);
    lv_arc_set_rotation(ui->race_arc_3, 0);
    lv_obj_set_pos(ui->race_arc_3, 8, 8);
    lv_obj_set_size(ui->race_arc_3, 450, 450);

    //Write style for race_arc_3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->race_arc_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->race_arc_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_width(ui->race_arc_3, 15, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_opa(ui->race_arc_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_color(ui->race_arc_3, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->race_arc_3, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->race_arc_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->race_arc_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->race_arc_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->race_arc_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->race_arc_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for race_arc_3, Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_arc_width(ui->race_arc_3, 15, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_opa(ui->race_arc_3, 255, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_color(ui->race_arc_3, lv_color_hex(0xff6500), LV_PART_INDICATOR|LV_STATE_DEFAULT);

    //Write style for race_arc_3, Part: LV_PART_KNOB, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->race_arc_3, 0, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_all(ui->race_arc_3, 0, LV_PART_KNOB|LV_STATE_DEFAULT);

    //Write codes race_arc_2
    ui->race_arc_2 = lv_arc_create(ui->race);
    lv_arc_set_mode(ui->race_arc_2, LV_ARC_MODE_REVERSE);
    lv_arc_set_range(ui->race_arc_2, 0, 200);
    lv_arc_set_bg_angles(ui->race_arc_2, 130, 180);
    lv_arc_set_value(ui->race_arc_2, 200);
    lv_arc_set_rotation(ui->race_arc_2, 0);
    lv_obj_set_pos(ui->race_arc_2, 8, 8);
    lv_obj_set_size(ui->race_arc_2, 450, 450);

    //Write style for race_arc_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->race_arc_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->race_arc_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_width(ui->race_arc_2, 15, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_opa(ui->race_arc_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_color(ui->race_arc_2, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->race_arc_2, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->race_arc_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->race_arc_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->race_arc_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->race_arc_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->race_arc_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for race_arc_2, Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_arc_width(ui->race_arc_2, 15, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_opa(ui->race_arc_2, 255, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_color(ui->race_arc_2, lv_color_hex(0xff6500), LV_PART_INDICATOR|LV_STATE_DEFAULT);

    //Write style for race_arc_2, Part: LV_PART_KNOB, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->race_arc_2, 0, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_all(ui->race_arc_2, 0, LV_PART_KNOB|LV_STATE_DEFAULT);

    //Write codes race_arc_1
    ui->race_arc_1 = lv_arc_create(ui->race);
    lv_arc_set_mode(ui->race_arc_1, LV_ARC_MODE_REVERSE);
    lv_arc_set_range(ui->race_arc_1, 0, 200);
    lv_arc_set_bg_angles(ui->race_arc_1, 130, 180);
    lv_arc_set_value(ui->race_arc_1, 200);
    lv_arc_set_rotation(ui->race_arc_1, 180);
    lv_obj_set_pos(ui->race_arc_1, 8, 8);
    lv_obj_set_size(ui->race_arc_1, 450, 450);

    //Write style for race_arc_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->race_arc_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->race_arc_1, 6, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->race_arc_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->race_arc_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->race_arc_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->race_arc_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->race_arc_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->race_arc_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_width(ui->race_arc_1, 15, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_opa(ui->race_arc_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_color(ui->race_arc_1, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for race_arc_1, Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_arc_width(ui->race_arc_1, 15, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_opa(ui->race_arc_1, 255, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_arc_color(ui->race_arc_1, lv_color_hex(0xff6500), LV_PART_INDICATOR|LV_STATE_DEFAULT);

    //Write style for race_arc_1, Part: LV_PART_KNOB, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->race_arc_1, 0, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_all(ui->race_arc_1, 0, LV_PART_KNOB|LV_STATE_DEFAULT);

    //Write codes race_label_4
    ui->race_label_4 = lv_label_create(ui->race);
    lv_label_set_text(ui->race_label_4, "kw");
    lv_label_set_long_mode(ui->race_label_4, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->race_label_4, 250, 306);
    lv_obj_set_size(ui->race_label_4, 100, 32);

    //Write style for race_label_4, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->race_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->race_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->race_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->race_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->race_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->race_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->race_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->race_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->race_label_4, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->race_label_4, &lv_font_montserratMedium_20, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->race_label_4, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->race_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->race_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->race_label_4, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes race_label_speed_number
    ui->race_label_speed_number = lv_label_create(ui->race);
    lv_label_set_text(ui->race_label_speed_number, "120");
    lv_label_set_long_mode(ui->race_label_speed_number, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->race_label_speed_number, 88, 110);
    lv_obj_set_size(ui->race_label_speed_number, 289, 71);

    //Write style for race_label_speed_number, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->race_label_speed_number, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->race_label_speed_number, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->race_label_speed_number, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->race_label_speed_number, &lv_font_montserratMedium_70, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->race_label_speed_number, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->race_label_speed_number, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->race_label_speed_number, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->race_label_speed_number, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->race_label_speed_number, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->race_label_speed_number, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->race_label_speed_number, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->race_label_speed_number, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->race_label_speed_number, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->race_label_speed_number, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes race_label_energy_number
    ui->race_label_energy_number = lv_label_create(ui->race);
    lv_label_set_text(ui->race_label_energy_number, "+24.8");
    lv_label_set_long_mode(ui->race_label_energy_number, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->race_label_energy_number, -12, 283);
    lv_obj_set_size(ui->race_label_energy_number, 289, 71);

    //Write style for race_label_energy_number, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->race_label_energy_number, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->race_label_energy_number, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->race_label_energy_number, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->race_label_energy_number, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->race_label_energy_number, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->race_label_energy_number, &lv_font_montserratMedium_48, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->race_label_energy_number, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->race_label_energy_number, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->race_label_energy_number, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->race_label_energy_number, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->race_label_energy_number, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->race_label_energy_number, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->race_label_energy_number, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->race_label_energy_number, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes race_btn_G
    ui->race_btn_G = lv_btn_create(ui->race);
    ui->race_btn_G_label = lv_label_create(ui->race_btn_G);
    lv_label_set_text(ui->race_btn_G_label, "G");
    lv_label_set_long_mode(ui->race_btn_G_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->race_btn_G_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->race_btn_G, 0, LV_STATE_DEFAULT);
    lv_obj_set_width(ui->race_btn_G_label, LV_PCT(100));
    lv_obj_set_pos(ui->race_btn_G, 208, 208);
    lv_obj_set_size(ui->race_btn_G, 50, 50);

    //Write style for race_btn_G, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->race_btn_G, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->race_btn_G, 50, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->race_btn_G, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->race_btn_G, &lv_font_montserratMedium_48, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->race_btn_G, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->race_btn_G, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->race_btn_G, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->race_btn_G, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes race_led_G
    ui->race_led_G = lv_led_create(ui->race);
    lv_led_set_brightness(ui->race_led_G, 255);
    lv_led_set_color(ui->race_led_G, lv_color_hex(0xff6500));
    lv_obj_set_pos(ui->race_led_G, 228, 228);
    lv_obj_set_size(ui->race_led_G, 10, 10);

    //Write codes race_label_ECT_temp
    ui->race_label_ECT_temp = lv_label_create(ui->race);
    lv_label_set_text(ui->race_label_ECT_temp, "89.7");
    lv_label_set_long_mode(ui->race_label_ECT_temp, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->race_label_ECT_temp, 145, 421);
    lv_obj_set_size(ui->race_label_ECT_temp, 60, 18);

    //Write style for race_label_ECT_temp, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_radius(ui->race_label_ECT_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->race_label_ECT_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->race_label_ECT_temp, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->race_label_ECT_temp, &lv_font_montserratMedium_18, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->race_label_ECT_temp, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->race_label_ECT_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->race_label_ECT_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->race_label_ECT_temp, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->race_label_ECT_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->race_label_ECT_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->race_label_ECT_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->race_label_ECT_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->race_label_ECT_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->race_label_ECT_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes race_label_MOTOR_temp
    ui->race_label_MOTOR_temp = lv_label_create(ui->race);
    lv_label_set_text(ui->race_label_MOTOR_temp, "24.2");
    lv_label_set_long_mode(ui->race_label_MOTOR_temp, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->race_label_MOTOR_temp, 246, 421);
    lv_obj_set_size(ui->race_label_MOTOR_temp, 60, 18);

    //Write style for race_label_MOTOR_temp, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_radius(ui->race_label_MOTOR_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->race_label_MOTOR_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->race_label_MOTOR_temp, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->race_label_MOTOR_temp, &lv_font_montserratMedium_18, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->race_label_MOTOR_temp, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->race_label_MOTOR_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->race_label_MOTOR_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->race_label_MOTOR_temp, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->race_label_MOTOR_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->race_label_MOTOR_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->race_label_MOTOR_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->race_label_MOTOR_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->race_label_MOTOR_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->race_label_MOTOR_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes race_img_temp2
    ui->race_img_temp2 = lv_img_create(ui->race);
    lv_obj_add_flag(ui->race_img_temp2, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->race_img_temp2, &_tempunit_alpha_16x15);
    lv_img_set_pivot(ui->race_img_temp2, 50,50);
    lv_img_set_angle(ui->race_img_temp2, 0);
    lv_obj_set_pos(ui->race_img_temp2, 211, 423);
    lv_obj_set_size(ui->race_img_temp2, 16, 15);

    //Write style for race_img_temp2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_radius(ui->race_img_temp2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->race_img_temp2, true, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_recolor_opa(ui->race_img_temp2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_recolor(ui->race_img_temp2, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->race_img_temp2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes race_img_temp1
    ui->race_img_temp1 = lv_img_create(ui->race);
    lv_obj_add_flag(ui->race_img_temp1, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->race_img_temp1, &_tempunit_alpha_16x15);
    lv_img_set_pivot(ui->race_img_temp1, 50,50);
    lv_img_set_angle(ui->race_img_temp1, 0);
    lv_obj_set_pos(ui->race_img_temp1, 306, 423);
    lv_obj_set_size(ui->race_img_temp1, 16, 15);

    //Write style for race_img_temp1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_radius(ui->race_img_temp1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->race_img_temp1, true, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_recolor_opa(ui->race_img_temp1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_recolor(ui->race_img_temp1, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->race_img_temp1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes race_label_9
    ui->race_label_9 = lv_label_create(ui->race);
    lv_label_set_text(ui->race_label_9, "23:00");
    lv_label_set_long_mode(ui->race_label_9, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->race_label_9, 173, 21);
    lv_obj_set_size(ui->race_label_9, 120, 32);

    //Write style for race_label_9, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_radius(ui->race_label_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->race_label_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->race_label_9, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->race_label_9, &lv_font_montserratMedium_30, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->race_label_9, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->race_label_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->race_label_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->race_label_9, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->race_label_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->race_label_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->race_label_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->race_label_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->race_label_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->race_label_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes race_img_1
    ui->race_img_1 = lv_img_create(ui->race);
    lv_obj_add_flag(ui->race_img_1, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->race_img_1, &_e_alpha_18x18);
    lv_img_set_pivot(ui->race_img_1, 50,50);
    lv_img_set_angle(ui->race_img_1, 0);
    lv_obj_set_pos(ui->race_img_1, 243, 421);
    lv_obj_set_size(ui->race_img_1, 18, 18);

    //Write style for race_img_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_radius(ui->race_img_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->race_img_1, true, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_recolor_opa(ui->race_img_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->race_img_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes race_img_2
    ui->race_img_2 = lv_img_create(ui->race);
    lv_obj_add_flag(ui->race_img_2, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->race_img_2, &_o_alpha_18x18);
    lv_img_set_pivot(ui->race_img_2, 50,50);
    lv_img_set_angle(ui->race_img_2, 0);
    lv_obj_set_pos(ui->race_img_2, 141, 421);
    lv_obj_set_size(ui->race_img_2, 18, 18);

    //Write style for race_img_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_radius(ui->race_img_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->race_img_2, true, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_recolor_opa(ui->race_img_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->race_img_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);

    //The custom code of race.


    //Update current screen layout.
    lv_obj_update_layout(ui->race);

}
