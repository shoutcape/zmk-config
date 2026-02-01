#!/usr/bin/env bash
set -euo pipefail

repo_root="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$repo_root"

left_in="build/left/zephyr/zmk.uf2"
right_in="build/right/zephyr/zmk.uf2"

if [[ ! -f "$left_in" ]]; then
  echo "Missing: $left_in" >&2
  echo "Build it with: west build -p always -d build/left -b nice_nano -s zmk/app -- -DZMK_CONFIG=\"$PWD/config\" -DSHIELD=\"splitkb_aurora_corne_left nice_view_adapter nice_view\"" >&2
  exit 1
fi

if [[ ! -f "$right_in" ]]; then
  echo "Missing: $right_in" >&2
  echo "Build it with: west build -p always -d build/right -b nice_nano -s zmk/app -- -DZMK_CONFIG=\"$PWD/config\" -DSHIELD=\"splitkb_aurora_corne_right nice_view_adapter nice_view\"" >&2
  exit 1
fi

out_dir="dist"
mkdir -p "$out_dir"

left_out="$out_dir/nice_nano-splitkb_aurora_corne_left.uf2"
right_out="$out_dir/nice_nano-splitkb_aurora_corne_right.uf2"
zip_out="$out_dir/nice_nano-splitkb_aurora_corne-firmware.zip"

cp -f "$left_in" "$left_out"
cp -f "$right_in" "$right_out"

(
  cd "$out_dir"
  sha256sum "$(basename "$left_out")" "$(basename "$right_out")" > SHA256SUMS.txt
  zip -q -r "$(basename "$zip_out")" \
    "$(basename "$left_out")" \
    "$(basename "$right_out")" \
    SHA256SUMS.txt
)

echo "Wrote: $left_out"
echo "Wrote: $right_out"
echo "Wrote: $zip_out"
