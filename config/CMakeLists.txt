# CMakeLists.txt for your custom Sofle keymap

# Ensure that the Kconfig symbols for display status and activity are defined.
# This is a fallback if default ZMK Kconfig isn't defining them for some reason.
set(EXTRA_KCONFIG_OPTIONS
    "CONFIG_ZMK_DISPLAY_STATUS=y"
    "CONFIG_ZMK_ACTIVITY=y"
    "CONFIG_ZMK_LOG_LEVEL_DBG=y"
    "CONFIG_ZMK_DISPLAY_LOG_LEVEL_DBG=y"
    "CONFIG_ZMK_ACTIVITY_LOG_LEVEL_DBG=y"
    PARENT_SCOPE
)

# Ensure the C source files for your custom display logic are compiled.
# Replace 'display_gif.c' and 'gif_frames.h' with your actual file names.
target_sources(zmk_keymap PRIVATE
    sofle.keymap # Your main keymap file
    display_gif.c # Your custom display logic
    gif_frames.h  # Your GIF data header (often included by display_gif.c, but good to list)
)
