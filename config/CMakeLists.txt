# zephyr_library_sources_ifdef(CONFIG_ZMK_DISPLAY sofle_animation_widget.c display_manager.c)
# Altere para:
zephyr_library_sources_ifdef(CONFIG_ZMK_DISPLAY) # Lista vazia ou remova os arquivos customizados
# Se você não tiver outros arquivos .c personalizados que não são do ZMK,
# você pode até remover a linha 'zephyr_library_sources_ifdef' por completo.

zephyr_library_include_directories(${CMAKE_CURRENT_SOURCE_DIR})
