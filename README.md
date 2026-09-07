# SM-G781W HYJ1 kernel profile

This repository preserves reproducible, read-only evidence for the Samsung
Galaxy S20 FE 5G `SM-G781W` (`r8q`) on firmware `G781WVLSLHYJ1`, kernel
`4.19.113-27223811`.

It is not a root package, exploit, flasher, arbitrary-write implementation,
or compatibility profile for another device or firmware. Write-capable builds
remain blocked in `target.h`.

This repository contains a boot-bound diagnostic and the seven instruction
offsets it needs. They resolve to `__arm64_sys_getresuid`; the named symbol,
instruction map, stock offsets, unslid VAs, and current-boot VAs are in
[docs/EVIDENCE.md](docs/EVIDENCE.md) and the machine-readable
[`symbols.json`](profiles/r8q-G781WVLSLHYJ1/symbols.json). It deliberately
excludes reclaim, writer, credential, file-operations, pipe, SELinux,
usermode-helper, and root-handoff metadata. Keep the repository private until
responsible-disclosure review is complete.

## Status

| Item | Status |
|---|---|
| Exact firmware identity | Recorded |
| Sampled symbol | `__arm64_sys_getresuid` (`+0x8` through `+0x20`) |
| Read-only sampled instruction offsets | Recorded against hash-bound evidence |
| Current boot ID | `502d0979-02d1-4692-996b-d516dc6d7dce` |
| Current read-only slide | `0x50000` |
| Typed same-extent reclaim | Unproven |
| Safe writer / kernel write / root | Unavailable |

Current-boot fields expire on reboot.

## Repository layout

- `profiles/r8q-G781WVLSLHYJ1/` — minimal target header and one-symbol JSON map.
- `tools/` — numeric, contamination, and fail-closed capability audits.
- `preflight/` — boot-bound, read-only perf slide sampler source.
- `docs/` — evidence ledger, sampled-offset map, and known-bad hashes.

## Validate

```sh
make audit
```

The audit requires Bash, `jq`, and a C11 compiler.

The audit checks arithmetic, the named symbol record, instruction window and
VA map, capability gates, header self-containment, personal-data patterns,
and foreign-device contamination. It also verifies that writer, root, oracle,
and `APP_PAYLOAD` builds fail closed. This is a consistency audit, not
independent validation against the proprietary kernel Image.

CI runs `make audit` and an NDK r29 AArch64 preflight compile on every push
and pull request.

## Build the read-only preflight

Set `ANDROID_NDK_HOME` to Android NDK r29, then run:

```sh
make preflight
```

The output is an Android API 33 AArch64 shared library. The repository does
not distribute compiled payloads or proprietary firmware.

See [CONTRIBUTING.md](CONTRIBUTING.md), [SECURITY.md](SECURITY.md), and
[docs/PROVENANCE.md](docs/PROVENANCE.md). No license has been selected yet.
