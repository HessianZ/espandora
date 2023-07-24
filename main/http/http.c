#include <esp_types.h>
#include <string.h>
#include <inttypes.h>
#include <lwip/netdb.h>
#include <esp_http_client.h>
#include <sys/param.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "esp_tls.h"
#include "sdkconfig.h"
#include "esp_crt_bundle.h"
#include "http.h"
#include "json_parser.h"
#include "util.h"

/* Constants that aren't configurable in menuconfig */
#define WEB_PORT "443"
#define BILIBILI_FANS_URL "https://api.bilibili.com/x/relation/stat?vmid=10442962&jsonp=jsonp"

// 101280401 = 梅州
#define CITY_CODE "101280401"
#define WEATHER_URL "http://d1.weather.com.cn/weather_index/" CITY_CODE ".html"
#define WEATHER_REFERER "http://www.weather.com.cn/"

#define MAX_HTTP_RECV_BUFFER 512
#define MAX_HTTP_OUTPUT_BUFFER 5192
#define SERVER_URL_MAX_SZ 256
#define RESPONSE_BUF_SIZE 1024

static const char *TAG = "http";

static int g_fans = -1;
static int64_t g_fans_update_time = 0;

#define TIME_PERIOD (120 * 1000000L)

#define USER_AGENT "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/114.0.0.0 Safari/537.36 Edg/114.0.1823.37"
static const char REQUEST[] = "GET " BILIBILI_FANS_URL " HTTP/1.1\r\n"
                                        "Host: api.bilibili.com\r\n"
                                        "User-Agent: " USER_AGENT "\r\n"
                                        "\r\n";
static const char WEATHER_REQUEST[] = "GET " WEATHER_URL " HTTP/1.1\r\n"
                                        "Host: d1.weather.com.cn\r\n"
                                        "Referer: http://www.weather.com.cn/\r\n"
                                        "User-Agent: " USER_AGENT "\r\n"
                                        "\r\n";

typedef struct {
    char *name;
    char *value;
} http_header_t;

static void https_get_request(char *response)
{
    esp_tls_cfg_t cfg = {
            .crt_bundle_attach = esp_crt_bundle_attach,
    };

    int ret, len;

    esp_tls_t *tls = esp_tls_init();
    if (!tls) {
        ESP_LOGE(TAG, "Failed to allocate esp_tls handle!");
        goto exit;
    }

    if (esp_tls_conn_http_new_sync(BILIBILI_FANS_URL, &cfg, tls) == 1) {
        ESP_LOGI(TAG, "Connection established...");
    } else {
        ESP_LOGE(TAG, "Connection failed...");
        goto cleanup;
    }

    size_t written_bytes = 0;
    do {
        ret = esp_tls_conn_write(tls,
                                 REQUEST + written_bytes,
                                 strlen(REQUEST) - written_bytes);
        if (ret >= 0) {
            ESP_LOGI(TAG, "%d bytes written", ret);
            written_bytes += ret;
        } else if (ret != ESP_TLS_ERR_SSL_WANT_READ  && ret != ESP_TLS_ERR_SSL_WANT_WRITE) {
            ESP_LOGE(TAG, "esp_tls_conn_write  returned: [0x%02X](%s)", ret, esp_err_to_name(ret));
            goto cleanup;
        }
    } while (written_bytes < strlen(REQUEST));

    ESP_LOGI(TAG, "Reading HTTP response...");
    char buf[256];
    do {
        len = sizeof(buf) - 1;
        memset(buf, 0x00, sizeof(buf));
        ret = esp_tls_conn_read(tls, (char *)buf, len);

        if (ret == ESP_TLS_ERR_SSL_WANT_WRITE  || ret == ESP_TLS_ERR_SSL_WANT_READ) {
            ESP_LOGI(TAG, "ret: %d", ret);
            continue;
        } else if (ret < 0) {
            ESP_LOGE(TAG, "esp_tls_conn_read  returned [-0x%02X](%s)", -ret, esp_err_to_name(ret));
            break;
        } else if (ret == 0) {
            ESP_LOGI(TAG, "connection closed");
            break;
        }

        len = ret;
        ESP_LOGI(TAG, "%d bytes read", len);
        strcat(response, buf);
    } while (1);

    cleanup:
    esp_tls_conn_destroy(tls);
    exit:
    return;
}


