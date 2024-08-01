#include <sys/cdefs.h>
//
// Created by meshi on 24-7-27.
//

#include <esp_log.h>
#include <esp_check.h>
#include <math.h>
#include "app_sensor.h"

#include "aht.h"
#include "bmp280.h"
#include "mqtt.h"
#define I2C_SDA GPIO_NUM_11
#define I2C_SCL GPIO_NUM_10
#define STANDARD_PRESSURE 101325  // 标准大气压（单位：帕斯卡）
#define EARTH_RADIUS 6371000  // 地球平均半径（单位：米）
#define CONST_PF 0.1902630958	//(1/5.25588f) Pressure factor
#define FIX_TEMP 25				// Fixed Temperature. ASL is a function of pressure and temperature, but as the temperature changes so much (blow a little towards the flie and watch it drop 5 degrees) it corrupts the ASL estimates.
// TLDR: Adjusting for temp changes does more harm than good.


static const char *TAG = "APP";

extern char g_client_id[32];
rht_report_data_t g_rht_data = {0};

/**
 * Converts pressure to altitude above sea level (ASL) in meters
 */
float BMP280PressureToAltitude(float* pressure/*, float* groundPressure, float* groundTemp*/)
{
    if (*pressure > 0)
    {
        return ((pow((STANDARD_PRESSURE / *pressure), CONST_PF) - 1.0f) * (FIX_TEMP + 273.15f)) / 0.0065f;
    }
    else
    {
        return 0;
    }
}


//static aht20_i2c_config_t i2c_conf = {
//        .i2c_port = I2C_NUM_0,
//        .i2c_addr = AHT20_ADDRRES_0
//};
//
//static aht20_dev_handle_t aht20_dev = {0};
static bmp280_t bmp_dev = {0};
static bmp280_params_t params = {0};
static aht_t aht_dev = {0};

esp_err_t app_sensor_init(void)
{
    esp_err_t ret = ESP_OK;
    ret = i2cdev_init();
    ESP_RETURN_ON_ERROR(ret, TAG, "i2cdev_init failed");

    aht_dev.type = AHT_TYPE_AHT20;

    bmp_dev.id = BMP280_CHIP_ID;
    bmp280_init_default_params(&params);

    return ret;
}

_Noreturn void app_sensor_task() {
    esp_err_t ret;
    ret = app_sensor_init();

    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Could not initialize sensors");
        return;
    }

    uint32_t raw_data;
    float bmp_temp, bmp_hum;
    char *json = NULL;

    bool bmp280_init_done = false;

    while (true) {
        ret = bmp280_init_desc(&bmp_dev, BMP280_I2C_ADDRESS_1, I2C_NUM_0, I2C_SDA, I2C_SCL);
        if (ret != ESP_OK) {
            ESP_LOGE(TAG, "Could not initialize BMP280 desc");
            return;
        }

        if (bmp280_init_done == false) {
            ret = bmp280_init(&bmp_dev, &params);
            if (ret != ESP_OK) {
                ESP_LOGE(TAG, "Could not initialize BMP280");
                return;
            }
            bmp280_init_done = true;
        }

        ret = bmp280_read_float(&bmp_dev, &bmp_temp, &g_rht_data.pressure, &bmp_hum);
        if (ret == ESP_OK) {
            ESP_LOGI("BMP280", "temperature: %.2f, humidity: %.2f, pressure: %.4f, altitude: %.1f",
                     bmp_temp, bmp_hum, g_rht_data.pressure, BMP280PressureToAltitude(&g_rht_data.pressure));
        } else {
            ESP_LOGE(TAG, "Could not read g_rht_data from BMP280");
        }
        bmp280_free_desc(&bmp_dev);

        ret = aht_init_desc(&aht_dev, AHT_I2C_ADDRESS_GND, I2C_NUM_0, I2C_SDA, I2C_SCL);
        if (ret != ESP_OK) {
            ESP_LOGE(TAG, "Could not initialize AHT20 desc");
        }
        ret = aht_init(&aht_dev);
        if (ret != ESP_OK) {
            ESP_LOGE(TAG, "Could not initialize AHT20");
        }
        vTaskDelay(100 / portTICK_PERIOD_MS);
        ret = aht_get_data(&aht_dev, &g_rht_data.temperature, &g_rht_data.humidity);
        ESP_LOGI("AHT20", "temperature: %.2f, humidity: %.2f", g_rht_data.temperature, g_rht_data.humidity);
        aht_free_desc(&aht_dev);

        char topic[64] = {0};

        sprintf(topic, "homeassistant/sensor/%s/state", g_client_id);

        char *fmt = "{"
                    "\"temperature\": %.2f"
                    ",\"humidity\": %.2f"
                    ",\"pressure\": %.1f"
                    "}";

        json = calloc(1, 512);
        sprintf(json, fmt, g_rht_data.temperature, g_rht_data.humidity, g_rht_data.pressure);
        mqtt_publish(topic, json, 1);
        free(json);
        vTaskDelay(15000 / portTICK_PERIOD_MS);
    }
}