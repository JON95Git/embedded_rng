#include "gui_app.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static gui_app_st gui;
static void gui_create_tabview_screen(void);

static void _seed_rng(void)
{
    time_t t;
    srand((unsigned) time(&t));
}

void gui_app(void)
{
    _seed_rng();
    gui_create_tabview_screen();
    gui_create_mass_storage_tab(gui.tab_mass_storage);
    gui_create_random_tab(gui.tab_random);
}

static void gui_create_tabview_screen(void)
{
    gui.tabview = lv_tabview_create(lv_scr_act(), NULL);
    gui.tab_mass_storage = lv_tabview_add_tab(gui.tabview, LV_SYMBOL_SD_CARD" Mass storage");
    gui.tab_random = lv_tabview_add_tab(gui.tabview, LV_SYMBOL_SHUFFLE" Random");
    lv_obj_set_style_local_pad_bottom(gui.tabview, LV_TABVIEW_PART_TAB_BTN, LV_STATE_DEFAULT, 1);
    lv_obj_set_style_local_text_color(gui.tabview, LV_TABVIEW_PART_TAB_BTN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_obj_set_style_local_text_color(gui.tabview, LV_TABVIEW_PART_TAB_BTN, LV_STATE_CHECKED, LV_COLOR_WHITE);
    lv_obj_set_style_local_text_color(gui.tabview, LV_TABVIEW_PART_TAB_BG, LV_STATE_CHECKED, LV_COLOR_WHITE);
    lv_obj_set_style_local_bg_color(gui.tabview, LV_TABVIEW_PART_TAB_BG, LV_STATE_DEFAULT, USER_COLOR);
}
