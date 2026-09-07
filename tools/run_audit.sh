#!/usr/bin/env bash
set -euo pipefail

script_dir="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
repo_dir="$(cd -- "${script_dir}/.." && pwd)"
profile_dir="${repo_dir}/profiles/r8q-G781WVLSLHYJ1"
target_header="${profile_dir}/target.h"
preflight_src="${repo_dir}/preflight/hyj1_preload_slide.c"
symbol_json="${profile_dir}/symbols.json"

if grep -Eirq 'S23|S918|pa3q|S938|dm3q|S25' \
  "${profile_dir}" "${repo_dir}/preflight"; then
  printf '%s\n' 'cross-device identifier found in active source' >&2
  exit 1
fi

if grep -Eirq --exclude='run_audit.sh' --exclude-dir=.git --exclude-dir=build \
  '/home/[^/]+|/Users/[^/]+|/run/media/[^/]+' "${repo_dir}"; then
  printf '%s\n' 'personal filesystem path found in repository source' >&2
  exit 1
fi

if grep -Eirq --exclude='run_audit.sh' --exclude-dir=.git --exclude-dir=build \
  'ffffffc0f55d3d00|ffffffc031496800' "${repo_dir}"; then
  printf '%s\n' 'expired process-local pointer found in repository source' >&2
  exit 1
fi

if grep -Eq 'task=0x|cred=0x|sp=0x|x9=0x|x12=0x' "${preflight_src}"; then
  printf '%s\n' 'preflight output exposes process-local kernel pointers' >&2
  exit 1
fi

if grep -Eq 'PERF_SAMPLE_REGS|PERF_REG_ARM64_|sample_regs_intr' \
  "${preflight_src}"; then
  printf '%s\n' 'preflight requests process-local kernel pointer registers' >&2
  exit 1
fi

if grep -E 'printf|fprintf' "${preflight_src}" | grep -Eq 'sampled_ip'; then
  printf '%s\n' 'preflight prints sampled instruction pointers' >&2
  exit 1
fi

if ! grep -Eq 'sample_type = PERF_SAMPLE_IP \| PERF_SAMPLE_TID' \
  "${preflight_src}"; then
  printf '%s\n' 'preflight sample_type is not IP/TID-only' >&2
  exit 1
fi

for required in \
  'SM-G781W' \
  'G781WVLSLHYJ1' \
  '4.19.113-27223811' \
  '502d0979-02d1-4692-996b-d516dc6d7dce' \
  'TARGET_CAPABILITY_KERNEL_WRITE 0' \
  'TARGET_CAPABILITY_ROOT 0' \
  'TARGET_DIAGNOSTIC_ONLY 1' \
  'PERF_SLIDE_SYMBOL_NAME "__arm64_sys_getresuid"'
do
  if ! grep -Fq "${required}" "${target_header}"; then
    printf 'missing required identity or gate: %s\n' "${required}" >&2
    exit 1
  fi
done

if ! command -v jq >/dev/null 2>&1; then
  printf '%s\n' 'jq required for HYJ1 symbol-map audit' >&2
  exit 1
fi

