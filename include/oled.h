#ifndef __OLED_H_
#define __OLED_H_

void oled_init();
void oled_show(float temp, float humidity, int count = 0);
#endif