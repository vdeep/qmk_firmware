# Lily58 Custom RP2040

A Lily58 configuration for SparkFun Pro Micro RP2040 as a drop-in replacement for AVR Pro Micro.

## Features

- **MCU**: SparkFun Pro Micro RP2040 (drop-in Pro Micro replacement)
- **Split Communication**: Half-duplex serial (same as original Lily58)
- **OLED Displays**: 128x32 I2C OLED on each half
- **Rotary Encoders**: One per half (optional)
- **Layout**: Standard Lily58 (58 keys total)

## Pin Mapping (Pro Micro → RP2040)

This configuration uses the standard Pro Micro footprint pin mapping:

| Pro Micro Pin | RP2040 GPIO | Function |
|---------------|-------------|----------|
| D2 | GP1 | Split serial data |
| D1 | GP2 | I2C SDA (OLED) |
| D0 | GP3 | I2C SCL (OLED) |
| D4 | GP4 | - |
| C6 | GP5 | Row 0 |
| D7 | GP6 | Row 1 |
| E6 | GP7 | Row 2 |
| B4 | GP8 | Row 3 |
| B5 | GP9 | Row 4 |
| B6 | GP21 | Column 5 |
| B2 | GP23 | Column 4 |
| B3 | GP20 | Column 3 |
| B1 | GP22 | Column 2 |
| F7 | GP26 | Column 1 |
| F6 | GP27 | Column 0 |
| F5 | GP28 | Encoder B (optional) |
| F4 | GP29 | Encoder A (optional) |

## Wiring

If your Lily58 PCB was designed for AVR Pro Micro, no rewiring is needed - the SparkFun Pro Micro RP2040 is a drop-in replacement with the same footprint.

### Split Communication
Uses half-duplex serial on D2 (GP1), same as original Lily58. Just connect the TRRS cable as usual.

### I2C for OLED
- D1 (GP2) = SDA
- D0 (GP3) = SCL

### Encoders (Optional)
If your Lily58 has encoder support:
- F4 (GP29) = Encoder A
- F5 (GP28) = Encoder B

## Building Firmware

```bash
qmk compile -kb lily58_custom -km default
```

## Flashing

1. Enter bootloader mode by double-tapping the reset button
2. The RP2040 will appear as a USB drive (RPI-RP2)
3. Copy the `.uf2` file to the drive
4. Repeat for the other half

```bash
qmk flash -kb lily58_custom -km default
```

## Layers

### Base (QWERTY)
Standard QWERTY layout with modifiers.

### Lower
- Function keys (F1-F12)
- Numbers and symbols

### Raise
- Navigation (arrows, page up/down, home/end)
- Clipboard shortcuts

### Adjust (Lower + Raise)
- QK_BOOT for entering bootloader
- Media controls

## Encoder Functions

- **Left Encoder**: Volume Up/Down
- **Right Encoder**: Page Down/Up

## OLED Display

- **Master (left)**: Shows current layer, modifier status, and lock indicators
- **Slave (right)**: Shows Lily58 logo

## Troubleshooting

### Keys not registering
- Verify your PCB is wired for standard Lily58 rev1 pinout
- Check diode orientation

### OLED not displaying
- Ensure OLED is connected to D1 (SDA) and D0 (SCL)
- Most 128x32 OLEDs have built-in pull-ups

### Split not working
- Check TRRS cable connection
- Ensure both halves have the same firmware
