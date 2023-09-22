//
// Created by Hessian on 2023/7/29.
//

#include <string.h>
#include "esp_log.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "settings.h"

static const char *TAG = "settings";

#define NAME_SPACE "sys_param"
#define KEY "param"

static sys_param_t g_sys_param = {0};

const sys_param_t g_default_sys_param = {
        .volume = 100,
        .mqtt_url = {0},
        .mqtt_username = {0},
        .mqtt_password = {0},
        .menjin_id = {0},
        .sr_lang = SR_LANG_CN,
};

esp_err_t settings_read_parameter_from_nvs(void)
{
    nvs_handle_t my_handle = 0;
    esp_err_t ret = nvs_open(NAME_SPACE, NVS_READONLY, &my_handle);
    if (ESP_ERR_NVS_NOT_FOUND == ret) {
        ESP_LOGW(TAG, "Not found, Set to default");
        memcpy(&g_sys_param, &g_default_sys_param, sizeof(sys_param_t));
        settings_write_parameter_to_nvs();
        return ESP_OK;
    }

    if (ESP_OK != ret) {
        ESP_LOGE(TAG, "nvs open failed (0x%x)", ret);
        goto err;
    }

    size_t len = sizeof(sys_param_t);
    ret = nvs_get_blob(my_handle, KEY, &g_sys_param, &len);
    if (ESP_OK != ret) {
        ESP_LOGE(TAG, "can't read param");
        goto err;
    }
    nvs_close(my_handle);

    return ret;
err:
    if (my_handle) {
        nvs_close(my_handle);
    }
    return ret;
}

esp_err_t settings_write_parameter_to_nvs(void)
{
    ESP_LOGI(TAG, "Saving settings");

    nvs_handle_t my_handle = {0};
    esp_err_t err = nvs_open(NAME_SPACE, NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        ESP_LOGI(TAG, "Error (%s) opening NVS handle!\n", esp_err_to_name(err));
    } else {
        err = nvs_set_blob(my_handle, KEY, &g_sys_param, sizeof(sys_param_t));
        err |= nvs_commit(my_handle);
        nvs_close(my_handle);
    }
    return ESP_OK == err ? ESP_OK : ESP_FAIL;
}

sys_param_t *settings_get_parameter(void)
{
    return &g_sys_param;
}

sys_param_t settings_get_default_parameter(void)
{
    return g_default_sys_param;
}

void settings_dump(void)
{
    ESP_LOGI(TAG, "settings_dump >>>");
    ESP_LOGI(TAG, "\tmqtt_url: %s", g_sys_param.mqtt_url);
    ESP_LOGI(TAG, "\tmqtt_username: %s", g_sys_param.mqtt_username);
    ESP_LOGI(TAG, "\tmqtt_password: %s", g_sys_param.mqtt_password);
}