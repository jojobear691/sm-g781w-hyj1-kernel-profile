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

The read-only sampler matches only this contiguous stock-Image window:

`0x544e4`, `0x544e8`, `0x544ec`, `0x544f0`, `0x544f4`, `0x544f8`,
`0x544fc`.

No other stock-Image symbols or kernel structure layouts are exported by the
active profile.

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
