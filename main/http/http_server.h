//
// Created by Hessian on 2023/7/29.
//
#ifndef ESP_MENJIN_HTTP_SERVER_H
#define ESP_MENJIN_HTTP_SERVER_H

#include <esp_http_server.h>

void http_server_init(void *pvParameters);
void http_server_stop(httpd_handle_t hd);

#endif // ESP_MENJIN_HTTP_SERVER_H
