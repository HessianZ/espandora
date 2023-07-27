/*
 * SPDX-FileCopyrightText: 2015-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Unlicense OR CC0-1.0
 */

#include <esp_partition.h>
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "esp_heap_caps.h"
#include "esp_log.h"
#include "esp_check.h"
#include "app_sr.h"
#include "file_manager.h"
#include "file_iterator.h"
#include "bsp_board.h"
#include "bsp/esp-bsp.h"
#include "ui_sr.h"
#include "app_sr_handler.h"
#include "settings.h"
#include "http.h"
#include "esp_tts.h"
#include "esp_tts_voice_template.h"
#include "util.h"
#include <string.h>


static const char *TAG = "sr_handler";

static bool b_audio_playing = false;

extern file_iterator_instance_t* file_iterator;

typedef enum {
    AUDIO_WAKE,
    AUDIO_OK,
    AUDIO_END,
    AUDIO_MAX,
} audio_segment_t;

typedef struct {
    uint8_t *audio_buffer;
    size_t len;
} audio_data_t;

static audio_data_t g_audio_data[AUDIO_MAX];

void tts_read(char *str)
{
    /*** 1. create esp tts handle ***/
    // initial voice set from separate voice data partition

    const esp_partition_t* part=esp_partition_find_first(ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_ANY, "voice_data");
    if (part==NULL) {
        ESP_LOGE(TAG, "Couldn't find voice data partition!\n");
        return;
    } else {
        ESP_LOGI(TAG, "voice_data paration size:%d\n", part->size);
    }
    void* voicedata;
#if ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(5, 0, 0)
    esp_partition_mmap_handle_t mmap;
    esp_err_t err=esp_partition_mmap(part, 0, part->size, ESP_PARTITION_MMAP_DATA, &voicedata, &mmap);
#else
    spi_flash_mmap_handle_t mmap;
    esp_err_t err=esp_partition_mmap(part, 0, part->size, SPI_FLASH_MMAP_DATA, &voicedata, &mmap);
#endif
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Couldn't map voice data partition!\n");
        return;
    }
    esp_tts_voice_t *voice=esp_tts_voice_set_init(&esp_tts_voice_template, (int16_t*)voicedata);

    esp_tts_handle_t *tts_handle=esp_tts_create(voice);

    /*** 2. play prompt text ***/
    ESP_LOGI(TAG, "play prompt text: %s", str);
    if (esp_tts_parse_chinese(tts_handle, str)) {
        int len[1]={0};
        bsp_codec_config_t *codec_handle = bsp_board_get_codec_handle();
        codec_handle->i2s_reconfig_clk_fn(16000, I2S_DATA_BIT_WIDTH_16BIT, I2S_SLOT_MODE_MONO);
        codec_handle->mute_set_fn(false);
        codec_handle->volume_set_fn(100, NULL);

        do {
            short *pcm_data=esp_tts_stream_play(tts_handle, len, 3);
            size_t bytes_written = 0;
            codec_handle->i2s_write_fn(pcm_data, len[0]*2, &bytes_written, portMAX_DELAY);
        } while(len[0]>0);
        vTaskDelay(pdMS_TO_TICKS(20));
    }
    esp_tts_stream_reset(tts_handle);
}


int play_bilibili_fans()
{
    sr_anim_set_text("正在请求...");
    char *prompt1 = (char*)malloc(50);
    char *prompt2 = (char*)malloc(50);
    int fans = http_get_bilibili_fans();
    if (fans < 0) {
        strcpy(prompt1, "获取粉丝数失败");
    } else {
        char *fansCn = (char*) malloc(30);
        memset(fansCn, 0, sizeof(fansCn));
        num2cn(fans, fansCn);
        sprintf(prompt1, "必站粉丝%s人", fansCn);
        sprintf(prompt2, "B站粉丝%d人", fans);
        sr_anim_set_text(prompt2);
        free(fansCn);
    }
    tts_read(prompt1);
    free(prompt1);
    free(prompt2);
}


