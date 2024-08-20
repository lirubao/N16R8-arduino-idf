#include "sdkconfig.h"
#include "queue.h"
#include <Arduino.h>
#include <stdio.h>

#if !CONFIG_AUTOSTART_ARDUINO
void arduinoTask(void *pvParameter) {
    while (1) {
    }
}

extern "C" void app_main() {
    // initialize arduino library before we start the tasks
    initArduino();

    queue_task();
    xTaskCreate(&arduinoTask, "arduino_task", configMINIMAL_STACK_SIZE, NULL, 5, NULL);
}
#else
void setup() {
    Serial.begin(115200);
    queue_init();
    queue_task();
}

void loop() {
}
#endif
