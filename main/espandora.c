#include <stdio.h>
#include <esp_wifi.h>
#include "esp_log.h"
#include "nvs_flash.h"
#include "nvs.h"

#include "bsp_board.h"
#include "bsp/esp-bsp.h"

#include "settings.h"
#include "gui/ui_main.h"
#include "file_manager.h"
#include "app_wifi.h"
#include "file_iterator.h"
#include "app_sr_handler.h"
#include "app_sr.h"
#include "mqtt.h"
#include "http_server.h"

static const char *TAG = "main";

file_iterator_instance_t *file_iterator;


static void wifi_credential_reset(void *handle, void *arg)
{
    ESP_LOGW(TAG, "WiFi credential reset");

    esp_wifi_restore();
    esp_restart();
}

void wifi_task(void *args)
{
    bsp_btn_register_callback(BOARD_BTN_ID_BOOT, BUTTON_LONG_PRESS_START, wifi_credential_reset, NULL);
    /* Initialize Wi-Fi.
     */
    app_wifi_init();

    /* Start the Wi-Fi. */
    esp_err_t err = app_wifi_start();
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Could not start Wifi");
    }

    vTaskDelete(NULL);
}


void app_main(void)
{
    esp_log_level_set("ui_main", ESP_LOG_VERBOSE);
//    esp_log_level_set("http", ESP_LOG_VERBOSE);

    ESP_LOGI(TAG, "Compile time: %s %s", __DATE__, __TIME__);
    /* Initialize NVS. */
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);
    ESP_ERROR_CHECK(settings_read_parameter_from_nvs());
#if !SR_RUN_TEST && MEMORY_MONITOR
    sys_monitor_start(); // Logs should be reduced during SR testing
#endif
    bsp_spiffs_mount();

    bsp_i2c_init();
    bsp_display_start();
    bsp_board_init();

//    TraverseDir("/spiffs", 0, 1);

    ESP_LOGI(TAG, "GUI start");
    bsp_display_brightness_set(75);
    ESP_ERROR_CHECK(ui_main_start());

    ESP_LOGI(TAG, "speech recognition start");
    app_sr_start(false);

    BaseType_t ret_val = xTaskCreatePinnedToCore(wifi_task, "Wifi Task", 6 * 1024, NULL, 1, NULL, 0);
    ESP_ERROR_CHECK_WITHOUT_ABORT((pdPASS == ret_val) ? ESP_OK : ESP_FAIL);

    xTaskCreate(mqtt_task, "mqtt_task", 4096, NULL, 3, NULL);
    xTaskCreate(http_server_init, "http_server_task", 4096, NULL, 3, NULL);
}
