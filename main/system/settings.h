//
// Created by Hessian on 2023/7/29.
//

#pragma once
#ifndef ESP_MENJIN_SETTINGS_H
#define ESP_MENJIN_SETTINGS_H

#define MOTION_GPIO_PIN GPIO_NUM_38
#define SWITCH_GPIO_PIN GPIO_NUM_42

#include "esp_err.h"
#include "app_sr.h"

typedef struct {
    sr_language_t sr_lang;
    uint8_t volume; // 0 - 100%
    char mqtt_url[64];
    char mqtt_username[32];
    char mqtt_password[64];
    char menjin_id[64];
} sys_param_t;

esp_err_t settings_read_parameter_from_nvs(void);
esp_err_t settings_write_parameter_to_nvs(void);
sys_param_t *settings_get_parameter(void);
sys_param_t settings_get_default_parameter(void);
void settings_dump(void);

#endif