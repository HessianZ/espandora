#include "esp_log.h"
#include "bsp_board.h"
#include "lvgl.h"
#include "ui_main.h"
#include "mqtt.h"
#include "ui_ring.h"

static const char *TAG = "ui_ring";

LV_FONT_DECLARE(ESPANDORA_MAIN_FONT);

static lv_obj_t *g_mask = NULL;
static lv_obj_t *g_ring_label = NULL;
static lv_group_t *g_btn_op_group_old = NULL;
static lv_group_t *g_btn_op_group = NULL;

static void ring_mask_event_handler(lv_event_t *event)
{
    bool active = (bool) event->param;

    lv_indev_t *indev = lv_indev_get_next(NULL);

    if (active) {
        // save old group
        g_btn_op_group_old = indev->group;

        lv_indev_set_group(indev, g_btn_op_group);
        lv_obj_clear_flag(g_mask, LV_OBJ_FLAG_HIDDEN);
        lv_obj_move_foreground(g_mask);
    } else {
        // restore group
        /* The second timer callback will hide sr mask */
        if (!lv_obj_has_flag(g_mask, LV_OBJ_FLAG_HIDDEN)) {
            lv_obj_add_flag(g_mask, LV_OBJ_FLAG_HIDDEN);
            lv_indev_set_group(indev, g_btn_op_group_old);
        }
    }
}


static void ring_label_event_handler(lv_event_t *event)
{
    char *text = (char *) event-> param;
    if (NULL != text) {
        lv_label_set_text_static(g_ring_label, text);
    }
}

static void ring_btn_event_handler(lv_event_t *event)
{
    switch (event->code) {
        case LV_EVENT_CLICKED:
//            mqtt_notify("open");
            ui_ring_hide();
            break;
    }
}

/**
 * 初始化闹铃提醒界面
 */
void ui_ring_init(void)
{
    ESP_LOGI(TAG, "sr animation initialize");
    g_mask = lv_obj_create(lv_scr_act());
    lv_obj_set_size(g_mask, lv_obj_get_width(lv_obj_get_parent(g_mask)), lv_obj_get_height(lv_obj_get_parent(g_mask)));
    lv_obj_clear_flag(g_mask, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_flag(g_mask, LV_OBJ_FLAG_HIDDEN);
    lv_obj_set_style_radius(g_mask, 0, LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(g_mask, 0, LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(g_mask, lv_obj_get_style_bg_color(lv_obj_get_parent(g_mask), LV_PART_MAIN), LV_STATE_DEFAULT);
    lv_obj_align(g_mask, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_event_cb(g_mask, ring_mask_event_handler, LV_EVENT_VALUE_CHANGED, NULL);

    lv_obj_t *obj_btn = NULL;
    obj_btn = lv_btn_create(g_mask);
    lv_obj_set_size(obj_btn, 80, 80);
    lv_obj_clear_flag(obj_btn, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_radius(obj_btn, 40, LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(obj_btn, 0, LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(obj_btn, 40, LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(obj_btn, LV_OPA_30, LV_STATE_DEFAULT);
    lv_obj_align(obj_btn, LV_ALIGN_CENTER, 0, -30);
    lv_obj_add_event_cb(obj_btn, ring_btn_event_handler, LV_EVENT_ALL, NULL);

    g_btn_op_group = lv_group_create();
    lv_group_add_obj(g_btn_op_group, obj_btn);

    lv_obj_t *obj_img = NULL;
    obj_img = lv_obj_create(obj_btn);
    lv_obj_set_size(obj_img, 80, 80);
    lv_obj_clear_flag(obj_img, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_radius(obj_img, 40, LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(obj_img, 0, LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(obj_img, 40, LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(obj_img, LV_OPA_30, LV_STATE_DEFAULT);
    lv_obj_align(obj_img, LV_ALIGN_CENTER, 0, -30);
    lv_obj_t *img_icon_ring = lv_img_create(obj_img);
    LV_IMG_DECLARE(icon_ring)
    lv_img_set_src(img_icon_ring, &icon_ring);
    lv_obj_center(img_icon_ring);
    lv_obj_center(obj_img);

    g_ring_label = lv_label_create(g_mask);
    lv_label_set_text_static(g_ring_label, "------------");
    lv_obj_set_style_text_font(g_ring_label, &ESPANDORA_MAIN_FONT, LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(g_ring_label, lv_color_black(), LV_STATE_DEFAULT);
    lv_obj_align(g_ring_label, LV_ALIGN_CENTER, 0, 80);
    lv_obj_add_event_cb(g_ring_label, ring_label_event_handler, LV_EVENT_VALUE_CHANGED, NULL);
}

void ui_ring_hide(void)
{
    lv_event_send(g_mask, LV_EVENT_VALUE_CHANGED, (void *) false);
}

static void ui_ring_show_timeout_cb(lv_timer_t *timer)
{
    ui_ring_hide();
}

void ui_ring_show()
{
    lv_event_send(g_mask, LV_EVENT_VALUE_CHANGED, (void *) true);

    lv_timer_create(ui_ring_show_timeout_cb, 30 * 1000, NULL);
}

void ui_ring_set_text(char *text)
{
    lv_event_send(g_ring_label, LV_EVENT_VALUE_CHANGED, (void *) text);
}
