#!/usr/bin/env bash
set -euo pipefail

script_dir="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
repo_dir="$(cd -- "${script_dir}/.." && pwd)"
ndk_dir="${ANDROID_NDK_HOME:-${HYJ1_NDK_DIR:-}}"

if [[ -z "${ndk_dir}" ]]; then
  printf '%s\n' 'Set ANDROID_NDK_HOME to Android NDK r29.' >&2
  exit 1
fi

clang="${ndk_dir}/toolchains/llvm/prebuilt/linux-x86_64/bin/aarch64-linux-android33-clang"
if [[ ! -x "${clang}" ]]; then
  printf 'Missing compiler: %s\n' "${clang}" >&2
  exit 1
fi

output_dir="${repo_dir}/build/preflight"
output_file="${output_dir}/hyj1-502d-slide-preflight.so"
build_one="${output_dir}/.build-one.so"
build_two="${output_dir}/.build-two.so"
mkdir -p "${output_dir}"

cleanup() {
  rm -f -- "${build_one}" "${build_two}"
}
trap cleanup EXIT

flags=(
  -std=gnu11 -Oz -Wall -Wextra -Werror
  -I"${repo_dir}/profiles/r8q-G781WVLSLHYJ1"
  -fstack-protector-strong -D_FORTIFY_SOURCE=2
  -fPIC -fvisibility=hidden -ffunction-sections -fdata-sections
  -shared -Wl,--gc-sections -Wl,--icf=all -Wl,-z,relro,-z,now
)

"${clang}" "${flags[@]}" -o "${build_one}" "${script_dir}/hyj1_preload_slide.c"
"${clang}" "${flags[@]}" -o "${build_two}" "${script_dir}/hyj1_preload_slide.c"
cmp "${build_one}" "${build_two}"
cp -p "${build_one}" "${output_file}"
sha256sum "${output_file}"
