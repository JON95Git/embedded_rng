#ifndef __GUI_RANDOM_H
#define __GUI_RANDOM_H

#ifdef EMBEDDED
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif
#include <stdio.h>
#include "../components/gui_widgets.h"
#include "../components/gui_spinbox.h"

#define USER_COLOR lv_color_hex(0x034885)

void gui_create_random_tab(lv_obj_t *parent);

#endif /* __GUI_RANDOM_H */