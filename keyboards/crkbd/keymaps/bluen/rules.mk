RGBLIGHT_ENABLE = no
RGB_MATRIX_ENABLE = no
OLED_ENABLE = yes
BOOTMAGIC_ENABLE = yes
BOOTLOADER = atmel-dfu

SRC += oneshot.c

ifeq ($(strip $(OLED_ENABLE)), yes)
    SRC += oled_display.c
endif

VIA_ENABLE          = yes
VIAL_ENABLE         = yes
LTO_ENABLE          = yes
COMBO_ENABLE        = no

QMK_SETTINGS        = no
MOUSEKEY_ENABLE     = no
