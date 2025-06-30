#include <zmk/display/widgets/key_count_widget.h>
#include <zmk/display/widgets/status.h> // Pode ser necessário para outros widgets, mas não diretamente para este
#include <zmk/event_manager.h>
#include <zmk/events/keycode_state_changed.h>
#include <zmk/events/activity_state_changed.h> // Para saber se o teclado está ativo
#include <zmk/activity.h>
#include <zmk/keymap.h> // Para obter o total de keycodes, se necessário

#include <logging/log.h>
LOG_MODULE_DECLARE(zmk, CONFIG_LOG_DEFAULT_LEVEL);

static int key_press_count = 0; // Nosso contador de teclas

static void key_count_widget_update(struct key_count_widget *widget) {
    char count_str[16];
    snprintk(count_str, sizeof(count_str), "Keys: %d", key_press_count);
    lv_label_set_text(widget->label, count_str);
}

int key_count_widget_init(struct key_count_widget *widget, lv_obj_t *parent) {
    widget->obj = lv_obj_create(parent);
    lv_obj_set_size(widget->obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_add_style(widget->obj, &zmk_display_widget_status_default_style, LV_PART_MAIN);

    widget->label = lv_label_create(widget->obj);
    lv_obj_set_style_text_font(widget->label, &lv_font_montserrat_14, LV_PART_MAIN);
    lv_obj_set_style_text_color(widget->label, lv_color_white(), LV_PART_MAIN);
    lv_obj_center(widget->label);

    key_count_widget_update(widget);
    return 0;
}

struct zmk_widget *key_count_widget_get_obj(struct key_count_widget *widget) {
    return &widget->widget;
}

void key_count_widget_update_key_count(int count) {
    key_press_count = count;
    // Opcional: Você pode querer forçar uma atualização do widget aqui
    // zmk_widget_update_all_widgets(); // Isso pode ser muito agressivo
    // É melhor que o display driver chame o update periodicamente ou em um evento.
}

// Event handler para incrementar o contador
static int key_count_listener(const zmk_event_t *eh) {
    const struct keycode_state_changed *ev = as_keycode_state_changed(eh);
    if (ev == NULL) {
        return ZMK_EV_EVENT_MANAGER_NO_ACTION;
    }

    if (ev->state) { // Se a tecla foi pressionada (não solta)
        key_press_count++;
        // Forçar atualização do widget
        key_count_widget_update(&((struct key_count_widget *)zmk_widget_get_widget_by_type(ZMK_WIDGET_TYPE_KEY_COUNT))->widget);
    }
    return ZMK_EV_EVENT_MANAGER_CONTINUE;
}

ZMK_LISTENER(key_count_widget, key_count_listener);
ZMK_SUBSCRIPTION(key_count_widget, keycode_state_changed);