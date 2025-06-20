# CMakeLists.txt para seu mapa de teclas Sofle personalizado

# Certifique-se de que os arquivos de origem C para sua lógica de exibição personalizada sejam compilados.
target_sources(zmk_keymap PRIVATE
    sofle.keymap
    display_gif.c
    quadros_gif.h # <--- Use o nome correto do seu arquivo
)
