# Security policy

This project supports only the exact `SM-G781W` / `G781WVLSLHYJ1` identity
documented in the repository.

Report security-sensitive findings privately through a GitHub Security
Advisory. Include firmware identity, artifact hashes, tool versions, and
reproducible read-only evidence.

Do not publish live kernel-write primitives, privilege-escalation chains,
policy-bypass instructions, secrets, personal device identifiers, or
proprietary firmware images in issues or pull requests.

Unsupported-device use, flashing, exploitation, and destructive testing are
outside this repository's scope.

This public repository intentionally contains only the sanitized, read-only
evidence subset described in `docs/RESEARCH_STATUS.md`. Do not expand it with
private logs, payloads, physical placement data, or proprietary material.
Compile-time capability flags do not make sensitive values safe to publish.
