#!/usr/bin/env bash
set -euo pipefail

script_dir="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
repo_dir="$(cd -- "${script_dir}/.." && pwd)"
profile_dir="${repo_dir}/profiles/r8q-G781WVLSLHYJ1"

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

if grep -Eq 'task=0x|cred=0x|sp=0x|x9=0x|x12=0x' \
  "${repo_dir}/preflight/hyj1_preload_slide.c"; then
  printf '%s\n' 'preflight output exposes process-local kernel pointers' >&2
  exit 1
fi

if grep -Eq 'PERF_SAMPLE_REGS|PERF_REG_ARM64_|sample_regs_intr' \
  "${repo_dir}/preflight/hyj1_preload_slide.c"; then
  printf '%s\n' 'preflight requests process-local kernel pointer registers' >&2
  exit 1
fi

for compiler in cc gcc clang; do
  if command -v "${compiler}" >/dev/null 2>&1; then
    "${compiler}" -std=c11 -Wall -Wextra -Werror -pedantic \
      -I"${profile_dir}" -fsyntax-only "${script_dir}/audit_target.c"
    printf '#include "target.h"\nint main(void) { return 0; }\n' | \
      "${compiler}" -x c -std=c11 -Wall -Wextra -Werror -pedantic \
        -I"${profile_dir}" -fsyntax-only -
  fi
done

if ! diff -u \
  <(LC_ALL=C sort -u "${script_dir}/allowed_target_macros.txt") \
  <(awk '/^#define[[:space:]]+[A-Z][A-Z0-9_]*/ { \
      name=$2; sub(/\(.*/, "", name); print name \
    }' "${profile_dir}/target.h" | LC_ALL=C sort -u); then
  printf '%s\n' 'target.h macro inventory changed' >&2
  exit 1
fi

if cc -std=c11 -Wall -Wextra -Werror -pedantic -DAPP_PAYLOAD=1 \
  -I"${profile_dir}" -fsyntax-only "${script_dir}/audit_target.c" \
  >/dev/null 2>&1; then
  printf '%s\n' 'APP_PAYLOAD gate did not fail closed' >&2
  exit 1
fi

printf '%s\n' 'HYJ1 clean target audit: PASS'
