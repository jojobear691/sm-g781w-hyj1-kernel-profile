# Security policy

This project supports only the exact `SM-G781W` / `G781WVLSLHYJ1` identity
documented in the repository.

Report security-sensitive findings privately through a GitHub Security
Advisory after the remote repository is created. Include firmware identity,
artifact hashes, tool versions, and reproducible read-only evidence.

Do not publish live kernel-write primitives, privilege-escalation chains,
policy-bypass instructions, secrets, personal device identifiers, or
proprietary firmware images in issues or pull requests.

Unsupported-device use, flashing, exploitation, and destructive testing are
outside this repository's scope.

The repository includes offsets and other security-sensitive research data.
Compile-time capability flags do not make those values safe to publish. Keep
the repository private until disclosure and firmware-redistribution questions
have been resolved.
