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
#include "app_sensor.h"
#include "mqtt.h"
#include "http_server.h"

static const char *TAG = "main";

file_iterator_instance_t *file_iterator;

extern char g_client_id[32];

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

#define ESP_INTR_FLAG_DEFAULT 0
static QueueHandle_t gpio_evt_queue = NULL;

static void IRAM_ATTR gpio_isr_handler(void* arg)
{
    uint32_t gpio_num = (uint32_t) arg;
    xQueueSendFromISR(gpio_evt_queue, &gpio_num, NULL);
}
static void gpio_task_example(void* arg)
{
    uint32_t io_num;
    char ent_id[34] = {0};
    for(;;) {
        if(xQueueReceive(gpio_evt_queue, &io_num, portMAX_DELAY)) {
            printf("GPIO[%"PRIu32"] intr, val: %d\n", io_num, gpio_get_level(io_num));

            if (io_num == MOTION_GPIO_PIN) {
                ha_entity_id(ent_id, g_client_id, "motion");
                if (gpio_get_level(io_num)) {
                    ESP_LOGI(TAG, "Motion detected");
                    ha_state_push(ent_id, "ON");
                } else {
                    ESP_LOGI(TAG, "Motion stopped");
                    ha_state_push(ent_id, "OFF");
                }
            }
        }
    }
}

void gpio_init(void)
{
    //zero-initialize the config structure.
    gpio_config_t io_conf = {};
    //disable interrupt
    io_conf.intr_type = GPIO_INTR_DISABLE;
    //set as output mode
    io_conf.mode = GPIO_MODE_OUTPUT;
    //bit mask of the pins that you want to set,e.g.GPIO18/19
    io_conf.pin_bit_mask = (1ULL<<SWITCH_GPIO_PIN);
    //disable pull-down mode
    io_conf.pull_down_en = 0;
    //disable pull-up mode
    io_conf.pull_up_en = 0;
    //configure GPIO with the given settings
    gpio_config(&io_conf);


    //disable interrupt
    io_conf.intr_type = GPIO_INTR_ANYEDGE;
    //set as input mode
    io_conf.mode = GPIO_MODE_INPUT;
    //bit mask of the pins that you want to set,e.g.GPIO18/19
    io_conf.pin_bit_mask = (1ULL<<MOTION_GPIO_PIN);
    //enable pull-down mode
    io_conf.pull_down_en = 1;
    //disable pull-up mode
    io_conf.pull_up_en = 0;
    gpio_config(&io_conf);


    //create a queue to handle gpio event from isr
    gpio_evt_queue = xQueueCreate(10, sizeof(uint32_t));
    //start gpio task
    xTaskCreate(gpio_task_example, "gpio_task_example", 2048, NULL, 10, NULL);

    //install gpio isr service
    gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
    //hook isr handler for specific gpio pin
    gpio_isr_handler_add(MOTION_GPIO_PIN, gpio_isr_handler, (void*) MOTION_GPIO_PIN);
}

void app_main(void)
{
    esp_log_level_set("ui_main", ESP_LOG_VERBOSE);
    esp_log_level_set("wifi_init", ESP_LOG_VERBOSE);
    // esp_log_level_set("http", ESP_LOG_VERBOSE);

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
    err = bsp_spiffs_mount();
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to mount SPIFFS (%s)", esp_err_to_name(err));
    } else {
        ESP_LOGI(TAG, "SPIFFS mounted successfully");
    }

    bsp_i2c_init();
    bsp_display_start();
    bsp_board_init();

    gpio_init();

    TraverseDir("/spiffs", 0, 1);

    ESP_LOGI(TAG, "GUI start");
    bsp_display_backlight_on();
    ESP_ERROR_CHECK(ui_main_start());

    ESP_LOGI(TAG, "speech recognition start");
    app_sr_start(false);

    BaseType_t ret_val = xTaskCreatePinnedToCore(wifi_task, "Wifi Task", 6 * 1024, NULL, 1, NULL, 0);
    ESP_ERROR_CHECK_WITHOUT_ABORT((pdPASS == ret_val) ? ESP_OK : ESP_FAIL);

    xTaskCreate(mqtt_task, "mqtt_task", 4096, NULL, 3, NULL);
    xTaskCreate(http_server_init, "http_server_task", 4096, NULL, 3, NULL);
    xTaskCreate(app_sensor_task, "app_sensor_task", 4096, NULL, 3, NULL);
}
