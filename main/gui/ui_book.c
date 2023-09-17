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
#include "ui_book_read.h"
#include "string.h"

#define MAX_BOOKS 10

static const char *TAG = "ui_book";

static void (*g_book_end_cb)(void) = NULL;

static lv_obj_t *g_page = NULL;

static BookInfo *g_book_list = NULL;
static uint16_t g_book_count = 0;

static void book_read_end_cb(void)
{
    ESP_LOGI(TAG, "book read end");

    ui_book_start(NULL);
}

static void ui_book_free()
{
    if (ui_get_btn_op_group()) {
        lv_group_remove_all_objs(ui_get_btn_op_group());
    }

    lv_obj_del(g_page);
    g_page = NULL;

    free(g_book_list);
}

static void ui_book_page_return_click_cb(lv_event_t *e)
{
    ui_book_free();

    if (g_book_end_cb) {
        g_book_end_cb();
    }
}

static void btn_event_cb(lv_event_t *event)
{
    BookInfo* bookInfo = (BookInfo*)lv_event_get_user_data(event);

    ui_book_free();

    ui_book_read_start(book_read_end_cb, bookInfo);
}

static esp_err_t read_book_name(const char *file_name, char *book_name)
{
    char file_name_with_path[128];
    sprintf(file_name_with_path, "/spiffs/%s", file_name);
    FILE *fp = fopen(file_name_with_path, "r");

    if (NULL == fp) {
        ESP_LOGE(TAG, "Failed to open file %s", file_name_with_path);
        return ESP_FAIL;
    }

    char *ret = fgets(book_name, BOOK_NAME_MAX_LEN, fp);
    if (ret == NULL || ret == EOF) {
        ESP_LOGE(TAG, "Failed to read file %s", file_name_with_path);
        return ESP_FAIL;
    }
    book_name[strlen(book_name) - 1] = '\0';
    ESP_LOGI(TAG, "book name: %s", book_name);

    fclose(fp);

    return ESP_OK;
}

static lv_obj_t* render_book_list(lv_obj_t *parent)
{
    ESP_LOGI(TAG, "render_book_list()");

    g_book_list = (BookInfo *) malloc(sizeof(BookInfo) * MAX_BOOKS);
    g_book_count = 0;

    lv_obj_t *list = lv_list_create(parent);
    lv_obj_set_size(list, lv_obj_get_width(parent) - 30, lv_obj_get_height(parent) - 30);
    lv_obj_set_style_border_width(list, 0, LV_STATE_DEFAULT);
    lv_obj_align(list, LV_ALIGN_LEFT_MID, 0, 30);
    LV_FONT_DECLARE(ESPANDORA_MAIN_FONT);
    lv_obj_set_style_text_font(list, &ESPANDORA_MAIN_FONT, LV_PART_ITEMS);

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

            g_book_list[g_book_count].name[0] = '\0';
            g_book_list[g_book_count].path[0] = '\0';
            strcpy(g_book_list[g_book_count].path, p_dirent->d_name);
            read_book_name(p_dirent->d_name, &g_book_list[g_book_count].name);

            lv_obj_t *btn = lv_list_add_btn(list, LV_SYMBOL_FILE, &g_book_list[g_book_count].name);
            lv_obj_set_style_text_font(btn, &ESPANDORA_MAIN_FONT, LV_PART_MAIN);
            lv_group_add_obj(g_btn_op_group, btn);
            lv_obj_add_event_cb(btn, btn_event_cb, LV_EVENT_CLICKED, &g_book_list[g_book_count]);

            g_book_count++;
        } else {
            closedir(p_dir_stream);
            break;
        }
    }

    return list;
}

void ui_book_start(void (*fn)(void))
{
    ESP_LOGI(TAG, "ui_book_start()");

    if (fn != NULL) {
        g_book_end_cb = fn;
    }

    // PAGE
    g_page = lv_obj_create(lv_scr_act());
    lv_obj_set_size(g_page, 290, 190);
    lv_obj_clear_flag(g_page, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_radius(g_page, 15, LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(g_page, 0, LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(g_page, 20, LV_PART_MAIN);
    lv_obj_set_style_shadow_opa(g_page, LV_OPA_30, LV_PART_MAIN);
    lv_obj_align(g_page, LV_ALIGN_TOP_MID, 0, 40);

    // RETURN BUTTON
    lv_obj_t *btn_return = lv_btn_create(g_page);
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
    lv_obj_add_event_cb(btn_return, ui_book_page_return_click_cb, LV_EVENT_CLICKED, g_page);

    if (ui_get_btn_op_group()) {
        lv_group_add_obj(ui_get_btn_op_group(), btn_return);
    }

    lv_obj_t* list = render_book_list(g_page);
}

