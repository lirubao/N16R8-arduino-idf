#include "rgb.h"

#define LED_PIN 48  // 板载RGB灯珠的引脚，根据实际使用的开发板型号而定
#define LED_COUNT 1 // LED灯条的灯珠数量（板载的是一颗）

/*
使用 Adafruit_NeoPixel 库创建了一个名为 strip 的对象，控制LED灯珠
      LED_COUNT 表示 LED 条上的 LED 数量，LED_PIN 表示连接到 Arduino
的引脚，NEO_GRB + NEO_KHZ800 用于设置 LED 条的颜色排列和通信速率
      */
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

void rgb_init() {
    strip.begin();
    strip.setBrightness(50); // 设置亮度（0-255范围）
}

void rgb_task(void *pvParameter) {
    while (1) {
        strip.setPixelColor(0, strip.Color(255, 0, 0)); // 设置灯珠为红色 (R, G, B)
        strip.show();                                   // 显示颜色
        vTaskDelay(1000 / portTICK_PERIOD_MS);          // 延迟1秒

        strip.setPixelColor(0, strip.Color(0, 255, 0)); // 设置灯珠为绿色 (R, G, B)
        strip.show();                                   // 显示颜色
        vTaskDelay(1000 / portTICK_PERIOD_MS);          // 延迟1秒

        strip.setPixelColor(0, strip.Color(0, 0, 255)); // 设置灯珠为蓝色 (R, G, B)
        strip.show();                                   // 显示颜色
        vTaskDelay(1000 / portTICK_PERIOD_MS);          // 延迟1秒
    }
}