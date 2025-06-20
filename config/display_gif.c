#include <zephyr/kernel.h>
#include <zmk/display.h>
#include <zmk/activity.h>
#include <zmk/event_manager.h>
#include <zmk/events/activity_state_changed.h>

// Inclua o arquivo com os seus dados de frames do GIF
// Este arquivo DEVE estar SEPARADO e se chamar "quadros_gif.h"
#include "quadros_gif.h" // <--- CONFIRME ESTE NOME (quadros_gif.h)

#define GIF_UPDATE_INTERVAL_MS 150
#define IDLE_TIMEOUT_MS CONFIG_ZMK_IDLE_TIMEOUT

static uint8_t current_gif_frame_index = 0;
static bool gif_should_play = false;
static struct k_timer gif_animation_timer;

static void draw_gif_frame(const uint8_t *frame_data) {
    zmk_display_clear();
    zmk_display_buffer_draw(frame_data, 0, 0, 128, 32, false); // false = não invertido
    zmk_display_update();
}

void gif_animation_timer_handler(struct k_timer *timer_id) {
    if (gif_should_play) {
        draw_gif_frame(epd_bitmap_allArray[current_gif_frame_index]);
        current_gif_frame_index++;
        if (current_gif_frame_index >= epd_bitmap_allArray_LEN) {
            current_gif_frame_index = 0;
        }
    } else {
        zmk_display_clear();
        zmk_display_update();
        LOG_DBG("Keyboard ACTIVE, clearing GIF display.");
    }
}

static int activity_state_listener(const zmk_event_t *eh) {
    const struct zmk_activity_state_changed *ev = as_zmk_activity_state_changed(eh);
    if (ev == NULL) {
        return -ENOTSUP;
    }
    if (ev->state == ZMK_ACTIVITY_ACTIVE) {
        gif_should_play = false;
        current_gif_frame_index = 0;
        zmk_display_clear();
        zmk_display_update();
        LOG_DBG("Keyboard ACTIVE, stopping GIF animation.");
    } else {
        gif_should_play = true;
        LOG_DBG("Keyboard IDLE, starting GIF animation.");
    }
    return 0;
}
ZMK_LISTENER(activity_state_listener, activity_state_listener);
ZMK_SUBSCRIPTION(activity_state_listener, zmk_activity_state_changed);

static int init_gif_display(void) {
    k_timer_init(&gif_animation_timer, gif_animation_timer_handler, NULL);
    k_timer_start(&gif_animation_timer, K_MSEC(GIF_UPDATE_INTERVAL_MS), K_MSEC(GIF_UPDATE_INTERVAL_MS));
    LOG_DBG("GIF display module initialized.");
    return 0;
}
SYS_INIT(init_gif_display, APPLICATION, CONFIG_APPLICATION_INIT_PRIORITY);
