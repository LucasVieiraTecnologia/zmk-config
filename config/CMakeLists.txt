# CMakeLists.txt para o layout personalizado Sofle
# Define opções Kconfig adicionais e inclui arquivos fonte.

# Definições Kconfig forçadas para superar "undefined symbol" warnings.
# Isso garante que esses símbolos sejam definidos no escopo do build.
set(EXTRA_KCONFIG_OPTIONS
    "CONFIG_ZMK_DISPLAY_STATUS=y"
    "CONFIG_ZMK_ACTIVITY=y"
    "CONFIG_ZMK_LOG_LEVEL_DBG=y"
    "CONFIG_ZMK_DISPLAY_LOG_LEVEL_DBG=y"
    "CONFIG_ZMK_ACTIVITY_LOG_LEVEL_DBG=y"
    # Adicione aqui outras flags CONFIG_ que possam dar "undefined symbol" no futuro
)

# Inclui os arquivos fonte C para o seu módulo de display
target_sources(zmk_keymap PRIVATE
    sofle.keymap    # Seu arquivo de keymap principal
    display_gif.c   # Seu código de lógica do display
    quadros_gif.h   # Seus dados de frames do GIF
)
