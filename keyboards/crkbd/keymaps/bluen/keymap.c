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
#define SFT_BS LSFT_T(KC_BSPC)

oneshot_state os_shft_state = OS_UP_UNQUEUED;
oneshot_state os_ctrl_state = OS_UP_UNQUEUED;
oneshot_state os_alt_state  = OS_UP_UNQUEUED;
oneshot_state os_cmd_state  = OS_UP_UNQUEUED;

enum layer_names { _BASE, _LOWER, _RAISE, _ADJUST };

enum custom_keycodes {
    RGB_SLD = QK_KB_0,
    OS_SHFT,
    OS_CTRL,
    OS_ALT,
    OS_CMD,
    OL_TGL,
    OL_32,
    OL_64
};

bool oled_enabled = true;

void keyboard_post_init_user(void) {
    oled_set_brightness(64);
}


// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                     ,-----------------------------------------------------.
      XXXXXXX,    KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                          KC_Y,    KC_U,    KC_I,    KC_O,   KC_P,  KC_BSPC,
  //|--------+--------+--------+--------+--------+--------|                     |--------+--------+--------+--------+--------+--------|
       KC_ESC,    KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                          KC_H,    KC_J,    KC_K,    KC_L, KC_SCLN, KC_QUOT,
  //|--------+--------+--------+--------+--------+--------|                     |--------+--------+--------+--------+--------+--------|
       KC_TAB,    KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,                          KC_N,    KC_M, KC_COMM,  KC_DOT, KC_SLSH,  KC_ENT,
  //|--------+--------+--------+--------+--------+--------+--------|   |--------+--------+--------+--------+--------+--------+--------|
                                          LALT_T(KC_DEL),  LA_LOW, KC_LSFT,      KC_SPC,  LA_RSE, RGUI_T(KC_BSPC)
                                      //`--------------------------'   `--------------------------'

  ),

    [_LOWER] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      _______, XXXXXXX, XXXXXXX,    TABL,    TABR,  KC_F12,                       KC_INS,    HOME,   KC_UP,     END,  KC_DEL, _______,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      _______, OS_SHFT, OS_CTRL,  OS_ALT,  OS_CMD,  KC_F11,                      KC_CAPS, KC_LEFT, KC_DOWN, KC_RGHT, KC_BTN1, KC_BTN2,
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
      QK_BOOT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, OL_64  ,                        KC_F1,   KC_F2,   KC_F3,   KC_F4, XXXXXXX, QK_BOOT,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      CG_SWAP, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, OL_32  ,                        KC_F5,   KC_F6,   KC_F7,   KC_F8, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      CG_NORM, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, OL_TGL ,                        KC_F9,  KC_F10,  KC_F11,  KC_F12, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          _______, _______, _______,    _______, _______, _______
                                      //`--------------------------'  `--------------------------'
  )
};

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
        case OL_32:
            oled_set_brightness(32);
            return false;
        case OL_64:
            oled_set_brightness(64);
            return false;
        case OL_TGL:
            if (record->event.pressed) {
                oled_enabled = !oled_enabled;  // toggle oled_enabled
            }
            return true;
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
        default:
            layer_state_set(layer_state);   // then immediately update the layer color
            return true; // Process all other keycodes normally
    }
}
