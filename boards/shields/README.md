# Local Shields

This directory contains local copies of shields used in this ZMK config, allowing for customization specific to this setup.

## Hardware Setup

This configuration is designed for:
- **Board:** nice!nano
- **Keyboard:** Splitkb Aurora Corne
- **Display:** nice!view

## Shields

### nice_view

The nice!view display shield provides the visual interface for the keyboard. This is a low-power, high refresh rate SPI display.

**Key Files:**
- `nice_view.overlay` - Device tree configuration
- `nice_view.conf` - Display settings
- `custom_status_screen.c` - Custom display layout
- `widgets/` - Display widget implementations (battery, connection status, layers, etc.)

**Customization:**
- Modify `widgets/` files to change display elements
- Edit `custom_status_screen.c` to adjust overall layout
- Update `nice_view.conf` to change display behavior

### nice_view_adapter

The adapter shield provides the interface between the nice!nano board and the nice!view display, utilizing the I2C OLED header pins.

**Key Files:**
- `nice_view_adapter.overlay` - Base adapter configuration
- `boards/nice_nano.overlay` - nice!nano-specific pin configuration (SPI: SCK=P0.20, MOSI=P0.17, MISO=P0.25, CS=D1)

**Note:** The CS pin is bodged to D1 (pro_micro pin 1) as per the adapter design.

## Build Configuration

These shields are used in `build.yaml`:

```yaml
shield: splitkb_aurora_corne_left nice_view_adapter nice_view
```

The build system automatically uses these local shields instead of the upstream ZMK versions.

## Why Local Shields?

1. **Customization** - Full control over display behavior and appearance
2. **Stability** - No dependency on upstream changes
3. **Simplicity** - Removed unused board support, keeping only nice!nano configuration
4. **Version Control** - All customizations tracked in this repository

## Upstream Source

These shields were originally copied from [ZMK Firmware](https://github.com/zmkfirmware/zmk) and simplified for this specific hardware setup.
