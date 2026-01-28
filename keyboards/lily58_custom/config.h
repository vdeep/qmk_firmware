// Copyright 2024 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

// RP2040 bootloader configuration
#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET
#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET_TIMEOUT 500U

// I2C configuration for OLED (D1=GP2, D0=GP3 on Pro Micro footprint)
#define I2C_DRIVER I2CD1
#define I2C1_SDA_PIN GP2
#define I2C1_SCL_PIN GP3

// OLED configuration
#define OLED_DISPLAY_128X32
#define OLED_TIMEOUT 30000
#define OLED_BRIGHTNESS 128

// Split keyboard settings
#define SPLIT_LED_STATE_ENABLE

// Encoder resolution
#define ENCODER_RESOLUTION 4

// Use the lily version to get the Lily58 logo instead of the qmk logo
#define OLED_FONT_H "lib/glcdfont_lily.c"
