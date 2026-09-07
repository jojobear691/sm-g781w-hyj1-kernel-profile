# HYJ1 research status

Updated: 2026-09-07

This page consolidates the verified, owner-authorized research for the
Samsung Galaxy S20 FE 5G `SM-G781W` (`r8q`) on firmware
`G781WVLSLHYJ1`.

This is an evidence record, not a root release or exploitation guide. No
validated kernel write or `uid=0` result exists for this phone.

## Exact target

| Field | Verified value |
|---|---|
| Device | Samsung Galaxy S20 FE 5G |
| Model / codename | `SM-G781W` / `r8q` |
| Android | 13 |
| Firmware | `G781WVLSLHYJ1` |
| Kernel | `4.19.113-27223811 #1 SMP PREEMPT Mon Oct 13 16:40:47 KST 2025` |
| Kernel compiler | Android NDK Clang `10.0.6` |
| Preserved post-run state | UID 2000, SELinux Enforcing |
| Current evidence state | `kernel_write=0 root=0` |

## Verified milestones

| Milestone | Status | Evidence boundary |
|---|---|---|
| Exact firmware and kernel identity | **Proven** | Exact Image, released source, and live device identity agree. |
| Exact-build vulnerability semantics | **Proven** | This does not establish a complete exploit chain. |
| Corrected embedded symbol map | **Proven** | The Samsung CFP/JOPP name/address shift was identified and corrected. |
| Read-only sampled symbol | **Proven** | The seven sampled offsets are inside `__arm64_sys_getresuid`, from `+0x8` through `+0x20`. |
| Current read-only displacement | **Proven for one boot** | Two IP/TID-only runs on fixture `d2bf…` measured and confirmed slide `0xe8000` with clean perf streams. |
| Previous read-only displacement | **Historical fixture** | User-supplied output for `502d…` reported slide `0x50000`, nine hits across five offsets, and a clean perf stream. |
| Controlled object-group collection | **Proven on phone** | A complete 32/32 `mm_struct` group was collected. |
| Complete slab return | **Proven on phone** | The observed slab total changed from 1152 to 1120 after all references closed. |
| Immediate AF_UNIX transfer | **Proven on phone** | A prepared 64-byte message moved and verified 64/64 immediately after the final close, with clean descriptor accounting. |
| Compat32 prerequisite lifecycle | **Proven as a witness** | ARM32 and combined lifecycle witnesses passed; this is not a writer. |
| Intended consumer source route | **Derived** | Matching source reaches the expected page-fragment consumer, but stock-phone function entry was not traceable. |
| Consumer allocation order | **Unknown on phone** | Observed only in the instrumented model. |
| Released/consumed physical identity | **Unknown on phone** | No suitable unprivileged typed-consumer identity observer was found. |
| Controlled writer | **Not implemented** | Static design and lifecycle work do not constitute a live writer. |
| Reversible kernel write | **Missing** | No change/readback/restore result exists. |
| Privilege handoff and root | **Missing** | Every preserved post-run S20 state remained UID 2000 with SELinux Enforcing. |

The strongest exact-phone conclusion is a complete controlled release
followed immediately by verified AF_UNIX behavior. It does not prove that the
consumer received the same released physical extent.

## Boot-fixture status

The public preflight is bound to boot
`d2bf6c9f-4232-4c2f-ad3a-21137bc68380`. An initial IP/TID-only measurement
found one `0xe8000` candidate with six hits across four offsets. A second
build pinned that value and confirmed it with ten hits across four offsets,
exit status 0, and the same boot ID before and after. Both streams had zero
lost, malformed, throttled, or overrun records. The runtime text base for
this boot is `0xffffff8008168000`.

The post-check remained UID 2000, SELinux Enforcing, and boot-complete. The
precheck reported about 7 hours 20 minutes of uptime and boot reason
`kernel_panic,null`, establishing that an automatic restart separated this
boot from the previous `502d…` fixture.

