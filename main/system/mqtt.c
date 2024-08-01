//
// Created by Hessian on 2023/7/29.
//
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <esp_tls.h>
#include <esp_mac.h>
#include "esp_system.h"
#include "esp_netif.h"
#include "driver/gpio.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_log.h"
#include "mqtt_client.h"
#include "settings.h"
#include "app_wifi.h"
#include "mqtt.h"

#define MQTT_TOPIC_PREFIX "homeassistant/"

static const char *TAG = "MQTT";

char g_client_id[32] = {0};
static esp_mqtt_client_handle_t g_client;

extern const uint8_t server_root_cert_pem_start[] asm("_binary_server_root_cert_pem_start");
extern const uint8_t server_root_cert_pem_end[]   asm("_binary_server_root_cert_pem_end");

typedef struct {
    char *id;
    char *name;
} ha_sensor_dev_t;

typedef struct {
    ha_sensor_dev_t *dev;
    char *configTopic;
    char *integration;
    char *deviceClass;
    char *name;
    char *unit;
    char *valueName;
} ha_sensor_ent_t;

static void ha_device_config_push(char *dev_id);
static void ha_entity_config_push(ha_sensor_ent_t *ent);

static esp_err_t mqtt_event_handler_cb(esp_mqtt_event_handle_t event)
{
    esp_mqtt_client_handle_t client = event->client;

    char ent_id[34] = {0};
    char topic[128] = {0};
    int msg_id;
    // your_context_t *context = event->context;
    switch (event->event_id) {
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");


            ha_device_config_push(g_client_id);

            // subcribe command topic
            ha_entity_id(ent_id, g_client_id, "switch");
            ha_dev_topic(topic, ent_id, "cmd");
            msg_id = esp_mqtt_client_subscribe(client, topic, 0);
            ESP_LOGI(TAG, "subscribe %s successful, msg_id=%d", topic, msg_id);
            // reset switch state
            gpio_get_level(SWITCH_GPIO_PIN) ? ha_state_push(ent_id, "ON") : ha_state_push(ent_id, "OFF");

            ha_entity_id(ent_id, g_client_id, "motion");
            // reset motion state
            gpio_get_level(MOTION_GPIO_PIN) ? ha_state_push(ent_id, "ON") : ha_state_push(ent_id, "OFF");

            break;

        case MQTT_EVENT_DISCONNECTED:
            ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
            break;

        case MQTT_EVENT_SUBSCRIBED:
            ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
//            msg_id = esp_mqtt_client_publish(client, "/topic/qos0", "data", 0, 0, 0);
//            ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg_id);
            break;

        case MQTT_EVENT_UNSUBSCRIBED:
            ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
            break;

        case MQTT_EVENT_PUBLISHED:
            ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
            break;

        case MQTT_EVENT_DATA:
            ESP_LOGI(TAG, "MQTT_EVENT_DATA");
            ESP_LOGI(TAG, "Receive [%.*s] DATA: %.*s", event->topic_len, event->topic, event->data_len, event->data);

            mqtt_handle_cmd(event->data, event->data_len);

            break;

        case MQTT_EVENT_ERROR:
            ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
            break;

        default:
            ESP_LOGI(TAG, "Other event id:%d", event->event_id);
            break;
    }

    return ESP_OK;
}

static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data) {
    ESP_LOGI(TAG, "Event dispatched from event loop base=%s, event_id=%d", base, event_id);
    mqtt_event_handler_cb(event_data);
}