int play_weather()
{
    weather_result_t *weather = malloc(sizeof(weather_result_t));
    memset(weather, 0, sizeof(weather_result_t));

    char *str = malloc(128);
    memset(str, 0, 128);

    sr_anim_set_text("正在请求...");
    esp_err_t ret = http_get_weather(weather);
    ESP_LOGI(TAG, "play_weather ret=%d", ret);
    if (ret != ESP_OK) {
        strcpy(str, "获取天气失败");
        sr_anim_set_text(str);
        tts_read(str);
    } else {
        char tempCn[12];
        char windSpeedCn[12];
        char humiCn[12];
        memset(tempCn, 0, 12);
        memset(windSpeedCn, 0, 12);
        memset(humiCn, 0, 12);
        num2cn(atoi(weather->temp), &tempCn);
        num2cn(atoi(weather->humi), &humiCn);
        num2cn(atoi(weather->windSpeed), &windSpeedCn);

        sprintf(str, "%s %s %s℃ 湿度%s%% %s%s级",
                weather->city,
                weather->weather,
                weather->temp,
                weather->humi,
                weather->wind,
                weather->windSpeed
        );
        sr_anim_set_text(str);
        ESP_LOGI(TAG, "play_weather str=%s", str);

        char *ttsStr = malloc(128);
        memset(ttsStr, 0, 128);
        sprintf(ttsStr, "%s %s\n气温%s摄氏度\n湿度百分之%s\n%s%s级",
                weather->city,
                weather->weather,
                tempCn,
                humiCn,
                weather->wind,
                windSpeedCn
        );

        tts_read(ttsStr);
    }
    free(str);
    free(weather);
}

static esp_err_t sr_echo_play(audio_segment_t audio)
{
    bsp_audio_poweramp_enable(false); // turn off the speaker to avoid play some noise

    typedef struct {
        // The "RIFF" chunk descriptor
        uint8_t ChunkID[4];
        int32_t ChunkSize;
        uint8_t Format[4];
        // The "fmt" sub-chunk
        uint8_t Subchunk1ID[4];
        int32_t Subchunk1Size;
        int16_t AudioFormat;
        int16_t NumChannels;
        int32_t SampleRate;
        int32_t ByteRate;
        int16_t BlockAlign;
        int16_t BitsPerSample;
        // The "data" sub-chunk
        uint8_t Subchunk2ID[4];
        int32_t Subchunk2Size;
    } wav_header_t;

    /**
     * read head of WAV file
     */
    bsp_codec_config_t *codec_handle = bsp_board_get_codec_handle();
    uint8_t *p = g_audio_data[audio].audio_buffer;
    wav_header_t *wav_head = (wav_header_t *)p;

    if (NULL == strstr((char *)wav_head->Subchunk1ID, "fmt") &&
            NULL == strstr((char *)wav_head->Subchunk2ID, "data")) {
        ESP_LOGE(TAG, "Header of wav format error");
        return ESP_FAIL;
    }
    p += sizeof(wav_header_t);
    size_t len = g_audio_data[audio].len - sizeof(wav_header_t);
    len = len & 0xfffffffc;
    ESP_LOGI(TAG, "frame_rate=%d, ch=%d, width=%d", wav_head->SampleRate, wav_head->NumChannels, wav_head->BitsPerSample);
    codec_handle->i2s_reconfig_clk_fn(wav_head->SampleRate, wav_head->BitsPerSample, I2S_SLOT_MODE_STEREO);

    vTaskDelay(pdMS_TO_TICKS(500));
    codec_handle->mute_set_fn(false);
    codec_handle->volume_set_fn(85, NULL);

    sys_param_t *param = settings_get_parameter();
    codec_handle->volume_set_fn(param->volume, NULL);
    codec_handle->mute_set_fn(false);
    ESP_LOGI(TAG, "bsp_codec_set_voice_volume=%d", param->volume);

    vTaskDelay(pdMS_TO_TICKS(50));
    bsp_audio_poweramp_enable(true);

    size_t bytes_written = 0;
    b_audio_playing = true;
    codec_handle->i2s_write_fn((char *)p, len, &bytes_written, portMAX_DELAY);
    vTaskDelay(pdMS_TO_TICKS(20));

    b_audio_playing = false;
    return ESP_OK;
}

bool sr_echo_is_playing(void)
{
    return b_audio_playing;
}

