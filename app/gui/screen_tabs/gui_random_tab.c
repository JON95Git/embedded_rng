#include "gui_random_tab.h"
#include <stdlib.h>

static lv_obj_t *update_button;
static _gui_label_st min_label;
static _gui_label_st max_label;
static _gui_spinbox_st min_spinbox;
static _gui_spinbox_st max_spinbox;
static _gui_label_st result_label;

static void _gui_init_spinbox(lv_obj_t *parent, lv_coord_t x_ofs,
                            lv_coord_t y_ofs, _gui_spinbox_st *spinbox);
static void _gui_create_labels(lv_obj_t *parent);
static void gui_set_init_parameters(void);
static void gui_get_pid_data_screen(void);
static void gui_set_pid_data_screen(void);
static void _gui_create_button(lv_obj_t *parent);
static void _gui_config_buttons(void);
static void update_event_handler(lv_obj_t *obj, lv_event_t event);
static void _gui_get_result(void);

void gui_create_random_tab(lv_obj_t *parent)
{
    _gui_init_spinbox(parent, -100, -70, &min_spinbox);
    _gui_init_spinbox(parent, 120, -70, &max_spinbox);
    _gui_create_labels(parent);
    _gui_create_button(parent);
    _gui_config_buttons();
    // gui_set_init_parameters();
}

static void _gui_create_param_label(lv_obj_t *parent, const char *string,
                    lv_coord_t x_ofs, lv_coord_t y_ofs, _gui_label_st *label_st)
{
    *label_st = (_gui_label_st){.parent = parent, 
                                .text = string, 
                                .align = LV_ALIGN_CENTER, 
                                .x_ofs = x_ofs, 
                                .y_ofs = y_ofs};
    label_st->label  = gui_create_label(label_st);
}

static void _gui_init_spinbox(lv_obj_t *parent, lv_coord_t x_ofs,
                lv_coord_t y_ofs, _gui_spinbox_st *spinbox)
{
    spinbox->x_ofs = x_ofs;
    spinbox->y_ofs = y_ofs;
    gui_create_spinbox(parent, spinbox);
}

static void _gui_create_labels(lv_obj_t *parent)
{
    _gui_create_param_label(parent, "Min", -200, -70, &min_label);
    _gui_create_param_label(parent, "Max", 20, -70, &max_label);
    _gui_create_param_label(parent, "0", 0, 60, &result_label);
}

static void update_event_handler(lv_obj_t * obj, lv_event_t event)
{
    if(event == LV_EVENT_CLICKED) {
        printf("Updating data on screen...\n\r");
        _gui_get_result();
    }
}

static void _gui_create_button(lv_obj_t *parent)
{
    lv_obj_t *button_label;

    update_button = lv_btn_create(parent, NULL);
    lv_obj_set_event_cb(update_button, update_event_handler);
    lv_obj_align(update_button, NULL, LV_ALIGN_CENTER, 0, 0);

    button_label = lv_label_create(update_button, NULL);
    lv_label_set_text(button_label, "Get random");
}

static void _gui_config_buttons(void)
{
    lv_obj_set_style_local_bg_color(update_button,
                    LV_OBJ_PART_MAIN, LV_BTN_STATE_PRESSED, USER_COLOR);
    lv_obj_set_style_local_bg_color(update_button,
                    LV_OBJ_PART_MAIN, LV_BTN_STATE_RELEASED, USER_COLOR);
    lv_obj_set_style_local_text_color(update_button,
                    LV_OBJ_PART_MAIN, LV_BTN_STATE_PRESSED, LV_COLOR_WHITE);
    lv_obj_set_style_local_text_color(update_button,
                    LV_OBJ_PART_MAIN, LV_BTN_STATE_RELEASED, LV_COLOR_WHITE);
}

static void gui_get_pid_data_screen(void)
{
    gui_get_spinbox_value(min_spinbox.spinbox, &min_spinbox.value);
    gui_get_spinbox_value(max_spinbox.spinbox, &max_spinbox.value);
}

static void gui_set_pid_data_screen(void)
{
    gui_set_spinbox_value(min_spinbox.spinbox, min_spinbox.value);
    gui_set_spinbox_value(max_spinbox.spinbox, max_spinbox.value);
}

static void gui_set_init_parameters(void)
{
#ifdef EMBEDDED
    // pid_get_parameters(&min_spinbox.value, &ki_spinbox.value,
    // &max_spinbox.value, &sp_spinbox.value);
#endif
    gui_set_pid_data_screen();
}

static void _return_random(uint32_t min, uint32_t max, uint32_t *random)
{
    *random = (rand() % (max - min + 1)) + min;
}

static void _gui_get_result(void)
{
    uint32_t random;
    char string[10];
    gui_get_pid_data_screen();
    _return_random(min_spinbox.value, max_spinbox.value, &random);
    int_to_string(random, 4, string);
    gui_set_text_label(result_label.label, string);
}