esp_err_t _http_event_handler(esp_http_client_event_t *evt)
{
    static char *output_buffer;  // Buffer to store response of http request from event handler
    static int output_len;       // Stores number of bytes read
    switch(evt->event_id) {
        case HTTP_EVENT_ERROR:
            ESP_LOGD(TAG, "HTTP_EVENT_ERROR");
            break;
        case HTTP_EVENT_ON_CONNECTED:
            ESP_LOGD(TAG, "HTTP_EVENT_ON_CONNECTED");
            break;
        case HTTP_EVENT_HEADER_SENT:
            ESP_LOGD(TAG, "HTTP_EVENT_HEADER_SENT");
            break;
        case HTTP_EVENT_ON_HEADER:
            ESP_LOGD(TAG, "HTTP_EVENT_ON_HEADER, key=%s, value=%s", evt->header_key, evt->header_value);
            break;
        case HTTP_EVENT_ON_DATA:
            ESP_LOGD(TAG, "HTTP_EVENT_ON_DATA, len=%d", evt->data_len);
            /*
             *  Check for chunked encoding is added as the URL for chunked encoding used in this example returns binary data.
             *  However, event handler can also be used in case chunked encoding is used.
             */
            if (!esp_http_client_is_chunked_response(evt->client)) {
                // If user_data buffer is configured, copy the response into the buffer
                int copy_len = 0;
                if (evt->user_data) {
                    copy_len = MIN(evt->data_len, (MAX_HTTP_OUTPUT_BUFFER - output_len));
                    if (copy_len) {
                        memcpy(evt->user_data + output_len, evt->data, copy_len);
                    }
                } else {
                    const int buffer_len = esp_http_client_get_content_length(evt->client);
                    if (output_buffer == NULL) {
                        output_buffer = (char *) malloc(buffer_len);
                        output_len = 0;
                        if (output_buffer == NULL) {
                            ESP_LOGE(TAG, "Failed to allocate memory for output buffer");
                            return ESP_FAIL;
                        }
                    }
                    copy_len = MIN(evt->data_len, (buffer_len - output_len));
                    if (copy_len) {
                        memcpy(output_buffer + output_len, evt->data, copy_len);
                    }
                }
                output_len += copy_len;
            }

            break;
        case HTTP_EVENT_ON_FINISH:
            ESP_LOGD(TAG, "HTTP_EVENT_ON_FINISH");
            if (output_buffer != NULL) {
                // Response is accumulated in output_buffer. Uncomment the below line to print the accumulated response
                // ESP_LOG_BUFFER_HEX(TAG, output_buffer, output_len);
                free(output_buffer);
                output_buffer = NULL;
            }
            output_len = 0;
            break;
        case HTTP_EVENT_DISCONNECTED:
            ESP_LOGI(TAG, "HTTP_EVENT_DISCONNECTED");
            int mbedtls_err = 0;
            esp_err_t err = esp_tls_get_and_clear_last_error((esp_tls_error_handle_t)evt->data, &mbedtls_err, NULL);
            if (err != 0) {
                ESP_LOGI(TAG, "Last esp error code: 0x%x", err);
                ESP_LOGI(TAG, "Last mbedtls failure: 0x%x", mbedtls_err);
            }
            if (output_buffer != NULL) {
                free(output_buffer);
                output_buffer = NULL;
            }
            output_len = 0;
            break;
        case HTTP_EVENT_REDIRECT:
            ESP_LOGD(TAG, "HTTP_EVENT_REDIRECT");
            esp_http_client_set_header(evt->client, "From", "user@example.com");
            esp_http_client_set_header(evt->client, "Accept", "text/html");
            esp_http_client_set_redirection(evt->client);
            break;
    }
    return ESP_OK;
}