The previous fixture remains part of the evidence history. User-supplied
terminal output for boot `502d0979-02d1-4692-996b-d516dc6d7dce` established
slide `0x50000` with nine target hits across five offsets and a clean stream.
The local legacy sampler bundle does not contain that device run log. Both
fixtures are recorded in
[`boot-fixtures.json`](../profiles/r8q-G781WVLSLHYJ1/boot-fixtures.json).

## Exact-build analysis completed

The stock ARM64 kernel Image was extracted and hash-pinned. The embedded raw
symbol data has a one-entry CFP/JOPP name/address shift; a corrected full map
and a curated corrected subset were produced. The public repository contains
only the narrow one-symbol derivative needed by the read-only sampler.

The sampled function is:

- symbol: `__arm64_sys_getresuid`
- stock-Image symbol offset: `0x544dc`
- sampled window: `0x544e4` through `0x544fc`
- relative window: `+0x8` through `+0x20`
- decoded body end: `0x54650` exclusive
- next corrected symbol: `__sys_setresgid` at `0x54654`

See [EVIDENCE.md](EVIDENCE.md) and the machine-readable
[`symbols.json`](../profiles/r8q-G781WVLSLHYJ1/symbols.json).

Configured-source structure-layout work also exists, but it is a separate
evidence class. There is no trusted shipped HYJ1 `vmlinux`, `System.map`, BTF,
or DWARF file, so source-derived layouts are not presented as shipped debug
truth in this public profile.

## Instrumented and offline results

The host-only compat32 lifecycle model passes all seven declared lifecycle
seams. It validates the design model, not a deployed writer.

A typed release/consumer trace contract and verifier were completed. The
contract requires typed events, allocation order, all-CPU capture, zero loss,
and exact extent identity. Its synthetic positive fixture passes and 17
rejection cases are handled, but no exact-build instrumented phone trace is
available.

A 17-candidate observer census found zero unprivileged, owner-controlled
observers satisfying the required allocation order, allocator domain, and
exact identity properties.

The frozen QEMU model reproduced typed same-extent reuse on 10/10 fresh boots
with no trace loss or overruns. That model deliberately uses a privileged,
explicit cache transition. It validates the instrumentation and verifier,
not natural stock-phone allocator behavior or a stock-phone success rate.

## Current blocker

The first decisive blocker is:

`MISSING_TYPED_CONSUMER_PHYSICAL_IDENTITY`

Changing timing, spray counts, offsets, or stack geometry cannot substitute
for observing both sides of the same typed handoff. Until an authorized
instrumented exact-build environment records the released and consumed
extents in one lossless trace, the evidence-backed result remains:

`allocation_order_observed=0 exact_pfn_proof=0 kernel_write=0 root=0`

## Retired paths

Two S20 test families caused kernel panics and remain permanently retired:

| Artifact SHA-256 | Disposition |
|---|---|
| `133dbf8b56b69db7ea9b6fc9687ddc62730a084d1444c56fd19cf90e0a8b8d7a` | Never rerun |
| `a0d661a9e63822b5ec2df26f1a22eaff7d0cb671c754f88684104ef895503c3b` | Never rerun |

See [KNOWN_BAD.md](KNOWN_BAD.md). Artifact names alone are not trustworthy;
classification is based on full hashes and provenance.

## Repository boundary

The complete private research workspace contains raw logs and additional
analysis that should not be copied into a public repository. This public
record intentionally does not distribute:

- firmware or the proprietary kernel Image;
- the full symbol or structure-layout corpus;
- process-local pointers, physical addresses, or crash placement data;
- raw device logs, identifiers, or personal filesystem paths;
- payload binaries, writer/root stages, or exploit execution instructions;
- expired boot values presented as reusable data;
- material copied from a different device or firmware.

The hashes in [EVIDENCE_INDEX.md](EVIDENCE_INDEX.md) authenticate the key
private source records without redistributing them. The broader 37-package
analysis corpus is recorded in [ARCHIVE_INDEX.md](ARCHIVE_INDEX.md). The
supported public deliverable remains a reproducible, fail-closed, read-only
HYJ1 diagnostic profile.
