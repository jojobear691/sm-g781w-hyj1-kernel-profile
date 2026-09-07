# SM-G781W HYJ1 kernel profile

[![audit](https://github.com/jojobear691/sm-g781w-hyj1-kernel-profile/actions/workflows/audit.yml/badge.svg)](https://github.com/jojobear691/sm-g781w-hyj1-kernel-profile/actions/workflows/audit.yml)

This repository records an effort to root the Samsung Galaxy S20 FE 5G
`SM-G781W` (`r8q`) on firmware `G781WVLSLHYJ1`, kernel
`4.19.113-27223811`.

The current public snapshot preserves a sanitized, reproducible, read-only
evidence record collected toward that goal. It does not yet provide a working
root method; write-capable builds remain blocked in `target.h`. It is not a
flasher or a compatibility profile for another device or firmware.

**Payload compatibility:** the repository's `target.h` is a preflight-only
diagnostic header. It is not interchangeable with the full target contract
used by Root-My-Galaxy/GhostLock payload sources and intentionally does not
define the `FAKE_WAITER_*`, `P0_*`, FOPS, PIPE, or ROOT macro families.

This repository contains a boot-bound diagnostic and the seven instruction
offsets it needs. They resolve to `__arm64_sys_getresuid`; the named symbol,
instruction map, stock offsets, unslid VAs, and current-boot VAs are in
[docs/EVIDENCE.md](docs/EVIDENCE.md) and the machine-readable
[`symbols.json`](profiles/r8q-G781WVLSLHYJ1/symbols.json). It deliberately
excludes deployable reclaim, writer, credential, file-operations, pipe,
SELinux, usermode-helper, and root-handoff implementation details.

The complete verified progression—including the exact-phone slab-release and
AF_UNIX results, compat32 lifecycle work, instrumented QEMU controls, current
blocker, and retired paths—is summarized in
[docs/RESEARCH_STATUS.md](docs/RESEARCH_STATUS.md). Exact public and private
evidence anchors are indexed in
[docs/EVIDENCE_INDEX.md](docs/EVIDENCE_INDEX.md).

The 37-package private analysis corpus is identified, without redistributing
its contents, in [docs/ARCHIVE_INDEX.md](docs/ARCHIVE_INDEX.md).

## Current status

| Item | Status |
|---|---|
| Exact firmware identity | Recorded |
| Exact live kernel | `4.19.113-27223811 #1 SMP PREEMPT Mon Oct 13 16:40:47 KST 2025` |
| Sampled symbol | `__arm64_sys_getresuid` (`+0x8` through `+0x20`) |
| Read-only sampled instruction offsets | Recorded against hash-bound evidence |
| Active-at-capture boot fixture | `d2bf6c9f-4232-4c2f-ad3a-21137bc68380` |
| Confirmed read-only slide | `0xe8000`; valid only for that boot |
| Previous boot fixture | `502d0979-02d1-4692-996b-d516dc6d7dce` / `0x50000` |
| Controlled group and full slab return | Proven on phone: 32/32 objects, 1152 → 1120 |
| Immediate AF_UNIX behavior | Proven on phone: 64/64 bytes verified |
| Compat32 prerequisite lifecycle | Witness passed; not a writer |
| Typed same-extent handoff on stock phone | Unproven |
| Safe writer / kernel write / root | Unavailable |

The `d2bf…` slide was measured once and then confirmed by a second clean run
on the same boot. The first run recorded six hits across four offsets; the
confirmation recorded ten hits across four offsets. Both streams had zero
lost, malformed, throttled, or overrun records. Every fixture remains
boot-bound and expires on reboot; history is preserved in
[`boot-fixtures.json`](profiles/r8q-G781WVLSLHYJ1/boot-fixtures.json).

## Repository layout

- `profiles/r8q-G781WVLSLHYJ1/` — target header, boot history, and one-symbol map.
- `tools/` — numeric, contamination, and fail-closed capability audits.
- `preflight/` — boot-bound, read-only perf slide sampler source.
- `docs/RESEARCH_STATUS.md` — complete publishable research status.
- `docs/EVIDENCE_INDEX.md` — artifact hashes and evidence boundaries.
- `docs/ARCHIVE_INDEX.md` — private analysis-package manifest index.
- `docs/` — provenance, sampled-offset map, and known-bad hashes.

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
