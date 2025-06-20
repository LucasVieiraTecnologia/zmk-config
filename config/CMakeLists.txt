# Adiciona o diretório 'config' como um diretório de fontes e includes
# para que o sistema de build Zephyr/ZMK encontre seus arquivos personalizados.

# Adiciona o arquivo do seu widget C (sofle_animation_widget.c) para ser compilado.
# A cláusula `CONFIG_ZMK_DISPLAY` garante que ele só é compilado se o display estiver habilitado.
zephyr_library_sources_ifdef(CONFIG_ZMK_DISPLAY sofle_animation_widget.c)

# Adiciona o diretório atual (config/) como um diretório de include.
# Isso é necessário para que o compilador encontre "sofle_oled_animation.h"
# quando ele é incluído em sofle_animation_widget.c.
zephyr_library_include_directories(${CMAKE_CURRENT_SOURCE_DIR})
