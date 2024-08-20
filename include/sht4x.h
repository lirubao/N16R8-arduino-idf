#ifndef __SHT4X_HH_
#define __SHT4X_HH_
#include <Adafruit_SHT4x.h>
#include <Arduino.h>
typedef struct xSht4xData {
    sensors_event_t temp, humidity;
    int count;
} xSht4xData;

void sht4x_init();
void sht4x_getEvent(sensors_event_t *temp, sensors_event_t *humidity);
#endif