//
// Created by Hessian on 2023/7/29.
//
#include <stdlib.h>
#include <stdbool.h>

#include <esp_log.h>
#include <esp_system.h>
#include <esp_http_server.h>
#include <esp_event.h>
#include <cJSON.h>
#include <esp_check.h>

#include "http_server.h"
#include "json_parser.h"
#include "settings.h"
#include "mqtt.h"
#include "app_wifi.h"

#define HTML_BUF_SIZE 2048

static const char *TAG="HTTP_SERVER";

int pre_start_mem, post_stop_mem, post_stop_min_mem;

/********************* Basic Handlers Start *******************/
bool file_ext_cmp(const char *filename, const char *extension) {
    // 获取文件名中最后一个点的位置
    const char *dot = strrchr(filename, '.');
    if (dot && dot != filename) {
        // 比较扩展名是否匹配
        return strcmp(dot + 1, extension) == 0;
    }

    return false; // 没有扩展名或者不匹配
}

static esp_err_t send_file_response(httpd_req_t *req, char* filename)
{
    esp_err_t ret = ESP_OK;
    FILE *fp = NULL;
    char *buf = NULL;
    size_t read_len = 0;

    fp = fopen(filename, "rb");

    ESP_GOTO_ON_FALSE(fp != NULL, ESP_ERR_NOT_FOUND, err, TAG, "Failed to open file %s", filename);

    buf = calloc(HTML_BUF_SIZE, sizeof(char));

    if (NULL == buf) {
        ESP_LOGE(TAG, "Failed to allocate memory for buf");
        return ESP_ERR_NO_MEM;
    }

    if (file_ext_cmp(filename, "js")) {
        httpd_resp_set_type(req, "text/javascript");
    } else if (file_ext_cmp(filename, "css")) {
        httpd_resp_set_type(req, "text/css");
    } else {
        httpd_resp_set_type(req, "text/html");
    }

    while ((read_len = fread(buf, sizeof(char), HTML_BUF_SIZE, fp)) > 0) {
        httpd_resp_send_chunk(req, buf, read_len);
    }

    ESP_GOTO_ON_FALSE(feof(fp), ESP_FAIL, err, TAG, "Failed to read file %s error: %d", filename, ferror(fp));

    // chunks send finish
    httpd_resp_send_chunk(req, NULL, 0);

    err:
    if (fp != NULL) {
        fclose(fp);
    }
    if (buf != NULL) {
        free(buf);
    }

    if (ret == ESP_ERR_NOT_FOUND) {
        ret = httpd_resp_send_err(req, HTTPD_404_NOT_FOUND, "File does not exist");
    }

    return ret;
}

esp_err_t api_handler_get_index(httpd_req_t *req)
{
    // default response
    char index_filename[] = CONFIG_BSP_SPIFFS_MOUNT_POINT "/index.html";
    return send_file_response(req, index_filename);
}

esp_err_t api_handler_get_settings(httpd_req_t *req)
{
    ESP_LOGI(TAG, "GET /api/settings handler read content length %d", req->content_len);

    char*  buf = malloc(512);

    sys_param_t *settings = settings_get_parameter();

    int len = sprintf(buf, "{"
                 "\"sr_lang\": %d,"
                 "\"volume\": %d,"
                 "\"mqtt_url\": \"%s\","
                 "\"mqtt_username\": \"%s\","
                 "\"mqtt_password\": \"%s\","
                 "\"menjin_id\": \"%s\""
                 "}",
                settings->sr_lang,
                settings->volume,
                settings->mqtt_url,
                settings->mqtt_username,
                settings->mqtt_password,
                settings->menjin_id
     );

    httpd_resp_set_type(req, HTTPD_TYPE_JSON);
    httpd_resp_send(req, buf, len);
    free (buf);
    return ESP_OK;
}

