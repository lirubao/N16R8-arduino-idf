#include "queue.h"
#include "oled.h"
#include "sht4x.h"
#include "rgb.h"
#include <driver/gpio.h>

void sht4x_task(void *pvParam) {
    QueueHandle_t qHandle;
    qHandle = (QueueHandle_t)pvParam;
    xSht4xData xData;
    while (1) {
        xQueueSend(qHandle, &xData, 0);
        sht4x_getEvent(&xData.temp, &xData.humidity);
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

void oeld_task(void *pvParam) {
    QueueHandle_t qHandle;
    qHandle = (QueueHandle_t)pvParam;
    xSht4xData xData;
    while (1) {
        if (uxQueueMessagesWaiting(qHandle) != 0) {
            xQueueReceive(qHandle, &xData, 0);
            oled_show(xData.temp.temperature, xData.humidity.relative_humidity, xData.count);
            vTaskDelay(100 / portTICK_PERIOD_MS);
        } else {
            Serial.println("------- No Data -------");
        }
    }
}
#define BLINK_GPIO (gpio_num_t)2

#ifndef LED_BUILTIN
#    define LED_BUILTIN 4
#endif

void blink_task(void *pvParameter) {
    gpio_pad_select_gpio(BLINK_GPIO);
    /* Set the GPIO as a push/pull output */
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);
    while (1) {
        /* Blink off (output low) */
        gpio_set_level(BLINK_GPIO, 0);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        /* Blink on (output high) */
        gpio_set_level(BLINK_GPIO, 1);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
void queue_init() {
    rgb_init();
    oled_init();
    sht4x_init();
}

void queue_task() {

    QueueHandle_t qHandle;
    qHandle = xQueueCreate(10, sizeof(xSht4xData));

    if (qHandle != NULL) {
        xTaskCreate(&sht4x_task, "sht4x_task", 1024 * 5, (void *)qHandle, 5, NULL);
        xTaskCreate(&oeld_task, "oeld_task", 1024 * 5, (void *)qHandle, 5, NULL);
    } else
        Serial.println("Queue create error!");
    xTaskCreate(&rgb_task, "rgb_task", 1024, NULL, 4, NULL);
    xTaskCreate(&blink_task, "blink_task", 1024, NULL, 5, NULL);
}