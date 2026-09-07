# Evidence index

Updated: 2026-09-07

This index ties the public HYJ1 research summary to exact artifacts. A hash is
an identity anchor, not independent proof that a claim is correct. “Private”
means the artifact is retained in the owner-controlled research workspace but
is not redistributed by this public repository.

## Public and reproducible artifacts

| Artifact | SHA-256 | Status |
|---|---|---|
| [`target.h`](../profiles/r8q-G781WVLSLHYJ1/target.h) | `48fe3ef27600910ddc426bcb15e0b2b6071dde48ffa2fc2cbdc656ffb4abf561` | Public current-boot diagnostic profile |
| [`boot-fixtures.json`](../profiles/r8q-G781WVLSLHYJ1/boot-fixtures.json) | `83feeb0a39d6268d64e2111a39e28ea690316a826a249f066669545ac6a0f75c` | Public aggregate boot history |
| [`symbols.json`](../profiles/r8q-G781WVLSLHYJ1/symbols.json) | `9b2aeb2d6157d0b37ea6581fa61595ee32f6de870b640578c764d8d1b478d453` | Public one-symbol map |
| [`hyj1_preload_slide.c`](../preflight/hyj1_preload_slide.c) | `45049313420898e35ebd5957382d77f03c6436f0f74dfc122a3b59737a428c65` | Public IP/TID-only source |
| NDK r29 AArch64 preflight build | `6428c6a052e47406e50f82e50e63721ef7ea71b6d29d4be257aeae38ae0c670a` | Reproducible current-boot build output; not committed |

The preflight hash above is the clean local NDK r29 result from this public
source. GitHub Actions independently compiles the same source on every push.

## Firmware, source, and toolchain provenance

| Evidence | SHA-256 | Distribution |
|---|---|---|
| Extracted stock boot-image compressed member | `d067a7bf62af0e172dcaec17310bac650bcd592ab87f2693881f507e631c0367` | Private |
| Decompressed stock boot image | `2366e35d99ea7b7fc39cbb5ec43d5eda50faf0d7357fce784a013180535e776c` | Private |
| Canadian exact-build released-source update package | `5087867a305b5a522dedf375af3d4c9acbb047d485fdce84014d44084f3d2489` | Private |
| Exact-build kernel-source overlay archive member | `e0a3edd26ab4004f3a673a56db45993a1cbcc17c0064ff980a9b20b9520b4cb0` | Private |
| Vendor source-layering/build note archive member | `a0b3e55e68b5a5d8dd414ea408de9c6b33ef03ef1163cf54afc9b16d79db4ea6` | Private |
| Reconstructed layered kernel-source Makefile | `d7f9cb790c68a280b20db07c7bcd3f8a3cce3ad99494f72f849c6f3a24a6b79d` | Private |
| Exact-target released-source defconfig | `7695964f8a017552bf0372992a2eaf58d25e08789705896b0af4ca5c70f97ed2` | Private |
| Android NDK r29 metadata | `716f3518a923198cfab037abb32dc3f1b1f7e9a9dcdcda6b66be5906215d2658` | Private; revision `29.0.14206865` |

## Exact-build analysis anchors

| Evidence | SHA-256 | Distribution |
|---|---|---|
| Exact extracted stock kernel Image | `9b13d7b6dd000f72247ed81f13835c9341cbbd3c73def6de9be57f1af74e32df` | Private |
| Raw shifted embedded kallsyms | `520dd21f3f0ffe5d070d213e80eaa2bc7d5fac92d78de88f1561b4088f8f11b5` | Private; not an address oracle |
| Curated correction-model JSON | `611b55f5ea017de99c368e9bac02344e72b200053fb3ef025ccd7dc80f26858d` | Private |
| Full corrected symbol map | `bdf70bb8c82b1fbec852390d1adf93a106ee5187356c64a0fad06a9c56737e84` | Private |
| Configured-source layout ledger | `2c91fd6e6aaddb1462d9534328cddba26323b8526df267de9025b0c43b4bcebf` | Private; source-derived, not shipped DWARF |
| Released `kernel/sys.c` | `6da9fb3cd791443beab5621f57741b4e0b0f500f1da1446a47a1d503daafc76c` | Private source cross-check |
| Sealed retained source closure | `0b9453e042def47733c35dc91f3474201b7d712ace36883489fc1c9302f0dee9` | Private |

