#include <sys/cdefs.h>
//
// Created by meshi on 24-7-27.
//

#ifndef ESPANDORA_APP_SENSOR_H
#define ESPANDORA_APP_SENSOR_H

typedef struct {
    float temperature;
    float humidity;
    float pressure;
} rht_report_data_t;

float BMP280PressureToAltitude(float* pressure/*, float* groundPressure, float* groundTemp*/);
_Noreturn void app_sensor_task();
#endif //ESPANDORA_APP_SENSOR_H
