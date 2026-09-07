#!/usr/bin/env bash
set -euo pipefail

script_dir="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
repo_dir="$(cd -- "${script_dir}/.." && pwd)"
profile_dir="${repo_dir}/profiles/r8q-G781WVLSLHYJ1"
target_header="${profile_dir}/target.h"
preflight_src="${repo_dir}/preflight/hyj1_preload_slide.c"

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
  'TARGET_DIAGNOSTIC_ONLY 1'
do
  if ! grep -Fq "${required}" "${target_header}"; then
    printf 'missing required identity or gate: %s\n' "${required}" >&2
    exit 1
  fi
done

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
