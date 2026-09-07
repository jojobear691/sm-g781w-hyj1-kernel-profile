# Contributing

Contributions may add documentation corrections, provenance records, hashes,
read-only extraction tooling, compile-time guards, or validation tests.

Every change must state the exact model/build/region, source hashes, tool
versions, and whether each claim is observed, derived, or unverified.

Pull requests must pass `make audit` and must not add copied cross-device
offsets, arbitrary kernel writers, root payloads, KDP/SELinux bypasses,
persistence mechanisms, destructive commands, proprietary firmware, or live
exploitation instructions. Keep the sampled window limited to the seven
`getresuid` instruction offsets documented in `docs/EVIDENCE.md`.

Before submitting, confirm:

- [ ] The target identity is exactly `SM-G781W / G781WVLSLHYJ1`.
- [ ] New evidence is hash-pinned and classified.
- [ ] No unsupported security capability is claimed.
- [ ] No personal paths, secrets, or expired process pointers are included.
- [ ] No proprietary byte samples are added without documented provenance and
      a confirmed redistribution basis.
