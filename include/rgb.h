#ifndef __RGB_HH_
#define __RGB_HH_
#include <Adafruit_NeoPixel.h>
#include <freertos/FreeRTOS.h>

void rgb_init();
void rgb_task(void *pvParameter);
#endif