#ifndef __GUI_MASS_STORAGE_TAB_H
#define __GUI_MASS_STORAGE_TAB_H

#ifdef EMBEDDED
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif
#include "../components/gui_widgets.h"

void gui_create_mass_storage_tab(lv_obj_t *parent);

#endif /* __GUI_MASS_STORAGE_TAB_H */