if ! jq -e '
  (.schema == 1) and
  (.target.model == "SM-G781W") and
  (.target.firmware == "G781WVLSLHYJ1") and
  (.target.kernel_release == "4.19.113-27223811") and
  (.source.raw_image_base == "0x04681000") and
  (.source.disassembly_vma == "0x04681000") and
  (.source.kernel_image_sha256 ==
    "9b13d7b6dd000f72247ed81f13835c9341cbbd3c73def6de9be57f1af74e32df") and
  (.source.corrected_full_symbol_map_sha256 ==
    "bdf70bb8c82b1fbec852390d1adf93a106ee5187356c64a0fad06a9c56737e84") and
  (.source.correction_model_json_sha256 ==
    "611b55f5ea017de99c368e9bac02344e72b200053fb3ef025ccd7dc80f26858d") and
  (.source.released_kernel_sys_c_sha256 ==
    "6da9fb3cd791443beab5621f57741b4e0b0f500f1da1446a47a1d503daafc76c") and
  (.source.correction ==
    "printed symbol name uses the following raw-map line\u0027s address") and
  ((.symbols | length) == 1) and
  (.symbols[0].name == "__arm64_sys_getresuid") and
  (.symbols[0].type == "T") and
  (.symbols[0].raw_map_address == "0x00000000046d54dc") and
  (.symbols[0].image_offset == "0x000544dc") and
  (.symbols[0].instruction_body_end == "0x00054650") and
  (.symbols[0].instruction_body_size == "0x174") and
  (.symbols[0].boundary_marker.image_offset == "0x00054650") and
  (.symbols[0].boundary_marker.word == "0x00be7bad") and
  (.symbols[0].next_symbol.name == "__sys_setresgid") and
  (.symbols[0].next_symbol.raw_map_address == "0x00000000046d5654") and
  (.symbols[0].next_symbol.image_offset == "0x00054654") and
  (.symbols[0].span_to_next_symbol == "0x178") and
  (.symbols[0].sample_window.first_image_offset == "0x000544e4") and
  (.symbols[0].sample_window.last_image_offset == "0x000544fc") and
  (.symbols[0].sample_window.first_symbol_relative_offset == "+0x8") and
  (.symbols[0].sample_window.last_symbol_relative_offset == "+0x20") and
  ([.symbols[0].sample_window.instructions[].image_offset] == [
    "0x000544e4", "0x000544e8", "0x000544ec", "0x000544f0",
    "0x000544f4", "0x000544f8", "0x000544fc"
  ]) and
  ([.symbols[0].sample_window.instructions[].symbol_relative_offset] == [
    "+0x8", "+0xc", "+0x10", "+0x14", "+0x18", "+0x1c", "+0x20"
  ]) and
  ([.symbols[0].sample_window.instructions[].instruction] == [
    "adrp x9, 0x763d000",
    "ldrb w1, [x8, #110]",
    "ldp x13, x11, [x0]",
    "ldr x18, [x8, #64]",
    "ldr w15, [x9, #3328]",
    "ldr w9, [x10, #4]",
    "ldr w12, [x10, #20]"
  ])
' "${symbol_json}" >/dev/null; then
  printf '%s\n' 'HYJ1 symbols.json identity, boundary, or instruction mismatch' >&2
  exit 1
fi

compiler=""
for candidate in cc gcc clang; do
  if command -v "${candidate}" >/dev/null 2>&1; then
    compiler="${candidate}"
    "${candidate}" -std=c11 -Wall -Wextra -Werror -pedantic \
      -I"${profile_dir}" -fsyntax-only "${script_dir}/audit_target.c"
    printf '#include "target.h"\nint main(void) { return 0; }\n' | \
      "${candidate}" -x c -std=c11 -Wall -Wextra -Werror -pedantic \
        -I"${profile_dir}" -fsyntax-only -
  fi
done

if [[ -z "${compiler}" ]]; then
  printf '%s\n' 'C compiler required for HYJ1 target audit' >&2
  exit 1
fi

if ! diff -u \
  <(LC_ALL=C sort -u "${script_dir}/allowed_target_macros.txt") \
  <(awk '/^#define[[:space:]]+[A-Z][A-Z0-9_]*/ { \
      name=$2; sub(/\(.*/, "", name); print name \
    }' "${target_header}" | LC_ALL=C sort -u); then
  printf '%s\n' 'target.h macro inventory changed' >&2
  exit 1
fi

fail_closed() {
  local label="$1"
  shift
  if "${compiler}" -std=c11 -Wall -Wextra -Werror -pedantic "$@" \
    -I"${profile_dir}" -fsyntax-only "${script_dir}/audit_target.c" \
    >/dev/null 2>&1; then
    printf '%s\n' "${label} gate did not fail closed" >&2
    exit 1
  fi
}

fail_closed APP_PAYLOAD -DAPP_PAYLOAD=1
fail_closed G781W_ENABLE_KERNEL_WRITE -DG781W_ENABLE_KERNEL_WRITE=1
fail_closed G781W_ENABLE_ROOT -DG781W_ENABLE_ROOT=1
fail_closed SLIDE_STACK_WRITER -DSLIDE_STACK_WRITER=1
fail_closed APP_RECLAIM_SKB_WRITE_DIAG -DAPP_RECLAIM_SKB_WRITE_DIAG=1
fail_closed APP_PHYS_P0_ORACLE -DAPP_PHYS_P0_ORACLE=1
fail_closed APP_PHYS_VIRTUAL_BASE_ORACLE -DAPP_PHYS_VIRTUAL_BASE_ORACLE=1
fail_closed ROOT_UMH_PATH -DROOT_UMH_PATH=1
fail_closed TARGET_CAPABILITY_KERNEL_WRITE -DTARGET_CAPABILITY_KERNEL_WRITE=1
fail_closed TARGET_CAPABILITY_ROOT -DTARGET_CAPABILITY_ROOT=1

printf '%s\n' 'HYJ1 clean target audit: PASS'
