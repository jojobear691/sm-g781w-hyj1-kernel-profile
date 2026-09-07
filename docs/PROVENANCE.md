# Provenance and redistribution notes

The profile records values supplied from a local SM-G781W/HYJ1 research
workspace. The source artifacts themselves are not included.

| Derived material | Recorded source |
|---|---|
| Symbol offsets | Extracted HYJ1 `kernel.bin`, SHA-256 `9b13d7b6dd000f72247ed81f13835c9341cbbd3c73def6de9be57f1af74e32df` |
| Current-boot slide | User-supplied read-only preflight output for the recorded boot ID |

The corrected symbol JSON (SHA-256
`611b55f5ea017de99c368e9bac02344e72b200053fb3ef025ccd7dc80f26858d`) is also
local-only. This profile keeps the seven sampled instruction offsets and does
not copy kallsyms names or other symbol table entries into the tree.

The repository does not contain the kernel Image, firmware package, runtime
logs, Image-derived byte samples, device serial number, IMEI, or process-local
pointers.

The hashes bind claims to named local artifacts; they do not independently
prove that the artifacts were acquired correctly or that every derived value
matches the shipped kernel.
