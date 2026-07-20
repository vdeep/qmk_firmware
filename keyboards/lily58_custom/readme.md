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

This keymap uses `EE_HANDS`, so each half stores its own handedness in EEPROM rather
than assuming the USB half is the left one. Either half can take the cable, but each
must be flashed once with its handedness target to write that flag:

```bash
qmk flash -kb lily58_custom -km default -bl uf2-split-left    # left half in bootloader
qmk flash -kb lily58_custom -km default -bl uf2-split-right   # right half in bootloader
```

After that one-time step, plain `qmk flash -kb lily58_custom -km default` works for
either half. `./build.sh --flash` walks both halves in order and prompts for each.

To enter bootloader mode, double-tap reset on a half; it mounts as an `RPI-RP2` drive.
Flashing is just a `.uf2` copy, so dragging the file onto that drive also works.

### Recovering a bad EEPROM

Bootmagic is deliberately disabled (`keyboard.json`). On RP2040 its EEPROM reset
formats the whole wear-levelled region, which would erase the `EE_HANDS` handedness
byte along with the stored home-row-mod toggle and OLED brightness — turning a stuck
key at power-on into a silent handedness wipe.

If the EEPROM ever does need clearing, flash `flash_nuke.uf2` to erase it, then
re-flash both halves with the split-left/right targets above to restore handedness.
`QK_BOOT` on the Adjust layer still reaches the bootloader for everyday flashing.

## Layers

### Base (QWERTY)
Standard QWERTY layout with modifiers.

### Lower
- Function keys (F1-F12)
- Numbers and symbols

### Raise
- Numbers, F1-F12
- Arrow keys on the right home row
- Bracket and operator symbols

### Adjust (Lower + Raise)
- `QK_BOOT` for entering bootloader
- Caps Lock on Esc
- Home row mod toggle (`HRM_TOGG`)

## Encoder Functions

Encoder behaviour depends on the active layer:

| Layer | Left encoder | Right encoder |
|-------|--------------|---------------|
| Base | Volume Up/Down | Page Down/Up |
| Raise | Display brightness | Display brightness |
| Adjust | OLED brightness | OLED brightness |

OLED brightness is saved to EEPROM and synced to the slave half over a split
transaction, so both displays track together.

Only the left encoder is populated. The right half's `pin_a`/`pin_b` are swapped
relative to the left in `keyboard.json`, matching the convention upstream uses for
`lily58/light` and `lily58/glow_enc` — the right PCB is mirrored, so the swap is what
makes both encoders turn the same logical direction. Untested on hardware.

## OLED Display

- **Master**: four rows — layer (with a right-aligned `CAPS` indicator), home row mod
  state, live modifiers, and the current encoder binding
- **Slave**: Lily58 logo

The display is 21 columns wide. No row may be a full 21 characters: `oled_write_ln`
always advances a page, so a row-filling line consumes two rows and pushes the last
line off the 4-row display. 20 is the usable maximum.

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
