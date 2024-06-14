#include "oneshot.h"

void update_oneshot(
    oneshot_state *state,
    uint16_t mod,
    uint16_t trigger,
    uint16_t keycode,
    keyrecord_t *record
) {
    if (keycode == trigger) {
        if (record->event.pressed) {
            // Trigger keydown
            if (*state == OS_UP_UNQUEUED) {
                register_code(mod);
            }
            *state = OS_DOWN_UNUSED;
        } else {
            // Trigger keyup
            switch (*state) {
            case OS_DOWN_UNUSED:
                // If we didn't use the mod while trigger was held, queue it.
                *state = OS_UP_QUEUED;
                break;
            case OS_DOWN_USED:
                // If we did use the mod while trigger was held, unregister it.
                *state = OS_UP_UNQUEUED;
                unregister_code(mod);
                break;
            default:
                break;
            }
        }
    } else {
        if (record->event.pressed) {
            if (is_oneshot_cancel_key(keycode) && *state != OS_UP_UNQUEUED) {
                // Cancel oneshot on designated cancel keydown.
                *state = OS_UP_UNQUEUED;
                unregister_code(mod);
            }
        } else {
            if (!is_oneshot_ignored_key(keycode)) {
                // On non-ignored keyup, consider the oneshot used.
                switch (*state) {
                case OS_DOWN_UNUSED:
                    *state = OS_DOWN_USED;
                    break;
                case OS_UP_QUEUED:
                    *state = OS_UP_UNQUEUED;
                    unregister_code(mod);
                    break;
                default:
                    break;
                }
            }
        }
    }
}
