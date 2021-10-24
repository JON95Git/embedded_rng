#include <stdio.h>
#include "gui_mass_storage_tab.h"
#include "../components/gui_spinbox.h"

#define USER_COLOR lv_color_hex(0x034885)

static _gui_label_st name_label;
static _gui_label_st size_label;
static _gui_spinbox_st size_spinbox;
static lv_obj_t *start_button;
static lv_obj_t *name_text_area = NULL;
static lv_obj_t *kb = NULL;
static lv_obj_t *mbox1;
static lv_obj_t *mbox2;
static lv_obj_t *main_parent;

static void _gui_create_param_label(lv_obj_t *parent, const char *string,
                lv_coord_t x_ofs, lv_coord_t y_ofs, _gui_label_st *label_pwm_st);
static void ta_event_cb(lv_obj_t * ta, lv_event_t event);

static void bar_event_cb(lv_obj_t *obj, lv_event_t evt)
{
    if (evt == LV_EVENT_VALUE_CHANGED) {
        printf("Bar value changed\n\r");
    }
}

void _create_progress_bar(lv_obj_t *parent)
{
    lv_obj_t * bar1 = lv_bar_create(parent, NULL);
    lv_obj_set_size(bar1, 180, 20);
    lv_obj_align(bar1, NULL, LV_ALIGN_CENTER, 0, 0);
    // lv_bar_set_anim_time(bar1, 2000);
    // lv_bar_set_value(bar1, 100, LV_ANIM_ON);
    lv_bar_set_value(bar1, 10, LV_ANIM_OFF);
    lv_obj_set_event_cb(bar1, bar_event_cb);
}

static void mbox_event_cb(lv_obj_t *obj, lv_event_t evt)
{
    if (evt == LV_EVENT_DELETE) {
        if (obj == mbox1) {
            /* Delete the parent modal background */
            // lv_obj_del_async(lv_obj_get_parent(mbox1));
            mbox1 = NULL; /* happens before object is actually deleted! */
        } else if (obj == mbox2) {
            /* Delete the parent modal background */
            // lv_obj_del_async(lv_obj_get_parent(mbox1));
            mbox2 = NULL; /* happens before object is actually deleted! */
        }
    } else if (evt == LV_EVENT_VALUE_CHANGED) {
        if (obj == mbox1) {
            /* A button was clicked */
            lv_msgbox_start_auto_close(mbox1, 0);
        } else if (obj == mbox2) {
            /* A button was clicked */
            lv_msgbox_start_auto_close(mbox2, 0);
        }
    }
}

void _create_err_msg_box(const char *error_message)
{
    static const char * btns[] ={"OK",""};
    mbox1 = lv_msgbox_create(lv_scr_act(), NULL);
    lv_msgbox_set_text(mbox1, error_message);
    lv_msgbox_add_btns(mbox1, btns);
    lv_obj_set_width(mbox1, 200);
    lv_obj_set_event_cb(mbox1, mbox_event_cb);
    lv_obj_align(mbox1, NULL, LV_ALIGN_CENTER, 0, 0);
}

void _create_save_msg_box(lv_obj_t *parent)
{
    mbox2 = lv_msgbox_create(parent, NULL);
    lv_msgbox_set_text(mbox2, "Saving samples on SD card...");
    lv_obj_set_size(mbox2, 400, 800);
    lv_obj_set_event_cb(mbox2, mbox_event_cb);
    lv_obj_align(mbox2, NULL, LV_ALIGN_CENTER, 0, 0);
}

static int _check_data_before_try_generate_file(void)
{
    int ret;
    const char *string_name;
    string_name = lv_textarea_get_text(name_text_area);
    printf("string_name: %s\n\r", string_name);
    ret = memcmp(string_name, "", 1);
    return ret;
}

static void _gui_init_spinbox(lv_obj_t *parent, lv_coord_t x_ofs,
                lv_coord_t y_ofs, _gui_spinbox_st *spinbox)
{
    spinbox->x_ofs = x_ofs;
    spinbox->y_ofs = y_ofs;
    gui_create_spinbox(parent, spinbox);
}

static void kb_event_cb(lv_obj_t * keyboard, lv_event_t e)
{
    lv_keyboard_def_event_cb(kb, e);
    if (e == LV_EVENT_CANCEL) {
        lv_keyboard_set_textarea(kb, NULL);
        lv_obj_del(kb);
        kb = NULL;
    }
}

