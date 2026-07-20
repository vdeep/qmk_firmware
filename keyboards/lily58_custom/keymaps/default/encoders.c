// Copyright 2024 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

// Rotary encoder bindings. What each encoder does depends on the active layer.

#include "lily58_user.h"

#ifdef ENCODER_ENABLE

bool encoder_update_user(uint8_t index, bool clockwise) {
    // _ADJUST must be checked before _RAISE: tri-layer leaves both LOWER and
    // RAISE on while ADJUST is active, so the RAISE branch would swallow it
    if (IS_LAYER_ON(_ADJUST)) {
#ifdef OLED_ENABLE
        // Both encoders: OLED brightness, saved to EEPROM
        oled_brightness_adjust(clockwise);
#endif
    } else if (IS_LAYER_ON(_RAISE)) {
        // Both encoders: Display brightness
        if (clockwise) {
            tap_code(KC_BRIU);
        } else {
            tap_code(KC_BRID);
        }
    } else if (index == 0) {
        // Left encoder: Volume control
        if (clockwise) {
            tap_code(KC_VOLU);
        } else {
            tap_code(KC_VOLD);
        }
    } else if (index == 1) {
        // Right encoder: Page up/down
        if (clockwise) {
            tap_code(KC_PGDN);
        } else {
            tap_code(KC_PGUP);
        }
    }
    return false;
}

#endif // ENCODER_ENABLE
