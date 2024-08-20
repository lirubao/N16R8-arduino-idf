#ifndef __QUEUE_HH_
#define __QUEUE_HH_
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <freertos/task.h>

void queue_init();
void queue_task();
#endif