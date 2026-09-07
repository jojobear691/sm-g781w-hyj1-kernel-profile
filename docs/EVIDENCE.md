# Evidence ledger

## Exact target identity

| Item | Recorded value |
|---|---|
| Model | `SM-G781W` |
| Firmware | `G781WVLSLHYJ1` |
| Kernel release | `4.19.113-27223811` |
| Extracted HYJ1 `kernel.bin` SHA-256 | `9b13d7b6dd000f72247ed81f13835c9341cbbd3c73def6de9be57f1af74e32df` |
| Corrected symbol JSON SHA-256 | `611b55f5ea017de99c368e9bac02344e72b200053fb3ef025ccd7dc80f26858d` |

The source artifacts are not distributed here. Their hashes bind the recorded
instruction offsets to named local evidence but are not independent proof of
correct extraction.

## Sampled instruction window

The read-only sampler matches only this contiguous 7-instruction stock-Image
window. Each slot is one AArch64 instruction (4 bytes). The sampler uses the
offsets for sampled-IP matching while the process executes native `getresuid`;
it does not export other stock-Image symbols or kernel structure layouts.

The exact kallsyms / `stext`-relative symbol name for this window is not
recorded in the repository. Provenance binds the offsets to a local corrected
symbol JSON (SHA-256 `611b55f5ea017de99c368e9bac02344e72b200053fb3ef025ccd7dc80f26858d`)
and HYJ1 `kernel.bin` that are not distributed here. Naming that symbol without
those artifacts would be an unverified claim.

Formulas for this firmware identity:

- unslid VA = `KIMAGE_TEXT_BASE` (`0xffffff8008080000`) + stock offset
- current-boot VA = unslid VA + recorded slide `0x50000`
- equivalently, current-boot VA = `TARGET_EVIDENCE_RUNTIME_TEXT_VA`
  (`0xffffff80080d0000`) + stock offset

Current-boot VAs are valid only for boot
`502d0979-02d1-4692-996b-d516dc6d7dce` and expire on reboot.

| Slot | Stock offset | Unslid VA | Current-boot VA |
|---|---|---|---|
| `PERF_SLIDE_PC_0_OFF` | `0x544e4` | `0xffffff80080d44e4` | `0xffffff80081244e4` |
| `PERF_SLIDE_PC_1_OFF` | `0x544e8` | `0xffffff80080d44e8` | `0xffffff80081244e8` |
| `PERF_SLIDE_PC_2_OFF` | `0x544ec` | `0xffffff80080d44ec` | `0xffffff80081244ec` |
| `PERF_SLIDE_PC_3_OFF` | `0x544f0` | `0xffffff80080d44f0` | `0xffffff80081244f0` |
| `PERF_SLIDE_PC_4_OFF` | `0x544f4` | `0xffffff80080d44f4` | `0xffffff80081244f4` |
| `PERF_SLIDE_PC_5_OFF` | `0x544f8` | `0xffffff80080d44f8` | `0xffffff80081244f8` |
| `PERF_SLIDE_PC_6_OFF` | `0x544fc` | `0xffffff80080d44fc` | `0xffffff80081244fc` |

`make audit` checks this arithmetic. The preflight log prints `target_off`
(stock offset), never the sampled kernel instruction pointer.

## Current boot

The user-supplied read-only preflight output for boot
`502d0979-02d1-4692-996b-d516dc6d7dce` reported one candidate, slide
`0x50000`, nine hits across five instruction offsets, and zero lost,
malformed, throttled, or overrun records. Process-local pointers are excluded
from this repository.

The preserved sampler is narrower than that supplied run: it requests only
sampled IP and TID, never kernel pointer registers, and fails closed unless
the result matches the recorded `0x50000` slide. This boot fixture expires on
reboot.
