// Copyright 2024 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

// Shared declarations for the lily58_custom default keymap. The keymap is split
// across a few files by concern: keymap.c (layouts and layer state),
// user_config.c (EEPROM-backed settings), home_row_mods.c (tap-hold handling),
// oled_display.c (rendering and brightness), encoders.c (encoder bindings).

#pragma once

#include QMK_KEYBOARD_H

enum layer_number {
  _QWERTY = 0,
  _LOWER,
  _RAISE,
  _ADJUST,
};

// Home row mods, CAGS order (Ctrl-Alt-GUI-Shift) mirrored across both hands
#define HM_A    LCTL_T(KC_A)
#define HM_S    LALT_T(KC_S)
#define HM_D    LGUI_T(KC_D)
#define HM_F    LSFT_T(KC_F)
#define HM_J    RSFT_T(KC_J)
#define HM_K    RGUI_T(KC_K)
#define HM_L    RALT_T(KC_L)
#define HM_SCLN RCTL_T(KC_SCLN)

enum custom_keycodes {
  HRM_TOGG = QK_USER,  // Toggle home row mods on/off, persisted to EEPROM
};

#define OLED_BRIGHTNESS_STEP 16
#define OLED_BRIGHTNESS_MIN  8

// user_config.c — settings persisted to EEPROM
void    user_config_load(void);
bool    hrm_is_enabled(void);
void    hrm_toggle(void);
uint8_t user_config_oled_brightness(void);
void    user_config_set_oled_brightness(uint8_t level);

// oled_display.c — hardware brightness and the split sync that backs it
#ifdef OLED_ENABLE
void oled_brightness_init(void);
void oled_brightness_adjust(bool increase);
#endif
