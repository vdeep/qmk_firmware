# Lily58 Custom RP2040

A custom Lily58 split keyboard configuration using SparkFun Pro Micro RP2040 controllers.

## Features

- **MCU**: SparkFun Pro Micro RP2040
- **Split Communication**: Full-duplex UART (Serial)
- **OLED Displays**: 128x32 I2C OLED on each half
- **Rotary Encoders**: One per half (volume on left, page up/down on right)
- **Layout**: Standard Lily58 (58 keys total)

## Pin Assignments

| Function   | Left Side | Right Side |
|------------|-----------|------------|
| Columns    | GP4, GP5, GP6, GP7, GP8, GP9 | GP9, GP8, GP7, GP6, GP5, GP4 |
| Rows       | GP29, GP28, GP27, GP26, GP22 | GP22, GP26, GP27, GP28, GP29 |
| Encoder A  | GP20      | GP20       |
| Encoder B  | GP23      | GP23       |
| I2C SDA    | GP2       | GP2        |
| I2C SCL    | GP3       | GP3        |
| Serial TX  | GP0       | GP0        |
| Serial RX  | GP1       | GP1        |

## Wiring

### Split Communication (CRITICAL)

The serial connection between halves uses **crossover wiring**:

```
Left Half          Right Half
---------          ----------
GP0 (TX) --------> GP1 (RX)
GP1 (RX) <-------- GP0 (TX)
GND      <-------> GND
```

**WARNING**: Incorrect wiring can damage the microcontrollers. Double-check before powering on:
- TX connects to RX on the opposite side (crossover, not straight-through)
- Both GND pins must be connected

### I2C for OLED

Each half has its own OLED connected to I2C1:
- GP2 (SDA) to OLED SDA
- GP3 (SCL) to OLED SCL
- 3.3V to OLED VCC
- GND to OLED GND

Most 128x32 OLEDs have built-in pull-up resistors. If your display doesn't work, you may need 4.7k pull-ups on SDA and SCL.

### Encoders

Connect rotary encoders to:
- GP20 (Encoder A)
- GP23 (Encoder B)
- GND (Common)

### Matrix

Standard Lily58 matrix wiring with diodes in COL2ROW orientation.

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
- Navigation

### Raise
- Navigation cluster (arrows, page up/down, home/end)
- Insert, Print Screen, Menu
- Caps Lock toggle
- Clipboard shortcuts (Undo, Cut, Copy, Paste)

### Adjust (Lower + Raise)
- QK_BOOT for entering bootloader
- Media controls (Volume, Play/Pause, Next/Previous)

## Encoder Functions

- **Left Encoder**: Volume Up/Down
- **Right Encoder**: Page Down/Up

## OLED Display

- **Master (left)**: Shows current layer, modifier status, and lock indicators
- **Slave (right)**: Shows QMK logo

## Troubleshooting

### Split communication not working
1. Verify crossover wiring (TX to RX, RX to TX)
2. Check GND connection between halves
3. Ensure both halves have the same firmware

### OLED not displaying
1. Check I2C wiring (SDA to GP2, SCL to GP3)
2. Verify OLED is 3.3V compatible
3. Try adding 4.7k pull-up resistors if not built into OLED

### Keys not registering
1. Check diode orientation (cathode/band toward row pin)
2. Verify column and row wiring matches pin assignments
