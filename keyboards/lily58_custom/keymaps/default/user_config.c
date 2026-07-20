// Copyright 2024 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

// EEPROM-backed user settings: home row mod toggle and OLED brightness.

#include "lily58_user.h"

// Stored inverted so a freshly-flashed board (EEPROM all zeroes) boots with mods on
typedef union {
  uint32_t raw;
  struct {
    bool    hrm_disabled    : 1;
    uint8_t oled_brightness : 8;
  };
} user_config_t;

static user_config_t user_config;
static bool          hrm_enabled = true;

void eeconfig_init_user(void) {
  user_config.raw = 0;  // hrm_disabled = false, i.e. home row mods on
  user_config.oled_brightness = OLED_BRIGHTNESS;
  eeconfig_update_user(user_config.raw);
}

void user_config_load(void) {
  user_config.raw = eeconfig_read_user();
  hrm_enabled = !user_config.hrm_disabled;
  // Guard against EEPROM written by an earlier build that had no brightness field,
  // which would otherwise restore a fully dark display
  if (user_config.oled_brightness < OLED_BRIGHTNESS_MIN) {
    user_config.oled_brightness = OLED_BRIGHTNESS;
  }
}

bool hrm_is_enabled(void) {
  return hrm_enabled;
}

void hrm_toggle(void) {
  hrm_enabled = !hrm_enabled;
  user_config.hrm_disabled = !hrm_enabled;
  eeconfig_update_user(user_config.raw);
}

uint8_t user_config_oled_brightness(void) {
  return user_config.oled_brightness;
}

void user_config_set_oled_brightness(uint8_t level) {
  user_config.oled_brightness = level;
  eeconfig_update_user(user_config.raw);
}
