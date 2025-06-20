# Adiciona o arquivo do seu widget C e o gerenciador de display para serem compilados.
# A cláusula `CONFIG_ZMK_DISPLAY` garante que eles só são compilados se o display estiver habilitado.
zephyr_library_sources_ifdef(CONFIG_ZMK_DISPLAY sofle_animation_widget.c display_manager.c)

# Adiciona o diretório atual (config/) como um diretório de include.
# Isso é necessário para que o compilador encontre "sofle_oled_animation.h" e outros includes.
zephyr_library_include_directories(${CMAKE_CURRENT_SOURCE_DIR})
