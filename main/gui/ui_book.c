//
// Created by Hessian on 2023/7/25.
//

#include <sys/dirent.h>
#include "esp_log.h"
#include "bsp_board.h"
#include "lvgl.h"
#include "app_sr.h"
#include "settings.h"
#include "ui_main.h"
#include "ui_book.h"
#include "constants.h"
#include "file_manager.h"

static const char *TAG = "ui_book";

static void (*g_book_end_cb)(void) = NULL;


static void ui_book_page_return_click_cb(lv_event_t *e)
{
    lv_obj_t *obj = lv_event_get_user_data(e);
    if (ui_get_btn_op_group()) {
        lv_group_remove_all_objs(ui_get_btn_op_group());
    }

    lv_obj_del(obj);
    if (g_book_end_cb) {
        g_book_end_cb();
    }
}

static void btn_event_cb(lv_event_t *event)
{
    lv_obj_t *img = (lv_obj_t *) event->user_data;
    const char *file_name = lv_list_get_btn_text(lv_obj_get_parent(event->target), event->target);
    char *file_name_with_path = (char *) heap_caps_malloc(256, MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);

    if (NULL != file_name_with_path) {
        /* Get full file name with mount point and folder path */
        strcpy(file_name_with_path, "S:/spiffs/");
        strcat(file_name_with_path, file_name);

        /* Set src of image with file name */
        lv_img_set_src(img, file_name_with_path);

        /* Align object */
        lv_obj_align(img, LV_ALIGN_CENTER, 80, 0);

        /* Only for debug */
        ESP_LOGI(TAG, "Display image file : %s", file_name_with_path);

        /* Don't forget to free allocated memory */
        free(file_name_with_path);
    }
}

static lv_obj_t* render_book_list(lv_obj_t *parent)
{
    lv_obj_t *list = lv_list_create(parent);
    lv_obj_set_size(list, lv_obj_get_width(parent) - 30, lv_obj_get_height(parent) - 30);
    lv_obj_set_style_border_width(list, 0, LV_STATE_DEFAULT);
    lv_obj_align(list, LV_ALIGN_LEFT_MID, 0, 30);

    /* Get file name in storage */
    struct dirent *p_dirent = NULL;
    DIR *p_dir_stream = opendir("/spiffs");

    lv_group_t *g_btn_op_group = ui_get_btn_op_group();

    /* Scan files in storage */
    while (true) {
        p_dirent = readdir(p_dir_stream);
        if (NULL != p_dirent) {
            // filter files by extension: .txt
            if (strstr(p_dirent->d_name, ".txt") == NULL) {
                continue;
            }
            lv_obj_t *btn = lv_list_add_btn(list, LV_SYMBOL_FILE, p_dirent->d_name);
            lv_group_add_obj(g_btn_op_group, btn);
            lv_obj_add_event_cb(btn, btn_event_cb, LV_EVENT_CLICKED, (void *) btn);
        } else {
            closedir(p_dir_stream);
            break;
        }
    }

    return list;
}

void ui_book_start(void (*fn)(void))
{
    g_book_end_cb = fn;

    // PAGE
    lv_obj_t *page = lv_obj_create(lv_scr_act());
    lv_obj_set_size(page, 290, 190);
    lv_obj_clear_flag(page, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_radius(page, 15, LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(page, 0, LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(page, 20, LV_PART_MAIN);
    lv_obj_set_style_shadow_opa(page, LV_OPA_30, LV_PART_MAIN);
    lv_obj_align(page, LV_ALIGN_TOP_MID, 0, 40);

    // RETURN BUTTON
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
    lv_obj_add_event_cb(btn_return, ui_book_page_return_click_cb, LV_EVENT_CLICKED, page);

    if (ui_get_btn_op_group()) {
        lv_group_add_obj(ui_get_btn_op_group(), btn_return);
    }

    lv_obj_t* list = render_book_list(page);

    lv_obj_t *lab = lv_label_create(page);
    lv_label_set_text(lab, "完整书名");
    lv_obj_align(lab, LV_ALIGN_TOP_MID, 50, 0);
}