void create_keyboard(lv_obj_t *parent)
{
    kb = lv_keyboard_create(parent, NULL);
    lv_obj_set_size(kb,  LV_HOR_RES - 40, LV_VER_RES / 2);
    lv_obj_set_event_cb(kb, kb_event_cb);
    lv_obj_align(kb, NULL, LV_ALIGN_CENTER, 0, 0);
    lv_keyboard_set_textarea(kb, name_text_area);
    lv_keyboard_set_cursor_manage(kb, true);
}

void _create_text_area_name(lv_obj_t *parent)
{
    name_text_area = lv_textarea_create(parent, NULL);
    lv_textarea_set_text(name_text_area, "");
    lv_textarea_set_pwd_mode(name_text_area, false);
    lv_textarea_set_cursor_hidden(name_text_area, true);
#ifndef EMBEDDED
    lv_obj_align(name_text_area, NULL, LV_ALIGN_IN_TOP_RIGHT, -85, 20);
#else
    lv_obj_align(name_text_area, NULL, LV_ALIGN_IN_TOP_RIGHT, -30, 20);
#endif
    lv_obj_set_size(name_text_area, 200, 40);
    lv_obj_set_event_cb(name_text_area, ta_event_cb);
}

static void ta_event_cb(lv_obj_t *ta, lv_event_t event)
{
    if(event == LV_EVENT_CLICKED) {
        /* Focus on the clicked text area */
        if(kb == NULL){
            create_keyboard(ta->parent);
            lv_keyboard_set_textarea(kb, ta);
        }
    } else if(event == LV_EVENT_INSERT) {
        const char * str = lv_event_get_data();
        if(str[0] == '\n') {
            printf("Ready\n");
        }
    }
}

static void _gui_create_labels(lv_obj_t *parent)
{
    _gui_create_param_label(parent, "Name: ", -150, -70, &name_label);
    _gui_create_param_label(parent, "Size: ", -160, -20, &size_label);
}

static void _gui_config_save_button(void)
{
    lv_obj_set_style_local_bg_color(start_button,
                    LV_OBJ_PART_MAIN, LV_BTN_STATE_PRESSED, USER_COLOR);
    lv_obj_set_style_local_bg_color(start_button,
                    LV_OBJ_PART_MAIN, LV_BTN_STATE_RELEASED, USER_COLOR);
    lv_obj_set_style_local_text_color(start_button,
                    LV_OBJ_PART_MAIN, LV_BTN_STATE_PRESSED, LV_COLOR_WHITE);
    lv_obj_set_style_local_text_color(start_button,
                    LV_OBJ_PART_MAIN, LV_BTN_STATE_RELEASED, LV_COLOR_WHITE);
}

static void save_button_event_handler(lv_obj_t * obj, lv_event_t event)
{
    int ret = 0;
    int32_t spinbox_value = 0;
    if (event == LV_EVENT_CLICKED) {
        ret = _check_data_before_try_generate_file();
        if (ret == 0) {
            printf("Invalid name\n\r");
            _create_err_msg_box("Invalid name. Please insert a valid name");
            return;
        }
        gui_get_spinbox_value(size_spinbox.spinbox, &spinbox_value);
        if (spinbox_value == 0) {
            printf("Invalid file size\n\r");
            _create_err_msg_box("Invalid file size. Please insert a valid file size");
            return;
        }
        printf("Saving samples on SD card...\n\r");
        _create_save_msg_box(main_parent);
        _create_progress_bar(mbox2);
        // open a box dialog with X icon
        // _gui_get_result();
    }
}

static void _gui_create_save_button(lv_obj_t *parent)
{
    lv_obj_t *button_label;

    start_button = lv_btn_create(parent, NULL);
    lv_obj_set_event_cb(start_button, save_button_event_handler);
    lv_obj_align(start_button, NULL, LV_ALIGN_CENTER, 0, 50);

    button_label = lv_label_create(start_button, NULL);
    lv_label_set_text(button_label, "Save");
}

void gui_create_mass_storage_tab(lv_obj_t *parent)
{
    _gui_create_labels(parent);
    _gui_create_save_button(parent);
    _gui_config_save_button();
    _create_text_area_name(parent);
    _gui_init_spinbox(parent, -40, -20, &size_spinbox);
    main_parent = parent;
}

static void _gui_create_param_label(lv_obj_t *parent, const char *string,
                lv_coord_t x_ofs, lv_coord_t y_ofs, _gui_label_st *label_pwm_st)
{
    *label_pwm_st = (_gui_label_st){.parent = parent, 
                                .text = string, 
                                .align = LV_ALIGN_CENTER, 
                                .x_ofs = x_ofs, 
                                .y_ofs = y_ofs};

    label_pwm_st->label  = gui_create_label(label_pwm_st);
}