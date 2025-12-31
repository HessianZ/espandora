#include <esp_types.h>
#include <string.h>
#include <inttypes.h>
#include <esp_http_client.h>
#include <sys/param.h>
#include <sys/time.h>
#include "esp_log.h"
#include "nvs.h"

#include "esp_tls.h"
#include "sdkconfig.h"
#include "esp_crt_bundle.h"
#include "http.h"
#include "json_parser.h"

#define BILIBILI_UID "10442962"
#define BILIBILI_FANS_URL "https://api.bilibili.com/x/relation/stat?vmid=" BILIBILI_UID "&jsonp=jsonp"

// #define CITY_CODE "101280401" // 梅州
#define CITY_CODE "101080201" // 包头
#define WEATHER_URL "https://d1.weather.com.cn/weather_index/" CITY_CODE ".html?_=%d"
#define WEATHER_REFERER "http://www.weather.com.cn/"

#define MAX_HTTP_OUTPUT_BUFFER 5192

static const char *TAG = "http";


#define USER_AGENT "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/114.0.0.0 Safari/537.36 Edg/114.0.1823.37"


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
            // Handle both chunked and non-chunked responses
            if (evt->user_data) {
                int copy_len = MIN(evt->data_len, (MAX_HTTP_OUTPUT_BUFFER - output_len));
                if (copy_len) {
                    memcpy(evt->user_data + output_len, evt->data, copy_len);
                    output_len += copy_len;
                }
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
            ESP_LOGD(TAG, "HTTP_EVENT_DISCONNECTED");
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

esp_err_t http_get_weather(weather_result_t *result)
{
    ESP_LOGD(TAG, "Start http_get_weather ...");

    char *response = calloc(8192, sizeof(char));

    // get unix timestamp in milliseconds
    struct timeval get_nvs_time;
    
    gettimeofday(&get_nvs_time, NULL);
    int64_t ts = get_nvs_time.tv_sec * 1000 + get_nvs_time.tv_usec / 1000;
    char url[128];
    sprintf(url, WEATHER_URL, ts);

    esp_http_client_config_t config = {
            .url = url,
            .event_handler = _http_event_handler,
            .crt_bundle_attach = esp_crt_bundle_attach,
            .buffer_size = 8192 * sizeof(char),
            .user_data = response,
            .user_agent = USER_AGENT
    };
    esp_http_client_handle_t client = esp_http_client_init(&config);
    esp_http_client_set_header(client, "Referer", WEATHER_REFERER);
    esp_err_t err = esp_http_client_perform(client);

    if (err == ESP_OK) {
        size_t output_len = strlen(response);

        ESP_LOGD(TAG, "HTTPS Status = %d, response_len = %d",
                 esp_http_client_get_status_code(client),
                 output_len
        );
        if (output_len > 0 && output_len < 8192) {
            response[output_len] = '\0';
        }
        ESP_LOGD(TAG, "Response: %s", response);
    } else {
        ESP_LOGE(TAG, "Error perform http request %s", esp_err_to_name(err));
    }
    esp_http_client_cleanup(client);

    // json eg.:
    // var cityDZ ={"weatherinfo":{"city":"包头","cityname":"baotou","temp":"-11","tempn":"-17","weather":"晴","wd":"西风","ws":"<3级","weathercode":"d0","weathercoden":"n0","fctime":"202512310800"}};var alarmDZ ={"w":[]};var dataSK ={"nameen":"baotou","cityname":"包头","city":"101080201","temp":"-6","tempf":"21.2","WD":"西南风","wde":"SW","WS":"1级","wse":"5km\/h","SD":"27%","sd":"27%","qy":"911","njd":"30km","time":"15:05","rain":"0","rain24h":"0","aqi":"48","aqi_pm25":"48","weather":"晴","weathere":"Sunny","weathercode":"d00","limitnumber":"","date":"12月31日(星期三)"};var dataZS ={"zs":{"date":"2025123111","lk_name":"路况指数","lk_hint":"干燥","lk_des_s":"天气较好，路面较干燥，路况较好。","cl_name":"晨练指数","cl_hint":"适宜","cl_des_s":"天气不错，空气清新。","pl_name":"空气污染扩散条件指数","pl_hint":"中","pl_des_s":"易感人群应适当减少室外活动。","gj_name":"逛街指数","gj_hint":"较不宜","gj_des_s":"天气较冷，坚持出门需注意保暖。","co_name":"舒适度指数","co_hint":"很不舒适","co_des_s":"白天晴，天气凉，注意保暖。","pj_name":"啤酒指数","pj_hint":"不适宜","pj_des_s":"天气寒冷，可少量饮用常温啤酒。","hc_name":"划船指数","hc_hint":"不适宜","hc_des_s":"天气寒冷，不适宜划船。","gl_name":"太阳镜指数","gl_hint":"需要","gl_des_s":"白天天空晴朗，请适时佩戴太阳镜","uv_name":"紫外线强度指数","uv_hint":"中等","uv_des_s":"涂擦SPF大于15、PA+防晒护肤品。","wc_name":"风寒指数","wc_hint":"寒冷","wc_des_s":"感觉寒冷，室外活动要注意保暖，可戴手套与帽子。","ct_name":"穿衣指数","ct_hint":"寒冷","ct_des_s":"建议着厚羽绒服等隆冬服装。","pk_name":"放风筝指数","pk_hint":"不宜","pk_des_s":"天气寒冷，不适宜放风筝。","ac_name":"空调开启指数","ac_hint":"开启制暖空调","ac_des_s":"体感略冷，建议开启暖空调调节室温。","dy_name":"钓鱼指数","dy_hint":"不宜","dy_des_s":"天气冷，不适合垂钓。","ls_name":"晾晒指数","ls_hint":"基本适宜","ls_des_s":"天气不错，抓紧时机让衣物晒太阳吧。","gm_name":"感冒指数","gm_hint":"极易发","gm_des_s":"天气寒冷，昼夜温差极大。","xc_name":"洗车指数","xc_hint":"适宜","xc_des_s":"天气较好，适合擦洗汽车。","tr_name":"旅游指数","tr_hint":"较适宜","tr_des_s":"有微风稍冷，但仍可尽情享受自然风光。","nl_name":"夜生活指数","nl_hint":"较不适宜","nl_des_s":"建议夜生活最好在室内进行。","xq_name":"心情指数","xq_hint":"较好","xq_des_s":"晴朗天气，将为您的身心带来温暖。","yh_name":"约会指数","yh_hint":"较不适宜","yh_des_s":"天气较冷，最好在温暖的室内促膝谈心。","yd_name":"运动指数","yd_hint":"较不宜","yd_des_s":"天气寒冷，推荐您进行室内运动。","ag_name":"过敏指数","ag_hint":"极不易发","ag_des_s":"无需担心过敏。","mf_name":"美发指数","mf_hint":"一般","mf_des_s":"建议选用防晒保湿型护发品。","ys_name":"雨伞指数","ys_hint":"不带伞","ys_des_s":"天气较好，不用带雨伞。","fs_name":"防晒指数","fs_hint":"中等","fs_des_s":"适合涂擦SPF大于15，PA+护肤品。","pp_name":"化妆指数","pp_hint":"保湿","pp_des_s":"请选用滋润保湿型化妆品。","zs_name":"中暑指数","zs_hint":"无中暑风险","zs_des_s":"天气舒适，对易中暑人群来说非常友善。","jt_name":"交通指数","jt_hint":"良好","jt_des_s":"气象条件良好，车辆可以正常行驶。","gz_name":"干燥指数","gz_hint":"适宜","gz_des_s":"气温偏低，体感稍冷，空气湿度状况适宜，使用普通保湿型护肤品即可。"},"cn":"包头"};var fc ={"f":[{"fa":"00","fb":"00","fc":"-4","fd":"-19","fe":"西风","ff":"西风","fg":"<3级","fh":"<3级","fk":"6","fl":"6","fm":"81.9","fn":"40.6","fi":"12\/31","fj":"今天"},{"fa":"00","fb":"00","fc":"-1","fd":"-19","fe":"西北风","ff":"西北风","fg":"<3级","fh":"<3级","fk":"7","fl":"7","fm":"81.2","fn":"26.9","fi":"1\/1","fj":"星期四"},{"fa":"00","fb":"00","fc":"-1","fd":"-20","fe":"西风","ff":"西风","fg":"<3级","fh":"<3级","fk":"6","fl":"6","fm":"82.9","fn":"28.2","fi":"1\/2","fj":"星期五"},{"fa":"00","fb":"00","fc":"-2","fd":"-14","fe":"西风","ff":"北风","fg":"3-4级","fh":"3-4级","fk":"6","fl":"8","fm":"77.1","fn":"29.2","fi":"1\/3","fj":"星期六"},{"fa":"00","fb":"00","fc":"-2","fd":"-18","fe":"北风","ff":"北风","fg":"<3级","fh":"<3级","fk":"8","fl":"8","fm":"89.7","fn":"38","fi":"1\/4","fj":"星期日"}]}
    char *json = NULL;
    jparse_ctx_t *jctx = NULL;
    const char *tok = "var dataSK =";
    char* pStart = strstr(response, tok);
    if (pStart == NULL) {
        ESP_LOGE(TAG, "Parse weather failed: %s", response);
        goto exception;
    }

    pStart += strlen(tok);  // Skip to inner object
    char* pEnd = strstr(pStart, "};");
    if (pEnd == NULL) {
        ESP_LOGE(TAG, "Parse weather failed: end marker not found");
        goto exception;
    }
    pEnd += 1;  // Include the closing }
    int len = pEnd - pStart;
    json = malloc(len + 1);
    memcpy(json, pStart, len);
    json[len] = '\0';
    
    ESP_LOGD(TAG, "Extracted JSON: %s", json);
    free(response);
    response = NULL;

    jctx = (jparse_ctx_t *)malloc(sizeof(jparse_ctx_t));
    int ret = json_parse_start(jctx, json, strlen(json));
    if (ret != OS_SUCCESS) {
        ESP_LOGE(TAG, "json_parse_start failed, json: %s", json);
        goto exception;
    }

    if (json_obj_get_string(jctx, "cityname", result->city, 10) != OS_SUCCESS) {
        ESP_LOGE(TAG, "json_obj_get_string failed: cityname\n");
        goto exception;
    }
    if (json_obj_get_string(jctx, "temp", result->temp, 8) != OS_SUCCESS) {
        ESP_LOGE(TAG, "json_obj_get_string failed: temp");
        goto exception;
    }
    if (json_obj_get_string(jctx, "SD", result->humi, 8) != OS_SUCCESS) {
        ESP_LOGE(TAG, "json_obj_get_string failed: humidity");
        goto exception;
    }
    if (json_obj_get_string(jctx, "weather", result->weather, 12) != OS_SUCCESS) {
        ESP_LOGE(TAG, "json_obj_get_string failed: weather");
        goto exception;
    }
    if (json_obj_get_string(jctx, "WD", result->wind, 12) != OS_SUCCESS) {
        ESP_LOGE(TAG, "json_obj_get_string failed: wind");
        goto exception;
    }
    if (json_obj_get_string(jctx, "WS", result->windSpeed, 10) != OS_SUCCESS) {
        ESP_LOGE(TAG, "json_obj_get_string failed: windSpeed");
        goto exception;
    }
    
    // Optional field SD (humidity) may not exist in response
    if (json_obj_get_string(jctx, "SD", result->humi, 5) == OS_SUCCESS) {
        char *pct = strstr(result->humi, "%");
        if (pct) pct[0] = '\0';
    } else {
        strcpy(result->humi, "N/A");
    }
    
    char *level = strstr(result->windSpeed, "级");
    if (level) level[0] = '\0';

    json_parse_end(jctx);
    free(json);
    json = NULL;


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
        ESP_LOGD(TAG, "HTTPS Status = %d, content_length = %"PRId64,
                 esp_http_client_get_status_code(client),
                 esp_http_client_get_content_length(client)
        );
        strstr(json, "}}")[2] = '\0';
        ESP_LOGD(TAG, "Response: %s", json);
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
