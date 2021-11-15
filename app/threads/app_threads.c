#include "app_threads.h"
#include "app.h"
#include "fatfs.h"
#include "gui_mass_storage_tab.h"

#define MAX_MESSAGE_QUEUE_OBJECTS 16
#define MAX_MESSAGE_QUEUE_SIZE 256

typedef struct {
  uint8_t buffer[MAX_MESSAGE_QUEUE_SIZE - 1];
  uint8_t id;
} message_box_t;

static osMessageQueueId_t msg_queue;
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
static void translate_fatfs_error_code_to_string(FRESULT error_code, char *string);

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

static void _create_message_queue(void)
{
    msg_queue = osMessageQueueNew(MAX_MESSAGE_QUEUE_OBJECTS, MAX_MESSAGE_QUEUE_SIZE, NULL);
    if (msg_queue == NULL) {
        // Message Queue object not created, handle failure
    }    
}

void put_queue_rng_thread(void *data, uint8_t id)
{
    message_box_t msg;
    strcpy((char *)msg.buffer, (char *)data);
    msg.id = id;
    osMessageQueuePut(msg_queue, &msg, 0U, 0U);
}

static void rng_thread(void *argument)
{
    char string_err[32];
    message_box_t msg;
    osStatus_t status;
    uint8_t ret;
    _create_message_queue();
    create_file_example();
    for(;;) {
        status = osMessageQueueGet(msg_queue, &msg, NULL, 0U);
        if (status != osOK) {
            continue;
        }
        ret = create_file_from_random_data((const char *)msg.buffer, msg.id);
        if (ret != 0) {
            translate_fatfs_error_code_to_string(ret, string_err);
            printf("[APP]: ERROR: %s\n\r", string_err);
        }
        osDelay(1000);
        gui_close();
    }
}

static void _create_mutexes(void)
{
    lvgl_mutex_id = osMutexNew(NULL);
    if (lvgl_mutex_id == NULL) {
        // Error
    }
}

static void translate_fatfs_error_code_to_string(FRESULT error_code, char *string)
{
    switch (error_code) {
    case FR_OK:
        strcpy(string, "FR_OK");
        break;
    case FR_DISK_ERR:
        strcpy(string, "FR_DISK_ERR");
        break;
    case FR_INT_ERR:
        strcpy(string, "FR_INT_ERR");
        break;
    case FR_NOT_READY:
        strcpy(string, "FR_NOT_READY");
        break;
    case FR_NO_FILE:
        strcpy(string, "FR_NO_FILE");
        break;
    case FR_NO_PATH:
        strcpy(string, "FR_NO_PATH");
        break;
    case FR_INVALID_NAME:
        strcpy(string, "FR_INVALID_NAME");
        break;
    case FR_DENIED:
        strcpy(string, "FR_DENIED");
        break;
    case FR_EXIST:
        strcpy(string, "FR_EXIST");
        break;
    case FR_INVALID_OBJECT:
        strcpy(string, "FR_INVALID_OBJECT");
        break;
    case FR_WRITE_PROTECTED:
        strcpy(string, "FR_WRITE_PROTECTED");
        break;
    case FR_INVALID_DRIVE:
        strcpy(string, "FR_INVALID_DRIVE");
        break;
    case FR_NOT_ENABLED:
        strcpy(string, "FR_NOT_ENABLED");
        break;
    case FR_NO_FILESYSTEM:
        strcpy(string, "FR_NO_FILESYSTEM");
        break;
    case FR_MKFS_ABORTED:
        strcpy(string, "FR_MKFS_ABORTED");
        break;
    case FR_TIMEOUT:
        strcpy(string, "FR_TIMEOUT");
        break;
    case FR_LOCKED:
        strcpy(string, "FR_LOCKED");
        break;
    case FR_NOT_ENOUGH_CORE:
        strcpy(string, "FR_NOT_ENOUGH_CORE");
        break;
    case FR_TOO_MANY_OPEN_FILES:
        strcpy(string, "FR_TOO_MANY_OPEN_FILES");
        break;
    case FR_INVALID_PARAMETER: 
        strcpy(string, "FR_INVALID_PARAMETER");
        break;
    default:
        break;
    }
}