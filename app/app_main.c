#include "app_main.h"

void app_main(void)
{
    hardware_init();
    system_utils_init();
    lv_init();
    tft_init();
    touchpad_init();
    app_create_threads();
    gui_app();
}