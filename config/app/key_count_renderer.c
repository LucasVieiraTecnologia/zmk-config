#include <stdio.h>
#include <zephyr/kernel.h>
#include <zmk/display.h>
#include <zmk/event_manager.h>
#include <zmk/events/keycode-state-changed.h>

static int key_count = 0;

static void update_key_count(const struct keycode_state_changed *ev) {
    if (ev->state) {
        key_count++;
        zmk_display_force_refresh();  // força atualização do OLED
    }
}

ZMK_EVENT_IMPL(keycode_state_changed);

int keycode_state_changed_listener(const struct zmk_event_header *eh) {
    const struct keycode_state_changed *ev = cast_keycode_state_changed(eh);
    update_key_count(ev);
    return 0;
}

int zmk_display_render(char *buf, size_t len) {
    return snprintf(buf, len, "Teclas:\n%d", key_count);
}
