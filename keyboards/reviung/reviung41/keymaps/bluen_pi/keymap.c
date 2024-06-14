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

void keyboard_post_init_user(void) {
    // Call the keymap level matrix init.

    // Read the user config from EEPROM
    user_config.raw              = 0; // eeconfig_read_user();
    user_config.rgb_layer_change = true;

    // Set default layer, if enabled
    if (user_config.rgb_layer_change) {
        rgblight_enable_noeeprom();
        rgblight_sethsv_noeeprom(HSV_PURPLE);
        rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);
    }
}

enum layer_names { _BASE, _LOWER, _RAISE, _ADJUST, _CAPSLOCK };

bool led_update_user(led_t led_state) {
    rgblight_set_layer_state(_CAPSLOCK, led_state.caps_lock);
    user_config.caps_lock_on = led_state.caps_lock;
    return true;
}

enum custom_keycodes {
    RGB_SLD = SAFE_RANGE,
    T_LC,
    OS_SHFT,
    OS_CTRL,
    OS_ALT,
    OS_CMD,
};

// clang-format off
      const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
        [_BASE] = LAYOUT(
          KC_TAB,  KC_Q, KC_W, KC_E, KC_R,   KC_T,            KC_Y,    KC_U, KC_I,    KC_O,   KC_P,    KC_BSPC,
          KC_ESC,  KC_A, KC_S, KC_D, KC_F,   KC_G,            KC_H,    KC_J, KC_K,    KC_L,   KC_SCLN, KC_QUOT,
          KC_LSFT, KC_Z, KC_X, KC_C, KC_V,   KC_B,            KC_N,    KC_M, KC_COMM, KC_DOT, KC_SLSH, KC_ENT,
                                     LA_LOW, KC_LSFT, KC_SPC, KC_SPC,  LA_RSE),
        [_LOWER] = LAYOUT(
          KC_TRNS, KC_NO,   KC_NO,   TABL,   TABR,    KC_F12,            KC_INS,  HOME,    KC_UP,   END,     KC_DEL, KC_BSPC,
          KC_LCTL, OS_SHFT, OS_CTRL, OS_ALT, OS_CMD,  KC_F11,            KC_CAPS, KC_LEFT, KC_DOWN, KC_RGHT, KC_NO,  KC_NO,
          KC_TRNS, SPC_L,   SPC_R,   KC_F7,  KC_F9,   KC_F8,             KC_NO,   KC_PGDN, KC_PGUP, KC_NO,   KC_NO,  KC_TRNS,
                                             KC_TRNS, KC_TRNS,  KC_SPC,  KC_TRNS, KC_TRNS),
        [_RAISE] = LAYOUT(
          KC_GRV,  KC_1,    KC_2,    KC_3,     KC_4,    KC_5,               KC_6,          KC_7,   KC_8,  KC_9,     KC_0,   KC_BSPC,
          KC_TRNS, KC_MINS, KC_EQL,  KC_LBRC,  KC_RBRC, KC_BSLS,            OSM(MOD_HYPR), OS_CMD, OS_ALT, OS_CTRL, OS_SHFT, KC_NO,
          KC_TRNS, KC_ESC,  KC_RGUI, KC_RALT,  KC_CAPS, KC_QUOT,            OSM(MOD_MEH),  KC_F19, KC_F11, KC_F12,  KC_F8,  KC_TRNS,
                                               KC_TRNS, KC_TRNS,   KC_DEL,  KC_TRNS,       KC_TRNS),
        [_ADJUST] = LAYOUT(
          RGB_VAI, RGB_SAI, RGB_HUI, RGB_MOD,  KC_NO,   T_LC,              KC_F1,   KC_F2,  KC_F3,  KC_F4,  KC_NO, QK_BOOT,
          RGB_VAD, RGB_SAD, RGB_HUD, RGB_RMOD, KC_NO,   KC_NO,             KC_F5,   KC_F6,  KC_F7,  KC_F8,  KC_NO, KC_NO,
          KC_NO,   KC_NO,   KC_NO,   KC_NO,    KC_NO,   RGB_TOG,           KC_F9,   KC_F10, KC_F11, KC_F12, KC_NO, KC_NO,
                                               KC_TRNS, KC_TRNS, KC_TRNS,  KC_TRNS, KC_TRNS),
};
// clang-format on

layer_state_t layer_state_set_user(layer_state_t state) {
    if (user_config.caps_lock_on) {
        if (user_config.rgb_layer_change) {
            rgblight_sethsv_noeeprom(HSV_RED);
            rgblight_mode_noeeprom(RGBLIGHT_MODE_ALTERNATING);
        }
        return state;
    }
    if (os_shft_state == OS_UP_QUEUED || os_ctrl_state == OS_UP_QUEUED || os_alt_state == OS_UP_QUEUED || os_cmd_state == OS_UP_QUEUED) {
        rgblight_sethsv_noeeprom(HSV_RED);
        rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);
        return state;
    }
    switch (get_highest_layer(state)) {
        case _RAISE:
            if (user_config.rgb_layer_change) {
                rgblight_sethsv_noeeprom(HSV_MAGENTA);
                rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);
            }
            break;
        case _LOWER:
            if (user_config.rgb_layer_change) {
                rgblight_sethsv_noeeprom(HSV_GREEN);
                rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);
            }
            break;
        case _ADJUST:
            if (user_config.rgb_layer_change) {
                rgblight_sethsv_noeeprom(HSV_YELLOW);
                rgblight_mode_noeeprom(RGBLIGHT_MODE_ALTERNATING);
            }
            break;
        case _CAPSLOCK:
            if (user_config.rgb_layer_change) {
                rgblight_sethsv_noeeprom(HSV_RED);
                rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);
            }
            break;
        default: //  for any other layers, or the default layer
            if (user_config.rgb_layer_change) {
                rgblight_sethsv_noeeprom(HSV_PURPLE);
                rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);
            } else {
                rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);
            }
            break;
    }
    return state;
}

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
            return true; // Process all other keycodes normally
    }
}

/*
void eeconfig_init_user(void) {  // EEPROM is getting reset!
  user_config.raw = 0;
  user_config.rgb_layer_change = true; // We want this enabled by default
  eeconfig_update_user(user_config.raw); // Write default value to EEPROM now

  // use the non noeeprom versions, to write these values to EEPROM too
  rgblight_enable(); // Enable RGB by default
  rgblight_sethsv(HSV_CYAN);  // Set it to CYAN by default
  rgblight_mode(2); // set to solid by default
}

*/