esp_err_t http_get_weather(char *result)
{
    ESP_LOGI(TAG, "Start http_get_weather ...");

    char *response = malloc(5192);
    esp_http_client_config_t config = {
            .url = WEATHER_URL,
            .event_handler = _http_event_handler,
            .crt_bundle_attach = esp_crt_bundle_attach,
            .user_data = response,
            .user_agent = USER_AGENT
    };
    esp_http_client_handle_t client = esp_http_client_init(&config);
    esp_http_client_set_header(client, "Referer", WEATHER_REFERER);
    esp_err_t err = esp_http_client_perform(client);

    if (err == ESP_OK) {
        ESP_LOGI(TAG, "HTTPS Status = %d, content_length = %"PRId64,
                 esp_http_client_get_status_code(client),
                 esp_http_client_get_content_length(client)
        );
        ESP_LOGI(TAG, "Response: %s", response);
    } else {
        ESP_LOGE(TAG, "Error perform http request %s", esp_err_to_name(err));
    }
    esp_http_client_cleanup(client);

    char *json = NULL;
    jparse_ctx_t *jctx = NULL;
    char* pStart = strstr(response, "var dataSK =");
    if (pStart == NULL) {
        ESP_LOGE(TAG, "Parse weather failed: %s", response);
        goto exception;
    }

    pStart += 12;
    char* pEnd = strstr(pStart, "};");
    pEnd += 1;
    int len = pEnd - pStart;
    json = malloc(len + 1);
    memcpy(json, pStart, len);
    json[len] = '\0';
    free(response);
    response = NULL;

    char city[10];
    char temp[5];
    char weather[12];
    char humi[5];
    char wind[12];
    char windSpeed[10];

    jctx = (jparse_ctx_t *)malloc(sizeof(jparse_ctx_t));
    int ret = json_parse_start(jctx, json, strlen(json));
    if (ret != OS_SUCCESS) {
        ESP_LOGE(TAG, "json_parse_start failed\n");
        goto exception;
    }

    if (json_obj_get_string(jctx, "cityname", &city, 10) != OS_SUCCESS) {
        ESP_LOGE(TAG, "json_obj_get_int failed: weather\n");
        goto exception;
    }
    if (json_obj_get_string(jctx, "temp", &temp, 5) != OS_SUCCESS) {
        ESP_LOGE(TAG, "json_obj_get_int failed: weather\n");
        goto exception;
    }
    if (json_obj_get_string(jctx, "SD", &humi, 5) != OS_SUCCESS) {
        ESP_LOGE(TAG, "json_obj_get_int failed: humi\n");
        goto exception;
    }
    if (json_obj_get_string(jctx, "weather", &weather, 12) != OS_SUCCESS) {
        ESP_LOGE(TAG, "json_obj_get_int failed: weather\n");
        goto exception;
    }
    if (json_obj_get_string(jctx, "WD", &wind, 12) != OS_SUCCESS) {
        ESP_LOGE(TAG, "json_obj_get_int failed: wind\n");
        goto exception;
    }
    if (json_obj_get_string(jctx, "WS", &windSpeed, 10) != OS_SUCCESS) {
        ESP_LOGE(TAG, "json_obj_get_int failed: windSpeed\n");
        goto exception;
    }
    strstr(humi, "%")[0] = '\0';
    strstr(windSpeed, "级")[0] = '\0';

    json_parse_end(jctx);
    free(json);
    json = NULL;

    char tempCn[12];
    char windSpeedCn[12];
    char humiCn[12];
    memset(tempCn, 0, 12);
    memset(windSpeedCn, 0, 12);
    memset(humiCn, 0, 12);
    num2cn(atoi(temp), &tempCn);
    num2cn(atoi(humi), &humiCn);
    num2cn(atoi(windSpeed), &windSpeedCn);

    sprintf(result, "%s %s 气温%s摄氏度湿度百分之%s %s%s级", city, weather, tempCn, humiCn, wind, windSpeedCn);
    ESP_LOGI(TAG, "weather text: %s", result);

    return ESP_OK;

    exception:
    ESP_LOGW(TAG, "http_get_weather exception");
    if (response != NULL) {
        free(response);
    }
    if (jctx != NULL) {
        json_parse_end(jctx);
    }
    if (json != NULL) {
        free(json);
    }

    return ESP_FAIL;
}

int http_get_bilibili_fans()
{
    int fans_num = -1;
    ESP_LOGI(TAG, "Start http_get_bilibili_fans");

    char *json = malloc(2048);
    esp_http_client_config_t config = {
            .url = BILIBILI_FANS_URL,
            .event_handler = _http_event_handler,
            .crt_bundle_attach = esp_crt_bundle_attach,
            .user_data = json,
            .user_agent = USER_AGENT
    };
    esp_http_client_handle_t client = esp_http_client_init(&config);
    esp_err_t err = esp_http_client_perform(client);

    if (err == ESP_OK) {
        ESP_LOGI(TAG, "HTTPS Status = %d, content_length = %"PRId64,
                 esp_http_client_get_status_code(client),
                 esp_http_client_get_content_length(client)
        );
        ESP_LOGI(TAG, "Response: %s", json);
    } else {
        ESP_LOGE(TAG, "Error perform http request %s", esp_err_to_name(err));
    }
    esp_http_client_cleanup(client);

    jparse_ctx_t *jctx = (jparse_ctx_t *)malloc(sizeof(jparse_ctx_t));
    int ret = json_parse_start(jctx, json, strlen(json));
    if (ret != OS_SUCCESS) {
        ESP_LOGE(TAG, "json_parse_start failed\n");
        return -1;
    }

    if (json_obj_get_object(jctx, "data")!= OS_SUCCESS) {
        ESP_LOGE(TAG, "json_obj_get_object failed: data\n");
        return -1;
    }

    if (json_obj_get_int(jctx, "follower", &fans_num) != OS_SUCCESS) {
        ESP_LOGE(TAG, "json_obj_get_int failed: follower\n");
        return -1;
    }

    ESP_LOGI(TAG, "follower: %d\n", fans_num);

    if (jctx != NULL) {
        json_parse_end(jctx);
    }
    if (json != NULL) {
        free(json);
    }

    ESP_LOGI(TAG, "BILIBILI_FANS: %d", fans_num);

    return fans_num;
}

static http_fans_change_cb fans_change_cb = NULL;

_Noreturn void http_bilibili_task(void *arg) {
    while (1) {
        g_fans_update_time = esp_timer_get_time();
        int fans = http_get_bilibili_fans();
        if (fans >= 0) {
            g_fans = fans;

            if (fans_change_cb != NULL) {
                fans_change_cb(fans);
            }
        }
        vTaskDelay(pdMS_TO_TICKS(120000));
    }
}

void http_bilibili_task_start() {
    ESP_LOGI(TAG, "Start http_bilibili_task");
    xTaskCreate(http_bilibili_task, "http bilibili task", 6 * 1024, NULL, 5, NULL);
}