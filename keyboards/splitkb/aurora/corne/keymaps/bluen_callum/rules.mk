RGBLIGHT_ENABLE = yes
RGB_MATRIX_ENABLE = no
OLED_ENABLE = yes
MOUSEKEY_ENABLE = yes
VIA_ENABLE = yes
VIAL_ENABLE = yes
VIALRGB_ENABLE = no


CONVERT_TO=liatris

SRC += oneshot.c

ifeq ($(strip $(OLED_ENABLE)), yes)
    SRC += oled_display.c
endif