esp_err_t api_handler_post_settings(httpd_req_t *req)
{
    ESP_LOGI(TAG, "POST /api/settings handler read content length %d", req->content_len);

    char*  buf = malloc(req->content_len + 1);
    size_t off = 0;
    int    ret;

    if (!buf) {
        httpd_resp_send_500(req);
        return ESP_FAIL;
    }

    while (off < req->content_len) {
        /* Read data received in the request */
        ret = httpd_req_recv(req, buf + off, req->content_len - off);
        if (ret <= 0) {
            if (ret == HTTPD_SOCK_ERR_TIMEOUT) {
                httpd_resp_send_408(req);
            }
            free (buf);
            return ESP_FAIL;
        }
        off += ret;
        ESP_LOGI(TAG, "/api/settings handler recv length %d", ret);
    }
    buf[off] = '\0';

    ESP_LOGI(TAG, "/api/settings handler read %s", buf);

    jparse_ctx_t *jctx = NULL;
    jctx = (jparse_ctx_t *)malloc(sizeof(jparse_ctx_t));
    ret = json_parse_start(jctx, buf, req->content_len);
    if (ret != OS_SUCCESS) {
        ESP_LOGE(TAG, "json_parse_start failed\n");
        httpd_resp_set_status(req, HTTPD_500);
        httpd_resp_send(req, "json parse failed", 18);
        return ESP_OK;
    }

    sys_param_t *settings = settings_get_parameter();
    char str_val[128];

    if (json_obj_get_string(jctx, "mqtt_url", (char *) &str_val, sizeof(settings->mqtt_url)) == OS_SUCCESS) {
        strcpy(settings->mqtt_url, str_val);
    }
    if (json_obj_get_string(jctx, "mqtt_url", (char *) &str_val, sizeof(settings->mqtt_url)) == OS_SUCCESS) {
        strcpy(settings->mqtt_url, str_val);
    }
    if (json_obj_get_string(jctx, "mqtt_username", (char *) &str_val, sizeof(settings->mqtt_username)) == OS_SUCCESS) {
        strcpy(settings->mqtt_username, str_val);
    }
    if (json_obj_get_string(jctx, "mqtt_password", (char *) &str_val, sizeof(settings->mqtt_password)) == OS_SUCCESS) {
        strcpy(settings->mqtt_password, str_val);
    }
    if (json_obj_get_string(jctx, "menjin_id", (char *) &str_val, sizeof(settings->menjin_id)) == OS_SUCCESS) {
        strcpy(settings->menjin_id, str_val);
    }
    int int_val;
    if (json_obj_get_int(jctx, "volume", &int_val) == OS_SUCCESS) {
        settings->volume = int_val;
    }
    if (json_obj_get_int(jctx, "sr_lang", &int_val) == OS_SUCCESS) {
        settings->sr_lang = int_val;
    }

    ESP_ERROR_CHECK(settings_write_parameter_to_nvs());

    settings_dump();

    httpd_resp_send(req, buf, req->content_len);
    json_parse_end(jctx);
    free (buf);
    return ESP_OK;
}

static void restart_task(void *arg)
{
    vTaskDelay(200 / portTICK_PERIOD_MS);
    esp_restart();
}

esp_err_t api_handler_restart(httpd_req_t *req)
{
    ESP_LOGW(TAG, "Restarting the device");

    httpd_resp_send(req, "ok", 2);
    xTaskCreate(restart_task, "restart_task", 2048, NULL, 5, NULL);
    return ESP_OK;
}

esp_err_t api_handler_info(httpd_req_t *req)
{
#define OK_STR "ESP32 MQTT Relay v1.0"
    ESP_LOGI(TAG, "/api/info handler read content length %d", req->content_len);

    httpd_resp_send(req, OK_STR, strlen(OK_STR));
    return ESP_OK;
#undef OK_STR
}

esp_err_t api_handler_menjin_cmd(httpd_req_t *req)
{
#define OK_STR "ok"
    ESP_LOGI(TAG, "/api/menjin/cmd handler read content length %d", req->content_len);

    char *resp_str = OK_STR;

    sys_param_t *settings = settings_get_parameter();

    // read cmd param from query string
    char buf[128];
    int ret = httpd_req_get_url_query_str(req, buf, sizeof(buf));
    if (ret == ESP_OK) {
        ESP_LOGI(TAG, "[api_handler_menjin_cmd] Found URL query => %s", buf);
        char param[16];
        if (httpd_query_key_value(buf, "cmd", param, sizeof(param)) == ESP_OK) {

            ESP_LOGD(TAG, "cmd: %s", param);

        } else {
            httpd_resp_set_status(req, "500 Server Internal Error");
            resp_str = "param 'cmd' not found";
        }
    } else {
        httpd_resp_set_status(req, "400 Bad Request");
        resp_str = "param 'cmd' not found";
        ESP_LOGW(TAG, "[api_handler_menjin_cmd] request menjin cmd without query string");
    }

    httpd_resp_send(req, resp_str, strlen(resp_str));

    return ESP_OK;
#undef OK_STR
}

