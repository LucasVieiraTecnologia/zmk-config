#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <drivers/display.h>
#include <lvgl.h>
#include <zmk/display/widgets/widget.h>
#include <zmk/event_manager.h>
#include <zmk/display.h> // ZMK display core functions

// Inclua o cabeçalho do seu widget de animação
#include "sofle_animation_widget.h"

LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

static const struct device *display_dev;
static lv_obj_t *zmk_display_screen;

// Esta função será chamada pelo ZMK durante a inicialização do firmware.
int zmk_display_init() {
    LOG_DBG("Inicializando display ZMK personalizado para Sofle");

    display_dev = DEVICE_DT_GET(DT_CHOSEN(zmk_display));

    if (!device_is_ready(display_dev)) {
        LOG_ERR("Display device not ready");
        return -ENODEV;
    }

    int err = display_blanking_off(display_dev);
    if (err) {
        LOG_ERR("Falha ao ligar o display: %d", err);
        return err;
    }

    if (zmk_display_screen == NULL) {
        zmk_display_screen = lv_obj_create(NULL);
    }
    lv_scr_load(zmk_display_screen);

    // Inicializa e adiciona seu widget de animação à tela.
    zmk_widget_animation_init(NULL);

    LOG_DBG("Display ZMK personalizado inicializado com animação.");
    return 0;
}
