// Copyright 2024 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

// Custom keycode handling: the home row mod runtime toggle and the bypass that
// makes those keys behave as plain letters while the toggle is off.

#include "lily58_user.h"

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case HRM_TOGG:
      if (record->event.pressed) {
        hrm_toggle();
      }
      return false;

    case HM_A: case HM_S: case HM_D: case HM_F:
    case HM_J: case HM_K: case HM_L: case HM_SCLN:
      if (!hrm_is_enabled()) {
        // Bypass the tap-hold logic entirely and act as a plain letter
        uint8_t tap_kc = QK_MOD_TAP_GET_TAP_KEYCODE(keycode);
        if (record->event.pressed) {
          register_code(tap_kc);
        } else {
          unregister_code(tap_kc);
        }
        return false;
      }
      return true;
  }
  return true;
}
