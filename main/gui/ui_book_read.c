//
// Created by Hessian on 2023/7/25.
//

#include "esp_log.h"
#include "lvgl.h"
#include "app_sr.h"
#include "ui_main.h"
#include "file_manager.h"
#include <locale.h>

#include "ui_book_read.h"
#include "bsp_board.h"

#define PAGE_MAX_LINE 9
#define CHAR_IS_ASCII(value)              ((value & 0x80U) == 0x00U)
#define CHAR_IS_2BYTES_UTF8_CODE(value)   ((value & 0xE0U) == 0xC0U)
#define CHAR_IS_3BYTES_UTF8_CODE(value)   ((value & 0xF0U) == 0xE0U)
#define CHAR_IS_4BYTES_UTF8_CODE(value)   ((value & 0xF8U) == 0xF0U)
#define CHAR_IS_INVALID_UTF8_CODE(value)  ((value & 0xC0U) != 0x80U)

static const char *TAG = "ui_book_read";

static void (*g_book_read_end_cb)(void) = NULL;

static char* g_book_read_filename = NULL;
static fpos_t* g_book_read_file_cursor = NULL;
static FILE* g_book_read_file_handle = NULL;
static lv_obj_t * g_label_content = NULL;
static uint16_t g_page_max_length = 500;

static char *content = NULL;

LV_FONT_DECLARE(font_HarmonyOS_Sans_Light_16);

static void ui_book_read_page_return_click_cb(void *button_handle, void *usr_data)
{
    ESP_LOGI(TAG, "ui_book_read_page_return_click_cb ");

    lv_obj_t *obj = (lv_obj_t *)usr_data;

    // unregister button callback
    bsp_btn_register_callback(BOARD_BTN_ID_BOOT, BUTTON_SINGLE_CLICK, NULL, NULL);

    if (ui_get_btn_op_group()) {
        lv_group_remove_all_objs(ui_get_btn_op_group());
    }

    if (obj != NULL) {
        lv_obj_del(obj);
    }

    if (g_book_read_file_handle != NULL) {
        fclose(g_book_read_file_handle);
    }

    if (content != NULL) {
        free(content);
    }

    if (g_book_read_filename != NULL) {
        free(g_book_read_filename);
    }

    if (g_book_read_end_cb) {
        g_book_read_end_cb();
    }
}

static void ui_book_read_page_return_click_lvcb(lv_event_t *e)
{
    ui_book_read_page_return_click_cb(NULL, e->user_data);
}

static int read_cjk_char(FILE *fp, char *dest, uint8_t *len)
{
    int c = fgetc(fp);
    if (c == EOF) {
        return EOF;
    }

    dest[0] = (char)c;
    if (CHAR_IS_ASCII(c)) {
        *len = 1;
    } else if (CHAR_IS_4BYTES_UTF8_CODE(c)) {
        *len = 4;
    } else if (CHAR_IS_3BYTES_UTF8_CODE(c)) {
        *len = 3;
    } else if (CHAR_IS_2BYTES_UTF8_CODE(c)) {
        *len = 2;
    } else {
        *len = 0;
    }

    for (int i = 1; i < *len; ++i) {
        c = fgetc(fp);
        if (c == EOF) {
            dest[i] = '\0';
            return EOF;
        }
        dest[i] = (char)c;
    }

    if (*len < 4) {
        dest[*len] = '\0';
    }

    return 0;
}


static void render_next_page()
{
    ESP_LOGI(TAG, "render_next_page() ");

    size_t size = sizeof(char) * g_page_max_length;
    memset(content, 0, size);

    char line[128];
    line[0] = '\0';
    char c[4];
    uint8_t char_len = 0;
    uint8_t lines = 0;
    uint8_t line_chars = 0;
    uint8_t line_len = 0;
    uint16_t page_len = 0;

    lv_coord_t letter_space = lv_obj_get_style_text_letter_space(g_label_content, LV_PART_MAIN);

    while (1) {
        int ret = read_cjk_char(g_book_read_file_handle, &c, &char_len);
        if (ret == EOF) {
            ESP_LOGI(TAG, "read_cjk_char EOF");
            break;
        } else if (char_len == 0) {
            ESP_LOGI(TAG, "read_cjk_char char_len == 0");
            continue;
        }
//        ESP_LOGI(TAG, "read_cjk_char -- %s:%d", c, char_len);

        line_len += char_len;
        page_len += char_len;

        bool new_line = false;

        // 换行符
        if (char_len == 1 && *c == '\n') {
            strcat(&line, "\n");
            new_line = true;
        } else {
            strncat(&line, c, char_len);
            line_chars ++;
        }

        if (line_chars > 15) {
            lv_coord_t line_width = lv_txt_get_width(&line, line_len, &font_HarmonyOS_Sans_Light_16, letter_space, LV_TEXT_FLAG_NONE);

//            ESP_LOGI(TAG, "line_width : %d", line_width);

            if (line_width > 280) {
//                ESP_LOGI(TAG, "LINE BREAK: x overflow");
                strcat(&line, "\n");
                page_len++;
                line_len ++;
                new_line = true;
            }
        }

        if (new_line) {
            lines ++;
            line[line_len] = '\0';
//            ESP_LOGI(TAG, "LINE[%d:%d] : %s", line_len, line_chars, line);
            strcat(content, &line);
            line[0] = '\0';
            line_chars = 0;
            line_len = 0;
        }

        if (lines >= PAGE_MAX_LINE || page_len + 4 >= g_page_max_length) {
            break;
        }
    }

    if (line_len > 0) {
        line[line_len+1] = '\0';
        strcat(content, line);
    }

    fgetpos(g_book_read_file_handle, &g_book_read_file_cursor);
    ESP_LOGI(TAG, "PAGE CONTENT: %s", content);
    lv_label_set_text_static(g_label_content, content);
}

