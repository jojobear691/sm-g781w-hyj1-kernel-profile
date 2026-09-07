# Provenance and redistribution notes

The profile records values supplied from a local SM-G781W/HYJ1 research
workspace. The source artifacts themselves are not included.

| Derived material | Recorded source |
|---|---|
| Symbol offsets and disassembly | Extracted HYJ1 `kernel.bin`, SHA-256 `9b13d7b6dd000f72247ed81f13835c9341cbbd3c73def6de9be57f1af74e32df` |
| Symbol name and corrected boundaries | Full corrected symbol map, SHA-256 `bdf70bb8c82b1fbec852390d1adf93a106ee5187356c64a0fad06a9c56737e84` |
| Syscall source cross-check | Released `kernel/sys.c`, SHA-256 `6da9fb3cd791443beab5621f57741b4e0b0f500f1da1446a47a1d503daafc76c` |
| Current boot slide | Two direct IP/TID-only read-only measurements for boot `d2bf…` |
| Previous boot slide | User-supplied read-only preflight output for boot `502d…` |

The local corrected-symbol JSON (SHA-256
`611b55f5ea017de99c368e9bac02344e72b200053fb3ef025ccd7dc80f26858d`) records
the raw Image base and correction rule but does not contain the sampled
symbol. The full corrected map establishes `__arm64_sys_getresuid` and its
next-symbol boundary. The repository's `symbols.json` is a minimal,
one-symbol derivative rather than a copy of either local source artifact.

The current and previous aggregate boot measurements are recorded in
`boot-fixtures.json`. Raw device logs and process-local pointers are not
included.

The repository does not contain the kernel Image, firmware package, full
symbol map, runtime logs, Image-derived byte samples, device serial number,
IMEI, or process-local pointers.

The hashes bind claims to named local artifacts; they do not independently
prove that the artifacts were acquired correctly or that every derived value
matches the shipped kernel.
