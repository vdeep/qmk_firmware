// Copyright 2024 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

// RP2040 bootloader configuration
#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET
#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET_TIMEOUT 500U

// I2C configuration for OLED
#define I2C_DRIVER I2CD1
#define I2C1_SDA_PIN GP2
#define I2C1_SCL_PIN GP3

// Serial configuration for split communication (full-duplex UART)
// Requires crossover wiring: Left GP0 <-> Right GP1, Left GP1 <-> Right GP0
#define SERIAL_USART_FULL_DUPLEX
#define SERIAL_USART_TX_PIN GP0
#define SERIAL_USART_RX_PIN GP1

// OLED configuration
#define OLED_DISPLAY_128X32
#define OLED_TIMEOUT 30000
#define OLED_BRIGHTNESS 128

// Split keyboard settings
#define SPLIT_LED_STATE_ENABLE

// Encoder resolution
#define ENCODER_RESOLUTION 4
