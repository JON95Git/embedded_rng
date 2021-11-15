#ifndef __APP_TASKS_H
#define __APP_TASKS_H

#include "main.h"
#include "cmsis_os.h"
#include "cmsis_os2.h"
#include "lvgl.h"
#include "tft.h"
#include "touchpad.h"
#include "lv_arc.h"
#include "system.h"

void app_create_threads(void);
void lvgl_task(void const *argument);
void put_queue_rng_thread(void *data, uint8_t id);

#endif /* __APP_TASKS_H */