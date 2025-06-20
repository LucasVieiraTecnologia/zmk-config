#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <drivers/display.h>
#include <lvgl.h>
#include <zmk/display/widgets/widget.h> // Se você tiver uma pasta widgets, use zmk/display/widgets/widget.h

// Inclua seu arquivo de imagens. O caminho é relativo à pasta 'config' se ele estiver lá.
// Se você moveu para config/sofle/images/sofle_oled_animation.h
// #include "images/sofle_oled_animation.h"
// Se está em config/sofle_oled_animation.h (como na sua imagem), o caminho é mais simples:
#include "sofle_oled_animation.h"

// Defina o intervalo entre os quadros da animação em milissegundos
// 100ms = 10 quadros por segundo (fps)
#define ANIMATION_FRAME_INTERVAL_MS 100

static lv_obj_t *animation_img; // Objeto LVGL para a imagem
static uint16_t current_frame_idx = 0; // Índice do quadro atual na animação

// Forward declaration do display device (será inicializado no update_animation_frame)
static const struct device *display_dev;

// Função de trabalho (work handler) para atualizar o frame da animação
static void update_animation_frame(struct k_work *work) {
    if (!display_dev) {
        display_dev = DEVICE_DT_GET(DT_CHOSEN(zmk_display));
        if (!device_is_ready(display_dev)) {
            // LOG_ERR("Display device not ready"); // Descomente para depuração
            return;
        }
    }

    // Estrutura LVGL para descrever o bitmap atual
    static lv_img_dsc_t animation_img_dsc = {
        .header.always_zero = 0,
        .header.w = 128, // Largura do seu OLED (verifique se é 128x32 ou 128x64)
        .header.h = 32,  // Altura do seu OLED (se for 128x32)
        .header.cf = LV_IMG_CF_ALPHA_1BIT, // Formato de cor: 1 bit por pixel (monocromático)
        .data_size = 128 * 32 / 8, // Tamanho dos dados em bytes (128*32 pixels / 8 bits por byte)
        .data = NULL, // Será preenchido com o ponteiro para o quadro atual
    };

    // Atualiza o ponteiro de dados para o quadro atual
    animation_img_dsc.data = epd_bitmap_allArray[current_frame_idx];

    // Define a fonte da imagem para o objeto LVGL de imagem
    lv_img_set_src(animation_img, &animation_img_dsc);

    // Incrementa o índice do quadro e volta para o início se chegar ao fim
    current_frame_idx = (current_frame_idx + 1) % epd_bitmap_allArray_LEN;

    // O display é atualizado automaticamente pelo ZMK/LVGL quando há mudanças
    // display_blanking_off(display_dev); // Pode ser necessário se o display estiver desligado
}

// Definição do work queue para a atualização do frame
K_WORK_DEFINE(animation_work, update_animation_frame);

// Handler do timer para agendar a atualização dos frames
static void animation_timer_handler(struct k_timer *timer) {
    k_work_submit(&animation_work);
}

// Definição do timer
K_TIMER_DEFINE(animation_timer, animation_timer_handler, NULL);


// Função de inicialização do widget da animação ZMK
static int zmk_widget_animation_init(struct zmk_widget_widget *widget) {
    // Cria o objeto de imagem LVGL e o anexa à tela principal do ZMK
    animation_img = lv_img_create(zmk_display_get_obj());
    lv_obj_align(animation_img, LV_ALIGN_CENTER, 0, 0); // Centraliza a animação na tela

    // Inicia o timer da animação
    // O K_NO_WAIT faz com que o primeiro frame seja desenhado imediatamente
    // O K_MSEC(ANIMATION_FRAME_INTERVAL_MS) define o período de repetição
    k_timer_start(&animation_timer, K_NO_WAIT, K_MSEC(ANIMATION_FRAME_INTERVAL_MS));

    return 0;
}

// Construtor do widget ZMK
static const struct zmk_widget_builder zmk_widget_animation_builder = {
    .init = zmk_widget_animation_init,
};

// Registra o widget no sistema ZMK
ZMK_WIDGET_BUILDER(animation_widget, zmk_widget_animation_builder);