sr_language_t sr_detect_language()
{
    static sr_language_t sr_current_lang = SR_LANG_MAX;
    esp_err_t ret;
    FILE *fp = NULL;
    const sys_param_t * param = settings_get_parameter();

    if(param->sr_lang^sr_current_lang){
        sr_current_lang = param->sr_lang;
        ESP_LOGI(TAG, "boardcast language change to = %s", (SR_LANG_EN == param->sr_lang ? "EN" : "CN"));

        const char *files[2][3] = {
            {"/spiffs/echo_en_wake.wav", "/spiffs/echo_en_ok.wav", "/spiffs/echo_en_end.wav"},
            {"/spiffs/echo_cn_wake.wav", "/spiffs/echo_cn_ok.wav", "/spiffs/echo_cn_end.wav"},
        };

        char audio_file[48] = {0};
        for (size_t i = 0; i < AUDIO_MAX; i++) {
            strncpy(audio_file, files[param->sr_lang][i], sizeof(audio_file));
            fp = fopen(audio_file, "rb");
            ESP_GOTO_ON_FALSE(NULL != fp, ESP_ERR_NOT_FOUND, err, TAG, "Open file %s failed", audio_file);
            size_t file_size = fm_get_file_size(audio_file);

            if(g_audio_data[i].audio_buffer){
                heap_caps_free(g_audio_data[i].audio_buffer);
                g_audio_data[i].len = 0;
            }
            g_audio_data[i].len = file_size;
            g_audio_data[i].audio_buffer = heap_caps_calloc(1, file_size, MALLOC_CAP_8BIT | MALLOC_CAP_SPIRAM);
            ESP_GOTO_ON_FALSE(NULL != g_audio_data[i].audio_buffer, ESP_ERR_NO_MEM, err, TAG,  "No mem for sr echo buffer");
            fread(g_audio_data[i].audio_buffer, 1, file_size, fp);
            fclose(fp);
        }
    }
    return sr_current_lang;

err:
    if(fp){
        fclose(fp);
    }
    return sr_current_lang;
}

void sr_handler_task(void *pvParam)
{
    sr_language_t sr_current_lang;
    while (true) {
        sr_result_t result;
        app_sr_get_result(&result, portMAX_DELAY);
        char audio_file[48] = {0};

        sr_current_lang = sr_detect_language();

        if (ESP_MN_STATE_TIMEOUT == result.state) {
            if (SR_LANG_EN == sr_current_lang) {
                sr_anim_set_text("Timeout");
            } else {
                sr_anim_set_text("超时");
            }
#if !SR_RUN_TEST
            sr_echo_play(AUDIO_END);
#endif
            sr_anim_stop();

            continue;
        }

        if (WAKENET_DETECTED == result.wakenet_mode) {
            sr_anim_start();

            if (SR_LANG_EN == sr_current_lang) {
                sr_anim_set_text("Say command");
            } else {
                sr_anim_set_text("请说");
            }
#if !SR_RUN_TEST
            sr_echo_play(AUDIO_WAKE);
#endif
            continue;
        }

        if (ESP_MN_STATE_DETECTED & result.state) {
            const sr_cmd_t *cmd = app_sr_get_cmd_from_id(result.command_id);
            ESP_LOGI(TAG, "command:%s, act:%d", cmd->str, cmd->cmd);
            sr_anim_set_text((char *) cmd->str);
#if !SR_CONTINUE_DET
            sr_anim_stop();
            if (PLAYER_STATE_PLAYING == last_player_state) {
                app_player_play();
            }
#endif

            switch (cmd->cmd) {
            case SR_CMD_FENSI:
                ESP_LOGW(TAG, "SR FENSI!!!!");
                play_bilibili_fans();
                ESP_LOGW(TAG, "SR FENSI --- END!!!!");
                break;
            case SR_CMD_WEATHER:
                ESP_LOGW(TAG, "SR WEATHER!!!!");
                play_weather();
                ESP_LOGW(TAG, "SR WEATHER --- END!!!!");
                break;
            default:
                ESP_LOGE(TAG, "Unknow cmd");
                break;
            }
#if !SR_RUN_TEST
            if (SR_LANG_EN == sr_current_lang) {
                strncpy(audio_file, "/spiffs/echo_en_ok.wav", sizeof(audio_file));
            } else {
                strncpy(audio_file, "/spiffs/echo_cn_ok.wav", sizeof(audio_file));
            }
            sr_echo_play(AUDIO_OK);
#endif
        }
    }
    vTaskDelete(NULL);
}


