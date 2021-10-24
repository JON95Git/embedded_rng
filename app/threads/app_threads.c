#include "app_threads.h"
#include "fatfs.h"

static osMutexId_t lvgl_mutex_id;
static osThreadId_t lvgl_thread_handle;
static const osThreadAttr_t lvgl_thread_attributes = {
  .name = "LVGL Thread",
  .priority = (osPriority_t) osPriorityHigh,
  .stack_size = 2048 * 4
};

static osThreadId_t rng_thread_handle;
static const osThreadAttr_t rng_thread_attributes = {
  .name = "RNG Thread",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 2048 * 4
};

static void lvgl_thread(void *argument);
static void rng_thread(void *argument);
static void _create_mutexes(void);

void app_create_threads(void)
{
    lvgl_thread_handle = osThreadNew(lvgl_thread, NULL, &lvgl_thread_attributes);
    rng_thread_handle = osThreadNew(rng_thread, NULL, &rng_thread_attributes);
}

static void lvgl_thread(void *argument)
{
    _create_mutexes();
    for (;;) {
        osDelay(5);
        osMutexWait(lvgl_mutex_id, osWaitForever);
        lv_task_handler();
        osMutexRelease(lvgl_mutex_id);
    }
}

static void rng_thread(void *argument)
{
    create_file_example();
    for(;;) {
        
    }
}

static void _create_mutexes(void)
{
    lvgl_mutex_id = osMutexNew(NULL);
    if (lvgl_mutex_id == NULL) {
        // Error
    }
}