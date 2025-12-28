# ZMK Config - Splitkb Aurora Corne

Configuration for a wireless split keyboard with custom behaviors and display integration.

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
