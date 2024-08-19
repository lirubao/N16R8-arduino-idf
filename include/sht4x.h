#ifndef __SHT4X_HH_
#define __SHT4X_HH_
#include <Adafruit_Sensor.h>

void sht4x_init();
void sht4x_getEvent(sensors_event_t *temp, sensors_event_t *humidity);
#endif