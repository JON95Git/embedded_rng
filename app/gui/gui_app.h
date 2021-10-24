#ifndef __GUI_APP_H
#define __GUI_APP_H

#ifdef EMBEDDED
#include "lvgl.h"
#include "gui_mass_storage_tab.h"
#include "gui_random_tab.h"
#else
#include "lvgl/lvgl.h"
#include "screen_tabs/gui_mass_storage_tab.h"
#include "screen_tabs/gui_random_tab.h"
#endif
#include "components/gui_widgets.h"

typedef struct {
    lv_group_t *group;
    lv_obj_t *tabview;
    lv_obj_t *tab_random;
    lv_obj_t *tab_mass_storage;
} gui_app_st;

void gui_app(void);

#endif /* __GUI_APP_H */