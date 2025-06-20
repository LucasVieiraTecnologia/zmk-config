#include <zephyr/kernel.h>      // Para k_timer, k_uptime_get_32, K_MSEC
#include <zmk/display.h>        // Para zmk_display_clear, zmk_display_buffer_draw, zmk_display_update
#include <zmk/activity.h>       // Para zmk_activity_get_idle_time_ms
#include <zmk/event_manager.h>  // Para zmk_event_t, ZMK_LISTENER, ZMK_SUBSCRIPTION
#include <zmk/events/activity_state_changed.h> // Para o evento de mudança de estado de atividade

// Inclua o arquivo com os seus dados de frames do GIF
#include "gif_frames.h" // <--- Este include é crucial!

// Define a taxa de atualização do GIF em milissegundos (ex: 150ms = ~6.6 FPS)
#define GIF_UPDATE_INTERVAL_MS 150
// Tempo em milissegundos para o teclado ser considerado "ocioso" e o GIF começar
#define IDLE_TIMEOUT_MS CONFIG_ZMK_IDLE_TIMEOUT

// Variável para rastrear o índice do quadro atual do GIF
static uint8_t current_gif_frame_index = 0;
// Flag para indicar se o GIF deve ser reproduzido (ocioso) ou não (ativo)
static bool gif_should_play = false;
// Timer para controlar a animação do GIF
static struct k_timer gif_animation_timer;

// Função para desenhar um quadro na tela OLED
static void draw_gif_frame(const uint8_t *frame_data) {
    // Limpa a tela OLED antes de desenhar o novo quadro
    zmk_display_clear();

    // Desenha o bitmap do quadro atual na tela
    // Parâmetros:
    // 1. frame_data: Ponteiro para o array de bytes do quadro
    // 2. x: Coordenada X inicial (0 para o canto esquerdo da tela)
    // 3. y: Coordenada Y inicial (0 para o topo da tela)
    // 4. width: Largura da imagem (128 pixels para o Sofle OLED)
    // 5. height: Altura da imagem (32 pixels para o Sofle OLED)
    // 6. inverted: Se true, inverte as cores (false para manter as cores como no bitmap)
    zmk_display_buffer_draw(frame_data, 0, 0, 128, 32, false);

    // Atualiza o display para que o novo quadro seja visível
    zmk_display_update();
}

// Callback do timer que é chamado periodicamente para avançar o quadro do GIF
void gif_animation_timer_handler(struct k_timer *timer_id) {
    if (gif_should_play) {
        // Se o GIF deve estar tocando, desenha o próximo quadro
        draw_gif_frame(epd_bitmap_allArray[current_gif_frame_index]);

        // Avança para o próximo quadro, reiniciando se chegar ao final
        current_gif_frame_index++;
        if (current_gif_frame_index >= epd_bitmap_allArray_LEN) {
            current_gif_frame_index = 0; // Reinicia a animação do GIF
        }
    } else {
        // Se o GIF não deve estar tocando, garante que o display está limpo
        // ou mostrando o conteúdo padrão (pode ser ajustado aqui)
        zmk_display_clear();
        zmk_display_update();
        // Opcional: Se você tiver um "widget" de status padrão (tipo layer/bateria)
        // você pode tentar reativá-lo aqui.
        // zmk_display_enable_widgets(true);
        // LOG_DBG("Keyboard ACTIVE, clearing GIF display."); // Isso pode ser muito log
    }
}

// Listener de eventos que reage a mudanças no estado de atividade do teclado
static int activity_state_listener(const zmk_event_t *eh) {
    const struct zmk_activity_state_changed *ev = as_zmk_activity_state_changed(eh);
    if (ev == NULL) {
        return -ENOTSUP;
    }

    if (ev->state == ZMK_ACTIVITY_ACTIVE) {
        // Teclado está ativo (digitando ou foi tocado recentemente)
        gif_should_play = false;
        // Reinicia o índice do GIF para que ele comece do zero na próxima vez que ficar ocioso
        current_gif_frame_index = 0;
        // Limpa o display imediatamente ou exibe o status padrão
        zmk_display_clear();
        zmk_display_update();
        //zmk_display_enable_widgets(true); // Se você usa widgets padrão
        LOG_DBG("Keyboard ACTIVE, stopping GIF animation.");
    } else { // ZMK_ACTIVITY_IDLE (teclado está ocioso)
        // Teclado está ocioso, começa a reproduzir o GIF
        gif_should_play = true;
        LOG_DBG("Keyboard IDLE, starting GIF animation.");
    }
    return 0;
}

// Registra o listener de evento para as mudanças de estado de atividade
ZMK_LISTENER(activity_state_listener, activity_state_listener);
ZMK_SUBSCRIPTION(activity_state_listener, zmk_activity_state_changed);

// Função de inicialização do módulo, chamada durante a inicialização do ZMK
static int init_gif_display(void) {
    // Inicializa o timer do GIF
    // gif_animation_timer_handler é o callback, NULL para nenhum argumento extra
    k_timer_init(&gif_animation_timer, gif_animation_timer_handler, NULL);

    // Inicia o timer para que ele chame o handler a cada GIF_UPDATE_INTERVAL_MS
    k_timer_start(&gif_animation_timer, K_MSEC(GIF_UPDATE_INTERVAL_MS), K_MSEC(GIF_UPDATE_INTERVAL_MS));

    LOG_DBG("GIF display module initialized.");
    return 0;
}

// Registra a função de inicialização para ser chamada no nível APPLICATION
SYS_INIT(init_gif_display, APPLICATION, CONFIG_APPLICATION_INIT_PRIORITY);
