#include "oled.h"
#include "rgb.h"
#include "sdkconfig.h"
#include "sht4x.h"
#include <Arduino.h>
#include <WiFi.h>
#include <driver/gpio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <freertos/task.h>
#include <stdio.h>

#define BLINK_GPIO (gpio_num_t)2

#ifndef LED_BUILTIN
#define LED_BUILTIN 4
#endif

typedef struct xSht4xData {
  sensors_event_t temp, humidity;
  int count;
} xSht4xData;

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

void rgb_task(void *pvParameter) {
  while (1)
    rgb_show();
}
void sht4x_task(void *pvParam) {
  QueueHandle_t qHandle;
  qHandle = (QueueHandle_t)pvParam;
  BaseType_t xStatus;
  xSht4xData xData;
  while (1) {
    xStatus = xQueueSend(qHandle, &xData, 0);
    if (xStatus != pdPASS)
      Serial.println("sht4x_task send ok");
    else
      Serial.println("sht4x_task send on");
    sht4x_getEvent(&xData.temp, &xData.humidity);
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

void oeld_task(void *pvParam) {
  QueueHandle_t qHandle;
  qHandle = (QueueHandle_t)pvParam;
  BaseType_t xStatus;
  xSht4xData xData;
  while (1) {
    if (uxQueueMessagesWaiting(qHandle) != 0) {
      xStatus = xQueueReceive(qHandle, &xData, 0);

      if (xStatus != pdPASS)
        Serial.println("oeld_task receive ok");
      else
        Serial.println("oeld_task receive on");
      oled_show(xData.temp.temperature, xData.humidity.relative_humidity,
                xData.count);
      vTaskDelay(100 / portTICK_PERIOD_MS);

    } else {
      Serial.println("-------On Data-------");
    }
  }
}

void queue_task() {
  QueueHandle_t qHandle;
  qHandle = xQueueCreate(10, sizeof(xSht4xData));

  if (qHandle != NULL) {
    xTaskCreate(&sht4x_task, "sht4x_task", 1024 * 5, (void *)qHandle, 5, NULL);
    xTaskCreate(&oeld_task, "oeld_task", 1024 * 5, (void *)qHandle, 5, NULL);
  } else
    Serial.println("Queue create error!");
}

void wifiScan() {
  // WiFi.scanNetworks will return the number of networks found
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n == 0) {
    Serial.println("no networks found");
  } else {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i) {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? " " : "*");
      delay(10);
    }
  }
  Serial.println("");
}

#if !CONFIG_AUTOSTART_ARDUINO
void arduinoTask(void *pvParameter) {
  // Set WiFi to station mode and disconnect from an AP if it was previously
  // connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  Serial.begin(115200);
  delay(100);

  while (1) {
    wifiScan();

    // Wait a bit before scanning again
    delay(5000);
  }
}

extern "C" void app_main() {
  // initialize arduino library before we start the tasks
  initArduino();

  xTaskCreate(&arduinoTask, "arduino_task", configMINIMAL_STACK_SIZE, NULL, 5,
              NULL);
}
#else
void setup() {
  Serial.begin(115200);

  rgb_init();
  oled_init();
  sht4x_init();
  xTaskCreate(&rgb_task, "rgb_task", 1024, NULL, 4, NULL);
  xTaskCreate(&blink_task, "blink_task", 1024, NULL, 5, NULL);
  queue_task();
}

void loop() {}
#endif
