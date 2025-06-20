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

// Define o módulo de log para depuração (opcional, mas útil)
LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

// Ponteiro global para o dispositivo de display (usado internamente pelo ZMK)
static const struct device *display_dev;

// Ponteiro global para a tela LVGL onde os widgets serão desenhados
static lv_obj_t *zmk_display_screen;

// Função de inicialização principal do display do ZMK.
// Esta função será chamada pelo ZMK durante a inicialização do firmware.
int zmk_display_init() {
    LOG_DBG("Inicializando display ZMK personalizado para Sofle");

    // Tenta obter o dispositivo de display especificado no DeviceTree
    display_dev = DEVICE_DT_GET(DT_CHOSEN(zmk_display));

    // Verifica se o dispositivo de display está pronto
    if (!device_is_ready(display_dev)) {
        LOG_ERR("Display device not ready");
        return -ENODEV; // Retorna erro se o display não estiver pronto
    }

    // Liga o blanking do display (torna-o visível)
    int err = display_blanking_off(display_dev);
    if (err) {
        LOG_ERR("Falha ao ligar o display: %d", err);
        return err;
    }

    // Cria a tela principal do LVGL.
    // É importante criar uma tela LVGL, pois todos os widgets são filhos dela.
    if (zmk_display_screen == NULL) {
        zmk_display_screen = lv_obj_create(NULL);
    }
    // Carrega a tela recém-criada como a tela ativa do LVGL.
    lv_scr_load(zmk_display_screen);

    // Inicializa e adiciona seu widget de animação à tela.
    // O NULL é passado porque o seu zmk_widget_animation_init não usa o argumento 'widget'.
    zmk_widget_animation_init(NULL);

    // Se você tiver outros widgets padrão do ZMK que deseja exibir junto com a animação,
    // ou em outras camadas/estados, você os inicializaria e posicionaria aqui.
    // Por exemplo, para um widget de status de bateria:
    // zmk_widget_battery_status_init(&my_battery_widget_instance);
    // zmk_widget_battery_status_set_parent(&my_battery_widget_instance, zmk_display_screen);
    // lv_obj_align(zmk_widget_battery_status_get_obj(&my_battery_widget_instance), LV_ALIGN_TOP_RIGHT, 0, 0);

    LOG_DBG("Display ZMK personalizado inicializado com animação.");
    return 0; // Retorna 0 para indicar sucesso
}