// HTTP Error (404) Handler - Redirects all requests to the root page
esp_err_t http_404_error_handler(httpd_req_t *req, httpd_err_code_t err)
{
    char filename[HTTPD_MAX_URI_LEN+10];
    sprintf(filename, CONFIG_BSP_SPIFFS_MOUNT_POINT "%s", req->uri);

    return send_file_response(req, filename);
}


httpd_uri_t basic_handlers[] = {
    { .uri      = "/",
            .method   = HTTP_GET,
            .handler  = api_handler_get_index,
            .user_ctx = NULL,
    },
    { .uri      = "/api/settings",
            .method   = HTTP_GET,
            .handler  = api_handler_get_settings,
            .user_ctx = NULL,
    },
    { .uri      = "/api/settings",
            .method   = HTTP_POST,
            .handler  = api_handler_post_settings,
            .user_ctx = NULL,
    },
    { .uri      = "/api/restart",
      .method   = HTTP_GET,
      .handler  = api_handler_restart,
      .user_ctx = NULL,
    },
    { .uri      = "/api/info",
      .method   = HTTP_GET,
      .handler  = api_handler_info,
      .user_ctx = NULL,
    },
    { .uri      = "/api/menjin/cmd",
      .method   = HTTP_GET,
      .handler  = api_handler_menjin_cmd,
      .user_ctx = NULL,
    }
};

int basic_handlers_no = sizeof(basic_handlers)/sizeof(httpd_uri_t);
void register_basic_handlers(httpd_handle_t hd)
{
    int i;
    ESP_LOGI(TAG, "Registering basic handlers");
    ESP_LOGI(TAG, "No of handlers = %d", basic_handlers_no);
    for (i = 0; i < basic_handlers_no; i++) {
        if (httpd_register_uri_handler(hd, &basic_handlers[i]) != ESP_OK) {
            ESP_LOGW(TAG, "register uri failed for %d", i);
            return;
        }
    }

    httpd_register_err_handler(hd, HTTPD_404_NOT_FOUND, http_404_error_handler);

    ESP_LOGI(TAG, "Success");
}

httpd_handle_t http_server_start()
{
    pre_start_mem = esp_get_free_heap_size();
    httpd_handle_t hd;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.server_port = 80;

    /* This check should be a part of http_server */
    config.max_open_sockets = (CONFIG_LWIP_MAX_SOCKETS - 3);

    if (httpd_start(&hd, &config) == ESP_OK) {
        ESP_LOGI(TAG, "Started HTTP server on port: '%d'", config.server_port);
        ESP_LOGI(TAG, "Max URI handlers: '%d'", config.max_uri_handlers);
        ESP_LOGI(TAG, "Max Open Sessions: '%d'", config.max_open_sockets);
        ESP_LOGI(TAG, "Max Header Length: '%d'", HTTPD_MAX_REQ_HDR_LEN);
        ESP_LOGI(TAG, "Max URI Length: '%d'", HTTPD_MAX_URI_LEN);
        ESP_LOGI(TAG, "Max Stack Size: '%d'", config.stack_size);
        return hd;
    }

    return NULL;
}

static httpd_handle_t server = NULL;

void http_server_init(void *pvParameters)
{

    ESP_LOGI(TAG, "check wifi connected");
    while (!app_wifi_is_connected()) {
        vTaskDelay(pdTICKS_TO_MS(1000));
    }
    ESP_LOGI(TAG, "wifi ok, start mqtt_client");

    server = http_server_start();
    if (server) {
        register_basic_handlers(server);
    } else {
        ESP_LOGE(TAG, "Failed to start httpd server");
    }

    vTaskDelete(NULL);
}

void http_server_stop(httpd_handle_t hd)
{
    ESP_LOGI(TAG, "Stopping httpd");
    httpd_stop(hd);
    post_stop_mem = esp_get_free_heap_size();
    ESP_LOGI(TAG, "HTTPD Stop: Current free memory: %d", post_stop_mem);
}

