/*
This is the c configuration file for the keymap

Copyright 2012 Jun Wako <wakojun@gmail.com>
Copyright 2015 Jack Humbert

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

// #define USE_MATRIX_I2C

/* Select hand configuration */

#define MASTER_LEFT
// #define MASTER_RIGHT
// #define EE_HANDS

// #define QUICK_TAP_TERM 0
// #define TAPPING_TERM 100

#ifdef RGBLIGHT_ENABLE
#    define RGBLIGHT_LAYERS
#    define RGBLIGHT_EFFECT_BREATHING
#    define RGBLIGHT_EFFECT_RAINBOW_MOOD
#    undef RGBLIGHT_EFFECT_RAINBOW_SWIRL
#    undef RGBLIGHT_EFFECT_SNAKE
#    undef RGBLIGHT_EFFECT_KNIGHT
#    undef RGBLIGHT_EFFECT_CHRISTMAS
#    define RGBLIGHT_EFFECT_STATIC_GRADIENT
#    undef RGBLIGHT_EFFECT_RGB_TEST
#    define RGBLIGHT_EFFECT_ALTERNATING
#    undef RGBLIGHT_EFFECT_TWINKLE
// #define RGBLIGHT_LIMIT_VAL 120
#    define RGBLIGHT_HUE_STEP 10
#    define RGBLIGHT_SAT_STEP 17
#    define RGBLIGHT_VAL_STEP 17
#    define RGBLIGHT_LAYERS_RETAIN_VAL
#    define RGBLIGHT_SLEEP
#    define RGBLIGHT_DEFAULT_VAL 128
#endif

#define SPLIT_LAYER_STATE_ENABLE
#define SPLIT_LED_STATE_ENABLE
#undef RGBLED_SPLIT
#define RGBLIGHT_SPLIT
#define RGBLED_SPLIT \
    { 21, 21 }


#define LED_CAPS_LOCK_PIN 24
#define LED_PIN_ON_STATE 0

#define VIAL_KEYBOARD_UID {0x89, 0x87, 0xB7, 0x0F, 0x54, 0x23, 0xC2, 0xE8}

#define VIAL_UNLOCK_COMBO_ROWS { 0, 4 }
#define VIAL_UNLOCK_COMBO_COLS { 5, 5 }

#define DYNAMIC_KEYMAP_LAYER_COUNT 8

