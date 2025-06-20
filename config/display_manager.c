#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <drivers/display.h>
#include <lvgl.h>
#include <zmk/display/widgets/widget.h>
#include <zmk/event_manager.h>
#include <zmk/display.h>
#include <zephyr/logging/log.h> // <--- ADICIONE ESTA LINHA para habilitar o log

// Inclua o cabeçalho do seu widget de animação
#include "sofle_oled_animation.h"

// Registre um módulo de log para o seu código customizado
// (Altere 'zmk' para 'zmk_custom_display' para evitar conflitos se zmk já estiver declarado)
LOG_MODULE_REGISTER(zmk_custom_display, CONFIG_ZMK_LOG_LEVEL); // <--- ALTERE/ADICIONE ESTA LINHA

static const struct device *display_dev;
static lv_obj_t *zmk_display_screen;

int zmk_display_custom_init() {
    // <--- ADICIONE ESTA LINHA NO INÍCIO DA FUNÇÃO
    LOG_INF("!!!!!!! ZMK Custom Display Init Called !!!!!!!");

    display_dev = DEVICE_DT_GET(DT_CHOSEN(zmk_display));

    // Adicione uma verificação de ponteiro nulo para o dispositivo
    if (!display_dev) {
        LOG_ERR("ZMK Display Device not found!");
        return -ENODEV;
    }

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

    zmk_widget_animation_init(NULL);

    // <--- ADICIONE ESTA LINHA NO FIM DA FUNÇÃO
    LOG_INF("!!!!!!! ZMK Custom Display Init Finished !!!!!!!");
    return 0;
}