void mqtt_client_id(char *id_string)
{
    id_string[0] = '\0';
    uint8_t mac[6];
    esp_read_mac(mac, ESP_MAC_WIFI_STA);
    sprintf(id_string, "espandora-%02x%02X%02X%02x%02X%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
}

int ha_entity_id(char *ent_id, char *dev_id, char *entity_name) {
    ent_id[0] = '\0';
    return sprintf(ent_id, "%s-%s", dev_id, entity_name);
}

int ha_dev_topic(char* topic, char *ent_id, char* suffix) {
    topic[0] = '\0';
    return sprintf(topic, MQTT_TOPIC_PREFIX "%s/%s", ent_id, suffix);
}

void mqtt_handle_cmd(char *payload, int len) {
    if (len == 0) {
        return;
    }

    char ent_id[64] = {0};
    ha_entity_id(ent_id, g_client_id, "switch");

    if (strncmp(payload, "ON", len) == 0) {
        gpio_set_level(SWITCH_GPIO_PIN, 1);
        ha_state_push(ent_id, "ON");
    } else if (strncmp(payload, "OFF", len) == 0) {
        gpio_set_level(SWITCH_GPIO_PIN, 0);
        ha_state_push(ent_id, "OFF");
    } else {
        ESP_LOGW(TAG, "unknown cmd: %.*s", len, payload);
    }
}

void mqtt_task(void *pvParameters)
{
    sys_param_t *settings = settings_get_parameter();

    if (strlen(settings->mqtt_url) == 0) {
        ESP_LOGE(TAG, "mqtt_url is empty, mqtt client will not start");
        vTaskDelete(NULL);
        return;
    }

    // 生成 clieng_id
    mqtt_client_id(g_client_id);

#if CONFIG_IDF_TARGET_ESP8266
    esp_mqtt_client_config_t mqtt_cfg = {
        .client_id = g_client_id,
        .uri = settings->mqtt_url,
        .cert_pem = (char *) &server_root_cert_pem_start,
        .cert_len = server_root_cert_pem_end - server_root_cert_pem_start,
        .skip_cert_common_name_check = true,
    };

    if (strlen(settings->mqtt_username) > 0) {
        mqtt_cfg.username = settings->mqtt_username;
    }

    if (strlen(settings->mqtt_password) > 0) {
        mqtt_cfg.password = settings->mqtt_password;
    }
#else
    esp_mqtt_client_config_t mqtt_cfg = {
            .broker = {
                    .address.uri = settings->mqtt_url,
//                    .verification.certificate = (const char *) server_root_cert_pem_start
            },
            .credentials = {
                    .client_id = g_client_id,
            }
    };

    if (strlen(settings->mqtt_username) > 0) {
        mqtt_cfg.credentials.username = settings->mqtt_username;
    }

    if (strlen(settings->mqtt_password) > 0) {
        mqtt_cfg.credentials.authentication.password = settings->mqtt_password;
    }
#endif

    ESP_LOGI(TAG, "check wifi connected");
    while (!app_wifi_is_connected()) {
        vTaskDelay(pdTICKS_TO_MS(1000));
    }
    ESP_LOGI(TAG, "wifi ok, start mqtt_client");

    ESP_LOGI(TAG, "mqtt_cfg.uri: %s", settings->mqtt_url);
    ESP_LOGI(TAG, "mqtt_cfg.username: %s", settings->mqtt_username);
    ESP_LOGI(TAG, "mqtt_cfg.password: %s", settings->mqtt_password);

    g_client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(g_client, ESP_EVENT_ANY_ID, mqtt_event_handler, g_client);
    esp_mqtt_client_start(g_client);
    ESP_LOGI(TAG, "mqtt_client started");

    vTaskDelete(NULL);
}

void mqtt_publish(char *topic, char* content, int retain)
{
    ESP_LOGI(TAG, "mqtt_publish: %s, %s", topic, content);
    esp_mqtt_client_publish(g_client, topic, content, 0, 1, retain);
}

// Home Assistant


/**
 * 注册传感器实体
 * @param ent
 */
static void ha_entity_config_push(ha_sensor_ent_t *ent)
{
    char ent_id[64] = {0};
    char config_topic[128] = {0};
    char state_topic[128] = {0};
    char cmd_topic[128] = {0};

    ha_entity_id(ent_id, ent->dev->id, ent->valueName);
    sprintf(config_topic, "homeassistant/%s/%s/config", ent->integration, ent_id);

    ha_dev_topic(cmd_topic, ent_id, "cmd");
    ha_dev_topic(state_topic, ent_id, "state");

    char *json = calloc(1, 512);
    json[0] = '\0';

    if (strncmp(ent->integration, "switch", 6) == 0) {
        const char *fmt = "{"
                          "\"name\": \"%s\""
                          ",\"device_class\": \"%s\""
                          ",\"state_topic\": \"%s\""
                          ",\"command_topic\": \"%s\""
                          ",\"payload_on\": \"ON\""
                          ",\"payload_off\": \"OFF\""
                          ",\"unique_id\": \"%s\""
                          ",\"device\": {\"identifiers\": [\"%s\"], \"name\": \"%s\" }"
                          "}";
        sprintf(json, fmt, ent->name, ent->deviceClass, state_topic, cmd_topic, ent_id, ent->dev->id, ent->dev->name);
    } else if (strncmp(ent->integration, "sensor", 6) == 0) {
        char *fmt = "{"
                    "\"name\": \"%s\""
                    ",\"device_class\": \"%s\""
                    ",\"state_topic\": \"homeassistant/sensor/%s/state\""
                    ",\"unit_of_measurement\": \"%s\""
                    ",\"value_template\": \"{{value_json.%s}}\""
                    ",\"unique_id\": \"%s\""
                    ",\"device\": {\"identifiers\": [\"%s\"], \"name\": \"%s\" }"
                    "}";
        sprintf(json, fmt, ent->name, ent->deviceClass, ent->dev->id, ent->unit, ent->valueName, ent_id, ent->dev->id, ent->dev->name);
    } else {
        const char *fmt = "{"
                          "\"name\": \"%s\""
                          ",\"device_class\": \"%s\""
                          ",\"state_topic\": \"%s\""
                          ",\"unique_id\": \"%s\""
                          ",\"device\": {\"identifiers\": [\"%s\"], \"name\": \"%s\" }"
                          "}";
        sprintf(json, fmt, ent->name, ent->deviceClass, state_topic, ent_id, ent->dev->id, ent->dev->name);
    }

    // Retain: The -r switch is added to retain the configuration config_topic in the broker. Without this, the sensor will not be available after Home Assistant restarts.
    mqtt_publish(config_topic, json, 1);
    free(json);
}

/**
 * 注册传感器设备
 * @param dev_id
 */
static void ha_device_config_push(char *dev_id)
{
    char name[64] = "Espandora";
    ha_sensor_dev_t dev = {
            .id = dev_id,
            .name = name
    };

    ha_sensor_ent_t ent = {
            .dev = &dev,
            .integration = "switch",
            .deviceClass = "switch",
            .name = "音箱",
            .valueName = "switch",
    };
    ha_entity_config_push(&ent);

    ent.integration = "binary_sensor";
    ent.deviceClass = "motion";
    ent.name = "人体感应";
    ent.valueName = "motion";
    ha_entity_config_push(&ent);

    ent.integration = "sensor";
    ent.deviceClass = "temperature";
    ent.name = "气温";
    ent.unit = "℃";
    ent.valueName = "temperature";
    ha_entity_config_push(&ent);

    // 空气湿度
    ent.integration = "sensor";
    ent.deviceClass = "humidity";
    ent.name = "空气湿度";
    ent.unit = "%";
    ent.valueName = "humidity";
    ha_entity_config_push(&ent);

    // 大气压
    ent.integration = "sensor";
    ent.deviceClass = "atmospheric_pressure";
    ent.name = "大气压";
    ent.unit = "Pa";
    ent.valueName = "pressure";
    ha_entity_config_push(&ent);
}

/**
 * 推送state
 * @param ent_id
 * @param report
 */
void ha_state_push(char *ent_id, char *state)
{
    char topic[128] = {0};
    ha_dev_topic(topic, ent_id, "state");

    mqtt_publish(topic, state, 0);
}