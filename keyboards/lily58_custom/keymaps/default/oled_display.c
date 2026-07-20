// Copyright 2024 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

// OLED status rendering, plus brightness control and the split transaction that
// keeps the slave half's display at the same level as the master's.

#include "lily58_user.h"

#ifdef OLED_ENABLE

#include "transactions.h"

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
  if (!is_keyboard_master())
    return OLED_ROTATION_180;  // flips the display 180 degrees if offhand
  return rotation;
}

static void render_layer_state(void) {
    switch (get_highest_layer(layer_state)) {
        case _QWERTY:
            oled_write_ln_P(PSTR("Layer: Default"), false);
            break;
        case _LOWER:
            oled_write_ln_P(PSTR("Layer: Lower"), false);
            break;
        case _RAISE:
            oled_write_ln_P(PSTR("Layer: Raise"), false);
            break;
        case _ADJUST:
            oled_write_ln_P(PSTR("Layer: Adjust"), false);
            break;
        default:
            oled_write_ln_P(PSTR("Layer: ???"), false);
    }
}

// Live modifier state, in the same CAGS order as the home row
static void render_mod_state(void) {
    uint8_t mods = get_mods() | get_oneshot_mods();
    oled_write_P(PSTR("Held:  "), false);
    oled_write_P((mods & MOD_MASK_CTRL)  ? PSTR("C") : PSTR("-"), false);
    oled_write_P((mods & MOD_MASK_ALT)   ? PSTR("A") : PSTR("-"), false);
    oled_write_P((mods & MOD_MASK_GUI)   ? PSTR("G") : PSTR("-"), false);
    oled_write_ln_P((mods & MOD_MASK_SHIFT) ? PSTR("S") : PSTR("-"), false);
}

// Caps lock plus what the encoders are currently bound to
static void render_status_line(void) {
    oled_write_P(PSTR("Enc:   "), false);
    if (IS_LAYER_ON(_ADJUST)) {
        oled_write_P(PSTR("OLED  "), false);
    } else if (IS_LAYER_ON(_RAISE)) {
        oled_write_P(PSTR("Brite "), false);
    } else {
        oled_write_P(PSTR("Vol/Pg"), false);
    }
    oled_write_ln_P(host_keyboard_led_state().caps_lock ? PSTR(" CAPS") : PSTR("     "), false);
}

static void render_logo(void) {
    static const char PROGMEM logo[] = {
        0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F, 0x90, 0x91, 0x92, 0x93, 0x94,
        0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF, 0xB0, 0xB1, 0xB2, 0xB3, 0xB4,
        0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF, 0xD0, 0xD1, 0xD2, 0xD3, 0xD4,
        0x00
    };
    oled_write_P(logo, false);
}

bool oled_task_user(void) {
  if (is_keyboard_master()) {
    render_layer_state();
    oled_write_ln_P(hrm_is_enabled() ? PSTR("Mods:  Home row") : PSTR("Mods:  Off"), false);
    render_mod_state();
    render_status_line();
  } else {
    render_logo();
  }
    return false;
}

// oled_set_brightness() only affects the half it runs on, and the encoder handler
// runs on the master. Push the level over to the slave so both displays match.
static bool oled_brightness_dirty = true;

static void user_sync_oled_brightness_handler(uint8_t in_buflen, const void *in_data, uint8_t out_buflen, void *out_data) {
  const uint8_t *level = (const uint8_t *)in_data;
  oled_set_brightness(*level);
}

void oled_brightness_init(void) {
  transaction_register_rpc(USER_SYNC_OLED_BRIGHTNESS, user_sync_oled_brightness_handler);
  oled_set_brightness(user_config_oled_brightness());
}

void oled_brightness_adjust(bool increase) {
  uint8_t level = oled_get_brightness();
  if (increase) {
    level = (level > 255 - OLED_BRIGHTNESS_STEP) ? 255 : level + OLED_BRIGHTNESS_STEP;
  } else {
    level = (level < OLED_BRIGHTNESS_MIN + OLED_BRIGHTNESS_STEP) ? OLED_BRIGHTNESS_MIN : level - OLED_BRIGHTNESS_STEP;
  }
  oled_set_brightness(level);
  user_config_set_oled_brightness(level);
  oled_brightness_dirty = true;  // housekeeping pushes it to the slave
}

void housekeeping_task_user(void) {
  if (!is_keyboard_master()) {
    return;
  }
  static uint32_t last_sync    = 0;
  static uint32_t last_refresh = 0;

  // Periodic refresh so a slave that reset or reconnected picks the level back up
  if (timer_elapsed32(last_refresh) > 5000) {
    last_refresh           = timer_read32();
    oled_brightness_dirty = true;
  }

  if (oled_brightness_dirty && timer_elapsed32(last_sync) > 100) {
    last_sync      = timer_read32();
    uint8_t level  = user_config_oled_brightness();
    if (transaction_rpc_send(USER_SYNC_OLED_BRIGHTNESS, sizeof(level), &level)) {
      oled_brightness_dirty = false;  // retried on the next tick if this failed
    }
  }
}

#endif // OLED_ENABLE
