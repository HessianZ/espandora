/*
 * SPDX-FileCopyrightText: 2015-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Unlicense OR CC0-1.0
 */

#include "esp_log.h"
#include "bsp_board.h"
#include "lvgl.h"
#include "app_sr.h"
#include "settings.h"
#include "ui_main.h"
#include "ui_about_us.h"
#include "constants.h"

static const char *TAG = "ui_about_us";

static void (*g_about_us_end_cb)(void) = NULL;


static void ui_about_us_page_return_click_cb(lv_event_t *e)
{
    lv_obj_t *obj = lv_event_get_user_data(e);
    if (ui_get_btn_op_group()) {
        lv_group_remove_all_objs(ui_get_btn_op_group());
    }

    lv_obj_del(obj);
    if (g_about_us_end_cb) {
        g_about_us_end_cb();
    }
}

void ui_about_us_start(void (*fn)(void))
{
    g_about_us_end_cb = fn;

    lv_obj_t *page = lv_obj_create(lv_scr_act());
    lv_obj_set_size(page, 290, 190);
    lv_obj_clear_flag(page, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_radius(page, 15, LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(page, 0, LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(page, 20, LV_PART_MAIN);
    lv_obj_set_style_shadow_opa(page, LV_OPA_30, LV_PART_MAIN);
    lv_obj_align(page, LV_ALIGN_TOP_MID, 0, 40);

    lv_obj_t *btn_return = lv_btn_create(page);
    lv_obj_set_size(btn_return, 24, 24);
    lv_obj_add_style(btn_return, &ui_button_styles()->style, 0);
    lv_obj_add_style(btn_return, &ui_button_styles()->style_pr, LV_STATE_PRESSED);
    lv_obj_add_style(btn_return, &ui_button_styles()->style_focus, LV_STATE_FOCUS_KEY);
    lv_obj_add_style(btn_return, &ui_button_styles()->style_focus, LV_STATE_FOCUSED);
    lv_obj_align(btn_return, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_obj_t *lab_btn_text = lv_label_create(btn_return);
    lv_label_set_text_static(lab_btn_text, LV_SYMBOL_LEFT);
    lv_obj_set_style_text_color(lab_btn_text, lv_color_make(158, 158, 158), LV_STATE_DEFAULT);
    lv_obj_center(lab_btn_text);
    lv_obj_add_event_cb(btn_return, ui_about_us_page_return_click_cb, LV_EVENT_CLICKED, page);

    if (ui_get_btn_op_group()) {
        lv_group_add_obj(ui_get_btn_op_group(), btn_return);
    }

    const boards_info_t *board = bsp_board_get_info();
    LV_IMG_DECLARE(logo_jiong);
    lv_obj_t *img = lv_img_create(page);
    lv_img_set_zoom(img, 96);
//    lv_obj_set_size(img, 96, 96);
    lv_obj_align(img, LV_ALIGN_LEFT_MID, -24, -10);

    lv_img_set_src(img, &logo_jiong);

    char msg[256] = {0};
    snprintf(msg, sizeof(msg),
             "#C62828 ESPANDORA v%u.%u.%u#\n"
             "#000000 Author: # "        "#888888 囧大大王#\n"
             "#000000 Board: # "         "#888888 %s#\n"
             "#000000 ESP-IDF Ver: # "   "#888888 %s#",
             ESPANDORA_VERSION_MAJOR, ESPANDORA_VERSION_MINOR, ESPANDORA_VERSION_PATCH,
             board->name,
             esp_get_idf_version()
             );

    lv_obj_t *lab = lv_label_create(page);
    lv_label_set_recolor(lab, true);
    lv_label_set_text(lab, msg);
//    lv_obj_align(lab, LV_ALIGN_LEFT_MID, 0, -10);
    lv_obj_align_to(lab, img, LV_ALIGN_OUT_RIGHT_MID, -24, 0);
    LV_FONT_DECLARE(ESPANDORA_MAIN_FONT);
    lv_obj_set_style_text_font(lab, &ESPANDORA_MAIN_FONT, LV_PART_MAIN);
}