//void ui_book_read_page_next_click_cb(void *button_handle, void *usr_data)
void ui_book_read_page_next_click_cb(lv_event_t *e)
{
    if (e == NULL) {
        return;
    }

    ESP_LOGI(TAG, "ui_book_read_page_next_click_cb event: %d", e->code);

    if (e->code == LV_EVENT_KEY) {
        ESP_LOGI(TAG, "ui_book_read_page_next_click_cb key: %d", lv_indev_get_key(lv_indev_get_act()));
    }

    if (e->code == LV_EVENT_CLICKED) {
        render_next_page();
    }
}

/*
void ui_book_read_start(void (*fn)(void), const char* filename)
{
    ESP_LOGI(TAG, "ui_book_read_start file: %s", filename);

    g_book_read_end_cb = fn;

    g_book_read_filename = malloc(strlen(filename) + 1);
    strcpy(g_book_read_filename, filename);

    // PAGE
    lv_obj_t *page = lv_obj_create(lv_scr_act());
    lv_obj_set_size(page, 300, 190);
    lv_obj_clear_flag(page, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_align(page, LV_ALIGN_TOP_MID, 0, 40);

    // MAIN CONTENT
    g_label_content = lv_label_create(page);
    lv_obj_set_size(g_label_content, 290, 190);
    lv_label_set_text_static(g_label_content, "loading...");
    lv_obj_set_style_text_font(g_label_content, &font_HarmonyOS_Sans_Light_16, LV_PART_MAIN);
    lv_label_set_long_mode(g_label_content, LV_LABEL_LONG_CLIP);
    lv_obj_align(g_label_content, LV_ALIGN_TOP_MID, 0, 0);

    // RETURN BUTTON
    bsp_btn_register_callback(BOARD_BTN_ID_BOOT, BUTTON_SINGLE_CLICK, ui_book_read_page_return_click_cb, page);
}
*/


void ui_book_read_start(void (*fn)(void), const BookInfo *bookInfo)
{
    char *filename = (char *) malloc(128);

    if (NULL != filename) {
        /* Get full file name with mount point and folder path */
        strcpy(filename, "/spiffs/");
        strcat(filename, bookInfo->path);
    }

    ESP_LOGI(TAG, "ui_book_read_start file: %s", filename);

    g_book_read_end_cb = fn;

    g_book_read_filename = malloc(strlen(filename) + 1);
    strcpy(g_book_read_filename, filename);
    free(filename);

    lv_obj_t * win = lv_win_create(lv_scr_act(), 40);
    lv_obj_t * btn;
    btn = lv_win_add_btn(win, LV_SYMBOL_LEFT, 40);
    lv_obj_add_event_cb(btn, ui_book_read_page_next_click_cb, LV_EVENT_CLICKED, NULL);
    lv_group_add_obj(ui_get_btn_op_group(), btn);

    lv_obj_t *title_label = lv_win_add_title(win, "placeholder");
    lv_obj_set_style_text_font(title_label, &font_HarmonyOS_Sans_Light_16, LV_PART_MAIN);
    lv_label_set_text(title_label, bookInfo->name);

    btn = lv_win_add_btn(win, LV_SYMBOL_RIGHT, 40);
    lv_obj_add_event_cb(btn, ui_book_read_page_next_click_cb, LV_EVENT_CLICKED, NULL);
    lv_group_add_obj(ui_get_btn_op_group(), btn);

    btn = lv_win_add_btn(win, LV_SYMBOL_CLOSE, 60);
    lv_obj_add_event_cb(btn, ui_book_read_page_return_click_lvcb, LV_EVENT_CLICKED, win);
    lv_group_add_obj(ui_get_btn_op_group(), btn);

    lv_obj_t * cont = lv_win_get_content(win);  /*Content can be added here*/
    lv_obj_clear_flag(cont, LV_OBJ_FLAG_SCROLLABLE);

    // RETURN BUTTON
    bsp_btn_register_callback(BOARD_BTN_ID_BOOT, BUTTON_SINGLE_CLICK, ui_book_read_page_return_click_cb, win);


    // MAIN CONTENT
    g_label_content = lv_label_create(cont);
    lv_obj_set_size(g_label_content, 300, 190);
    lv_label_set_text_static(g_label_content, "loading...");
    lv_obj_set_style_text_font(g_label_content, &font_HarmonyOS_Sans_Light_16, LV_PART_MAIN);
    lv_label_set_long_mode(g_label_content, LV_LABEL_LONG_CLIP);
    lv_obj_align(g_label_content, LV_ALIGN_TOP_MID, 0, 0);

    size_t size = sizeof(char) * g_page_max_length;
    content = malloc(size);
    g_book_read_file_handle = fopen(g_book_read_filename, "r");
    if (g_book_read_file_handle == NULL) {
        ESP_LOGE(TAG, "Failed to open file for reading");
        lv_label_set_text_static(g_label_content, "文件读取失败");
    } else {
        render_next_page();
    }
}