## Exact-phone witness anchors

| Evidence | SHA-256 | Proven boundary |
|---|---|---|
| Exact-phone checkpoint | `516c3fcd52303f4b68dec873fd06ede78ac4e81d6deb638958051aeb17a0c960` | Consolidated live summary; latest raw transcript is not yet sealed |
| Bounded release/splice witness | `e29973de059a22415a7b9c33272e839b761c0ae7f6e93679216a5bd2d3e8dddb` | Complete group release and immediate 64/64 AF_UNIX behavior; summary-pinned, no dedicated sealed raw-run bundle |
| Standalone AF_UNIX witness | `6804e9c6052e7cec80997b90ea5f5e1647092dbbe1a4cfc78309503174383f29` | Benign transfer witness |
| Combined lifecycle witness | `a528b3d6a7ca6719653eb93631941ca70be4556bfcf2ac9c758e00b1f808e0ca` | Compat32/lifecycle prerequisites only |
| Current `d2bf…` slide confirmation | `6428c6a052e47406e50f82e50e63721ef7ea71b6d29d4be257aeae38ae0c670a` | IP/TID-only; `0xe8000`, 10 hits across 4 offsets, clean stream, exit 0 |
| Preserved boot-bound `502d…` sampler | `33436bd9f8074c3e3141dca749eaa6aa0de6f7e78ec15068b31459adf79430f8` | Expired boot-bound sampler; run result came from user-supplied output |

These witness binaries and raw device logs are private. Their hashes are
published for provenance; they are not public payload releases.

## Analysis and control records

| Record | SHA-256 | Result |
|---|---|---|
| Live-readiness status v2 | `4caa06f69f8c03a6b8a64bc9c78f2f5d90c9818a4b49911f87311510195edde3` | No-go: missing typed-consumer physical identity |
| Typed-consumer trace-contract status | `7a723f7691c700f71251e75d8305737636f50b909f2ee0bcb240bb378048f167` | Contract/verifier complete; exact-phone trace unavailable |
| Owned order-3 observer-census status | `8677fd8b36faa2f3a5d8aac36610b43372846ef69100bc1d9230511e502af5f2` | Zero fully qualified observers among 17 candidates |
| Compat32 host-closure status | `757636945fb54c42fe5099a367bfa15bdd8edcbb0562b962bd15f9033652f848` | Seven of seven model seams pass; host-only |
| QEMU repeatability status | `8bfdd91066fc271956a6ef5f7d7bfd6736beb573cbb8322c259a0c575fdb758c` | Typed same-extent reuse in 10/10 controlled boots |
| QEMU repeatability manifest | `1a8fe12525b4e30e6ce78b50fba92db44c5439d9236a72896da51b013f4f6931` | Private bundle manifest |
| Complete private handoff | `06644712f6727db95c5afac16a64ac7574e0aad8febd2fcb91fdb2815154b65a` | Full private history; intentionally not copied here |
| Private kernel-offset ledger | `f98f149323cd32e7cdd976e4f4d6d9ea96caef584f50795a88b4438337393eaa` | Static/layout/boot arithmetic ledger; intentionally not copied here |

The QEMU result uses a privileged, explicit cache transition. It validates
the model and instrumentation; it is not evidence of natural stock-phone
reuse or a phone success probability.

## Retired and foreign artifacts

The exact hashes for panic-producing and wrong-device artifacts are kept in
[`KNOWN_BAD.md`](KNOWN_BAD.md). They must not be selected by filename or
directory name. In particular, the known `f275cde4…` binary is an S25/`pa3q`
artifact and is not compatible with this S20/HYJ1 profile.
