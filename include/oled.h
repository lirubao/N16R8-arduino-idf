#ifndef __OLED_H_
#define __OLED_H_
#include <U8g2lib.h>
#ifdef U8X8_HAVE_HW_SPI
#    include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#    include <Wire.h>
#endif

void oled_init();
void oled_show(float temp, float humidity, int count = 0);
#endif