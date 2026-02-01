#!/usr/bin/env bash
set -euo pipefail

repo_root="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$repo_root"

if ! command -v west >/dev/null 2>&1; then
  echo "Missing: west" >&2
  echo "Install west first, then retry." >&2
  exit 1
fi

if [[ "${ZEPHYR_TOOLCHAIN_VARIANT:-}" != "zephyr" || -z "${ZEPHYR_SDK_INSTALL_DIR:-}" ]]; then
  echo "Missing toolchain env vars." >&2
  echo "Set these, then retry:" >&2
  echo "  export ZEPHYR_TOOLCHAIN_VARIANT=zephyr" >&2
  echo "  export ZEPHYR_SDK_INSTALL_DIR=/opt/zephyr-sdk" >&2
  exit 1
fi

west build -p always -d build/left -b nice_nano -s zmk/app -- \
  -DZMK_CONFIG="$PWD/config" \
  -DSHIELD="splitkb_aurora_corne_left nice_view_adapter nice_view"

west build -p always -d build/right -b nice_nano -s zmk/app -- \
  -DZMK_CONFIG="$PWD/config" \
  -DSHIELD="splitkb_aurora_corne_right nice_view_adapter nice_view"

./scripts/package-firmware.sh
