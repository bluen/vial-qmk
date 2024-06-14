/*
Copyright 2019 @foostan
Copyright 2020 Drashna Jaelre <@drashna>

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

#include QMK_KEYBOARD_H

#include "oneshot.h"

#define LA_LOW MO(1)
#define LA_RSE MO(2)
#define LA_ADJ MO(3)
#define SPC_L A(G(KC_LEFT))
#define SPC_R A(G(KC_RGHT))
#define HOME G(KC_LEFT)
#define END G(KC_RGHT)
#define FWD G(KC_RBRC)
#define BACK G(KC_LBRC)
#define TABL G(S(KC_LBRC))
#define TABR G(S(KC_RBRC))

typedef union {
    uint32_t raw;
    struct {
        bool rgb_layer_change : 1;
        bool caps_lock_on : 1;
    };
} user_config_t;

user_config_t user_config;

oneshot_state os_shft_state = OS_UP_UNQUEUED;
oneshot_state os_ctrl_state = OS_UP_UNQUEUED;
oneshot_state os_alt_state  = OS_UP_UNQUEUED;
oneshot_state os_cmd_state  = OS_UP_UNQUEUED;

// const rgblight_segment_t PROGMEM capslock_layer[] = RGBLIGHT_LAYER_SEGMENTS({3, 3, HSV_RED}, {9, 3, HSV_RED});
#ifdef RGBLIGHT_ENABLE
const rgblight_segment_t PROGMEM oneshot_layer[] = RGBLIGHT_LAYER_SEGMENTS({0, 21, HSV_RED}, {22, 21, HSV_RED});

const rgblight_segment_t PROGMEM base_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {1, 1, HSV_RED}, {2, 1, HSV_GREEN}, {18, 1, HSV_OFF}, {19,1, HSV_RED}, {20,1, HSV_ORANGE},
    {22, 1, HSV_RED}, {23, 1, HSV_GREEN}, {39, 1, HSV_RED}, {41, 1, HSV_ORANGE}
);

const rgblight_segment_t PROGMEM lower_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {3, 3, HSV_CYAN}, {6, 2, HSV_YELLOW}, {8, 1, HSV_PINK}, {9, 4, HSV_RED}, {13, 1, HSV_PINK}, {14, 2, HSV_YELLOW}, {16, 3, HSV_OFF}, {1, HSV_RED}, {20,1, HSV_ORANGE},
    {22, 1, HSV_RED}, {23, 1, HSV_GREEN}, {24, 1, HSV_OFF}, {39, 1, HSV_RED}, {40, 1, HSV_OFF}, {41, 1, HSV_ORANGE},
    {25, 2, HSV_YELLOW}, {27, 2, HSV_OFF}, {29, 1, HSV_RED}, {30, 3, HSV_GREEN}, {33, 1, HSV_ORANGE}, {34, 1, HSV_CYAN}, {35, 1, HSV_ORANGE}, {36, 1, HSV_GREEN}, {37, 1, HSV_ORANGE}, {38, 1, HSV_RED}
);

const rgblight_segment_t PROGMEM raise_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {4, 1, HSV_RED}, {5, 2, HSV_GREEN}, {7, 1, HSV_RED}, {8, 10, HSV_ORANGE},  {18, 1, HSV_OFF}, {19,1, HSV_RED}, {20,1, HSV_ORANGE},
    {24, 1, HSV_GREEN}, {25, 1, HSV_RED}, {26, 2, HSV_CYAN}, {28, 1, HSV_YELLOW},
    {29, 1, HSV_GREEN}, {30, 4, HSV_RED}, {34, 5, HSV_ORANGE}, {39, 1, HSV_RED}, {40, 1, HSV_OFF}, {41, 1, HSV_ORANGE}
);

const rgblight_segment_t PROGMEM adj_layer[] = RGBLIGHT_LAYER_SEGMENTS({0, 42, HSV_YELLOW});

const rgblight_segment_t *const PROGMEM my_rgb_layers[] = RGBLIGHT_LAYERS_LIST(base_layer, lower_layer, raise_layer, adj_layer, oneshot_layer);
#endif

void keyboard_post_init_user(void) {
    user_config.raw = 0; // eeconfig_read_user();
#ifdef OLED_ENABLE
    oled_set_brightness(64);
#endif

#ifdef RGBLIGHT_ENABLE
    rgblight_layers = my_rgb_layers;

    // Read the user config from EEPROM
    user_config.rgb_layer_change = true;

    // Set default layer, if enabled
    if (user_config.rgb_layer_change) {
        rgblight_enable_noeeprom();
        rgblight_sethsv_noeeprom(180, 255, 128);
        rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);
    }
#endif
}

enum layer_names { _BASE, _LOWER, _RAISE, _ADJUST, _ONESHOT };

bool led_update_user(led_t led_state) {
    user_config.caps_lock_on = led_state.caps_lock;
    return true;
}

#ifdef RGBLIGHT_ENABLE
layer_state_t default_layer_state_set_user(layer_state_t state) {
    rgblight_set_layer_state(_BASE, layer_state_cmp(state, _BASE));
    return state;
}

layer_state_t layer_state_set_user(layer_state_t state) {
    if (os_shft_state == OS_UP_QUEUED || os_ctrl_state == OS_UP_QUEUED || os_alt_state == OS_UP_QUEUED || os_cmd_state == OS_UP_QUEUED || user_config.caps_lock_on) {
        rgblight_set_layer_state(_ONESHOT, true);
    } else {
        rgblight_set_layer_state(_ONESHOT, false);
        rgblight_set_layer_state(_BASE, layer_state_cmp(state, _BASE));
        rgblight_set_layer_state(_LOWER, layer_state_cmp(state, _LOWER));
        rgblight_set_layer_state(_RAISE, layer_state_cmp(state, _RAISE));
        rgblight_set_layer_state(_ADJUST, layer_state_cmp(state, _ADJUST));
    }
    return state;
}
#endif

enum custom_keycodes {
    RGB_SLD = SAFE_RANGE,
    T_LC,
    OS_SHFT,
    OS_CTRL,
    OS_ALT,
    OS_CMD,
    OBI,
    OBD,
};

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      XXXXXXX,    KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                         KC_Y,    KC_U,    KC_I,    KC_O,   KC_P,  KC_BSPC,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
       KC_ESC,    KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                         KC_H,    KC_J,    KC_K,    KC_L, KC_SCLN, KC_QUOT,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
       KC_TAB,    KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,                         KC_N,    KC_M, KC_COMM,  KC_DOT, KC_SLSH,  KC_ENT,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          KC_LALT,  LA_LOW, KC_LSFT,   KC_SPC,   LA_RSE, KC_RGUI
                                      //`--------------------------'  `--------------------------'

  ),

    [_LOWER] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      _______, XXXXXXX, XXXXXXX,    TABL,    TABR,  KC_F12,                       KC_INS,    HOME,   KC_UP,     END,  KC_DEL, _______,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      _______, OS_SHFT, OS_CTRL,  OS_ALT,  OS_CMD,  KC_F11,                      KC_CAPS, KC_LEFT, KC_DOWN, KC_RGHT, KC_BTN1, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      _______,   SPC_L,   SPC_R,   KC_F7,   KC_F9,   KC_F8,                      XXXXXXX, KC_PGDN, KC_PGUP, XXXXXXX, XXXXXXX, _______,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          _______, _______, _______,    _______, _______, _______
                                      //`--------------------------'  `--------------------------'
  ),

    [_RAISE] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      _______,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                         KC_6,    KC_7,   KC_8,     KC_9,    KC_0, _______,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
       KC_GRV, KC_MINS, KC_EQL,  KC_LBRC, KC_RBRC, KC_BSLS,                OSM(MOD_HYPR),  OS_CMD,  OS_ALT, OS_CTRL, OS_SHFT, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      _______,  KC_ESC, KC_RGUI, KC_RALT, KC_CAPS, KC_QUOT,                 OSM(MOD_MEH),  KC_F19,  KC_F11,  KC_F12,   KC_F8, _______,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          _______, _______, _______,    _______, _______, _______
                                      //`--------------------------'  `--------------------------'
  ),

     [_ADJUST] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      RGB_VAI, RGB_SAI, RGB_HUI, RGB_MOD,     OBI,    T_LC,                        KC_F1,   KC_F2,   KC_F3,   KC_F4, XXXXXXX, QK_BOOT,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      RGB_VAD, RGB_SAD, RGB_HUD, RGB_RMOD,    OBD, XXXXXXX,                        KC_F5,   KC_F6,   KC_F7,   KC_F8, KC_BTN1, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, RGB_TOG,                        KC_F9,  KC_F10,  KC_F11,  KC_F12, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          _______, _______, _______,    _______, _______, _______
                                      //`--------------------------'  `--------------------------'
  )
};
// clang-format on

bool is_oneshot_cancel_key(uint16_t keycode) {
    switch (keycode) {
        case LA_LOW:
        case LA_RSE:
        case LA_ADJ:
            return true;
        default:
            return false;
    }
}

bool is_oneshot_ignored_key(uint16_t keycode) {
    switch (keycode) {
        case LA_LOW:
        case LA_RSE:
        case LA_ADJ:
        case KC_LSFT:
        case OS_SHFT:
        case OS_CTRL:
        case OS_ALT:
        case OS_CMD:
            return true;
        default:
            return false;
    }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    update_oneshot(&os_shft_state, KC_LSFT, OS_SHFT, keycode, record);
    update_oneshot(&os_ctrl_state, KC_LCTL, OS_CTRL, keycode, record);
    update_oneshot(&os_alt_state, KC_LALT, OS_ALT, keycode, record);
    update_oneshot(&os_cmd_state, KC_LCMD, OS_CMD, keycode, record);

    switch (keycode) {
        case OS_SHFT:
        case OS_CTRL:
        case OS_ALT:
        case OS_CMD:
            layer_state_set(layer_state);
            return false;
#ifdef OLED_ENABLE
        case OBI:
            if (record->event.pressed) {
                oled_set_brightness(128);
            }
            return false;
        case OBD:
            if (record->event.pressed) {
                oled_set_brightness(64);
            }
            return false;
#endif
        case LA_LOW:
            if (record->event.pressed) {
                layer_on(_LOWER);
                update_tri_layer(_LOWER, _RAISE, _ADJUST);
            } else {
                layer_off(_LOWER);
                update_tri_layer(_LOWER, _RAISE, _ADJUST);
            }
            return false;
        case LA_RSE:
            if (record->event.pressed) {
                layer_on(_RAISE);
                update_tri_layer(_LOWER, _RAISE, _ADJUST);
            } else {
                layer_off(_RAISE);
                update_tri_layer(_LOWER, _RAISE, _ADJUST);
            }
            return false;
        case T_LC: // This allows me to use underglow as layer indication, or as normal
            if (record->event.pressed) {
                user_config.rgb_layer_change ^= 1; // Toggles the status
                // eeconfig_update_user(user_config.raw); // Writes the new status to EEPROM
                if (user_config.rgb_layer_change) { // if layer state indication is enabled,
                    layer_state_set(layer_state);   // then immediately update the layer color
                }
            }
            return false;
        case RGB_TOG ... RGB_MODE_TWINKLE:                // For any of the RGB codes (see quantum_keycodes.h, L400 for reference)
            if (record->event.pressed) {                  // This disables layer indication, as it's assumed that if you're changing this ... you want that disabled
                if (user_config.rgb_layer_change) {       // only if this is enabled
                    user_config.rgb_layer_change = false; // disable it, and
                    // eeconfig_update_user(user_config.raw); // write the setings to EEPROM
                }
            }
            return true;
            break;
        default:
            layer_state_set(layer_state); // then immediately update the layer color
            return true;                  // Process all other keycodes normally
    }
}
