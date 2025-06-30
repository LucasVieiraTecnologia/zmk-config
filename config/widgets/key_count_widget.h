#pragma once

#include <zmk/display/widget.h>

struct key_count_widget {
    struct zmk_widget widget;
    lv_obj_t *obj;
    lv_obj_t *label;
};

int key_count_widget_init(struct key_count_widget *widget, lv_obj_t *parent);
struct zmk_widget *key_count_widget_get_obj(struct key_count_widget *widget);
void key_count_widget_update_key_count(int count);