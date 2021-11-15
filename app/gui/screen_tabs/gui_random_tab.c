#include "gui_random_tab.h"
#include <stdlib.h>
#include "rng_user/rng_user.h"
#include "../../app.h"

static lv_obj_t *mbox1;
static lv_obj_t *update_button;
static _gui_label_st min_label;
static _gui_label_st max_label;
static _gui_spinbox_st min_spinbox;
static _gui_spinbox_st max_spinbox;
static _gui_label_st result_label;

static void _gui_init_spinbox(lv_obj_t *parent, lv_coord_t x_ofs,
                            lv_coord_t y_ofs, _gui_spinbox_st *spinbox);
static void _gui_create_labels(lv_obj_t *parent);
static void gui_get_pid_data_screen(void);
static void _gui_create_button(lv_obj_t *parent);
static void _gui_config_buttons(void);
static void update_event_handler(lv_obj_t *obj, lv_event_t event);
static void _gui_get_result(void);
static void _create_err_msg_box(const char *error_message);

void gui_create_random_tab(lv_obj_t *parent)
{
    _gui_init_spinbox(parent, -100, -70, &min_spinbox);
    _gui_init_spinbox(parent, 120, -70, &max_spinbox);
    _gui_create_labels(parent);
    _gui_create_button(parent);
    _gui_config_buttons();
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

static uint8_t _check_range_before_get_random(void)
{
    gui_get_pid_data_screen();
    if (min_spinbox.value > max_spinbox.value) {
        return 1;
    }
    return 0;
}

static void mbox_event_cb(lv_obj_t *obj, lv_event_t evt)
{
    if (evt == LV_EVENT_DELETE) {
        if (obj == mbox1) {
            mbox1 = NULL;
        }
    } else if (evt == LV_EVENT_VALUE_CHANGED) {
        if (obj == mbox1) {
            lv_msgbox_start_auto_close(mbox1, 0);
        }
    }
}

static void _create_err_msg_box(const char *error_message)
{
    static const char * btns[] ={"OK",""};
    mbox1 = lv_msgbox_create(lv_scr_act(), NULL);
    lv_msgbox_set_text(mbox1, error_message);
    lv_msgbox_add_btns(mbox1, btns);
    lv_obj_set_width(mbox1, 200);
    lv_obj_set_event_cb(mbox1, mbox_event_cb);
    lv_obj_align(mbox1, NULL, LV_ALIGN_CENTER, 0, 0);
}

static void update_event_handler(lv_obj_t * obj, lv_event_t event)
{
    uint8_t ret = 0;
    if(event == LV_EVENT_CLICKED) {
        printf("Getting RNG data\n\r");
        ret = _check_range_before_get_random();
        if (ret != 0) {
            printf("Invalid name\n\r");
            _create_err_msg_box("Invalid range. Please insert a valid range");
            return;
        }
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

static void _gui_get_result(void)
{
    uint32_t random;
    char string[10];
    gui_get_pid_data_screen();
    return_random_in_a_range(min_spinbox.value, max_spinbox.value, &random);
    int_to_string(random, 4, string);
    gui_set_text_label(result_label.label, string);
}
