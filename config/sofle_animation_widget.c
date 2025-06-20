#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <drivers/display.h>
#include <lvgl.h>
#include <zmk/display/widgets/widget.h>

#include "sofle_oled_animation.h"

#define ANIMATION_FRAME_INTERVAL_MS 100

static lv_obj_t *animation_img;
static uint16_t current_frame_idx = 0;
static const struct device *display_dev;

static void update_animation_frame(struct k_work *work) {
    if (!display_dev) {
        display_dev = DEVICE_DT_GET(DT_CHOSEN(zmk_display));
        if (!device_is_ready(display_dev)) {
            // LOG_ERR("Display device not ready");
            return;
        }
    }

    static lv_img_dsc_t animation_img_dsc = {
        .header.always_zero = 0,
        .header.w = 32,  // Largura CORRIGIDA do seu OLED
        .header.h = 128, // Altura CORRIGIDA do seu OLED
        .header.cf = LV_IMG_CF_ALPHA_1BIT,
        .data_size = 32 * 128 / 8, // Tamanho dos dados em bytes
        .data = NULL,
    };

    animation_img_dsc.data = epd_bitmap_allArray[current_frame_idx];

    lv_img_set_src(animation_img, &animation_img_dsc);

    current_frame_idx = (current_frame_idx + 1) % epd_bitmap_allArray_LEN;
}

K_WORK_DEFINE(animation_work, update_animation_frame);

static void animation_timer_handler(struct k_timer *timer) {
    k_work_submit(&animation_work);
}

K_TIMER_DEFINE(animation_timer, animation_timer_handler, NULL);

static int zmk_widget_animation_init(struct zmk_widget_widget *widget) {
    animation_img = lv_img_create(zmk_display_get_obj());
    lv_obj_align(animation_img, LV_ALIGN_CENTER, 0, 0);

    k_timer_start(&animation_timer, K_NO_WAIT, K_MSEC(ANIMATION_FRAME_INTERVAL_MS));

    return 0;
}

static const struct zmk_widget_builder zmk_widget_animation_builder = {
    .init = zmk_widget_animation_init,
};

ZMK_WIDGET_BUILDER(animation_widget, zmk_widget_animation_builder);
