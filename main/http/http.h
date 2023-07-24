//
// Created by Hessian on 2023/7/19.
//

#ifndef FACTORY_DEMO_HTTP_H
#define FACTORY_DEMO_HTTP_H


typedef void (*http_fans_change_cb)(int fans);

int http_get_bilibili_fans();
void http_bilibili_task_start();
esp_err_t http_get_weather(char *result);


#endif //FACTORY_DEMO_HTTP_H
