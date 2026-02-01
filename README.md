# ZMK Config - Splitkb Aurora Corne

Configuration for my wireless split keyboard with custom behaviors and display integration.

<img
  src="https://github.com/user-attachments/assets/047f85f6-19d1-4d18-9239-ad8dcc58da06"
  width="600"
/>

## Hardware

- **Board:** nice!nano
- **Keyboard:** Splitkb Aurora Corne
- **Display:** nice!view (both halves)
- **Firmware:** ZMK with Studio support (left half only)

## Custom Behaviors

### `LaltBackspace`
Backspace key that morphs to Option+Backspace when Command is held. Enables word deletion on macOS without triggering the default Cmd+Backspace behavior.

**Usage:** `&LaltBackspace`

### `Custom_Hold_Tap`
Hold for Caps Lock, tap for F key. Provides dual functionality in a single key position.

**Usage:** `&Custom_Hold_Tap CAPS F`

### `equals_tilde`
Hold for tilde (~), tap for equals (=). Quick access to both characters in number layers.

**Usage:** `&equals_tilde 0 LS(NUMBER_0)`

## ZMK Studio Workflow

**Important:** The left half has ZMK Studio enabled. Studio's runtime configuration takes precedence over compiled `.keymap` files.

### After Editing .keymap Files

1. Commit and push changes (triggers GitHub Actions build)
2. Flash new firmware to keyboard
3. Connect to ZMK Studio
4. **Click "Restore Stock Settings"**
5. Test your changes

### Why This is Required

- Studio's runtime config (stored in flash) overrides `.keymap` definitions
- Custom behaviors may appear as "unknown" until synced
- "Restore Stock Settings" syncs Studio with your `.keymap` file
- Only required when adding/modifying custom behaviors

## Layers

### Windows/Linux (0-7)
- **Base:** Default QWERTY layout
- **Layer 1-2:** Numbers, symbols, navigation
- **Layer 3-5:** Function keys, media controls, special characters
- **Layer 6-7:** Gaming optimized

### macOS (8-12)
- **MacOs:** Base layer with macOS-specific modifiers
- **MacOs 1-4:** Numbers, navigation, media controls

Switch between OS layouts using layer toggles on Layer 3.

## Building

Firmware builds automatically via GitHub Actions on push to `main`. Download artifacts from the Actions tab.

### Local builds

This repo is a full west workspace (Zephyr + ZMK + modules). For local builds you need:

- `west`
- Zephyr SDK (toolchain)
- Host tools like `cmake` and `ninja`

One-time setup (from the repo root):

```bash
west init -l config
west update
west zephyr-export
```

If you installed `west` via `pip`/`pipx`, you may need `pyelftools` for the build scripts:

```bash
pipx inject west pyelftools
```

Make sure Zephyr can find your toolchain (example path shown):

```bash
export ZEPHYR_TOOLCHAIN_VARIANT=zephyr
export ZEPHYR_SDK_INSTALL_DIR=/opt/zephyr-sdk
```

Build left + right (nice!view):

```bash
west build -p always -d build/left -b nice_nano -s zmk/app -- \
  -DZMK_CONFIG="$PWD/config" \
  -DSHIELD="splitkb_aurora_corne_left nice_view_adapter nice_view"

west build -p always -d build/right -b nice_nano -s zmk/app -- \
  -DZMK_CONFIG="$PWD/config" \
  -DSHIELD="splitkb_aurora_corne_right nice_view_adapter nice_view"
```

Headless builds:

```bash
west build -p always -d build/left-headless -b nice_nano -s zmk/app -- \
  -DZMK_CONFIG="$PWD/config" \
  -DSHIELD="splitkb_aurora_corne_left"

west build -p always -d build/right-headless -b nice_nano -s zmk/app -- \
  -DZMK_CONFIG="$PWD/config" \
  -DSHIELD="splitkb_aurora_corne_right"
```

Firmware output:

- `build/<target>/zephyr/zmk.uf2`

If you prefer a stable, easy-to-find output path for flashing, run:

```bash
./scripts/package-firmware.sh
```

Convenience (build both halves, then package to `dist/`):

```bash
./scripts/build-and-package.sh
```

This writes:

- `dist/nice_nano-splitkb_aurora_corne_left.uf2`
- `dist/nice_nano-splitkb_aurora_corne_right.uf2`
- `dist/nice_nano-splitkb_aurora_corne-firmware.zip`

### Configuration
- **Left half:** Studio enabled (`studio-rpc-usb-uart` + `CONFIG_ZMK_STUDIO=y`)
- **Right half:** Standard build
- **Both halves:** nice_view display support

## Local Shields

Custom shield definitions in `boards/shields/` enable display customization. See `boards/shields/README.md` for details.

## Resources

- [ZMK Documentation](https://zmk.dev/)
- [ZMK Studio](https://zmk.dev/docs/features/studio)
- [Aurora Corne](https://docs.splitkb.com/hc/en-us/articles/360010533820)
- [Agent Instructions](AGENTS.md) - Guidelines for AI assistants
