#pragma once

/* The way how "handedness" is decided (which half is which),
see https://docs.qmk.fm/#/feature_split_keyboard?id=setting-handedness
for more options.
*/

#define MASTER_LEFT
// #define MASTER_RIGHT

//#define TAPPING_TERM 170
#define TAPPING_TERM 170

// Prevent normal rollover on alphas from accidentally triggering mods.
//#define IGNORE_MOD_TAP_INTERRUPT

// Enable rapid switch from tap to hold, disables double tap hold auto-repeat.
//#define TAPPING_FORCE_HOLD

// Auto Shift
#define NO_AUTO_SHIFT_ALPHA
#define AUTO_SHIFT_TIMEOUT TAPPING_TERM
#undef AUTO_SHIFT_NO_SETUP
#define AUTO_SHIFT_NO_SETUP

#undef LOCKING_SUPPORT_ENABLE
#undef LOCKING_RESYNC_ENABLE
//#define NO_ACTION_ONESHOT
//#define NO_ACTION_TAPPING
//#define NO_MUSIC_MODE

#define VIAL_KEYBOARD_UID {0xBA, 0x38, 0x22, 0x3F, 0x8C, 0x42, 0x0F, 0x44}
#define VIAL_UNLOCK_COMBO_ROWS {0, 0}
#define VIAL_UNLOCK_COMBO_COLS {1, 2}
