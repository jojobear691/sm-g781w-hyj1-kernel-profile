# Read-only current-boot preflight

`hyj1_preload_slide.c` samples native `getresuid` with perf and reports the
canonical slide. It is bound to boot
`502d0979-02d1-4692-996b-d516dc6d7dce` and fails closed if the model,
firmware, kernel, boot ID, UID, SELinux state, or acknowledgement differs.

It contains no reclaim, waiter, FOPS, pipe, credential write, usermode-helper,
or root stage. It expires on reboot.

The sampler requests only sampled instruction pointers and thread identity.
It does not request or print task, credential, stack, or other process-local
kernel pointers. User identity is validated separately before sampling and on
every generated syscall. Do not commit runtime logs; only the confirmed slide
and aggregate counts belong in the evidence ledger.

Run it only on the recorded boot, with the explicit read-only acknowledgement:

```sh
G781W_HYJ1_PERF_IP_TID=I_ACCEPT_READ_ONLY_CURRENT_BOOT_PERF_CAPTURE \
  LD_PRELOAD=/data/local/tmp/hyj1-502d-slide-preflight.so \
  /system/bin/true
```

The sampler fails closed if the observed slide differs from the recorded
`0x50000` value. Hit logs print `target_off` (stock-Image offset), not the
sampled kernel PC. The seven-slot map is in [docs/EVIDENCE.md](../docs/EVIDENCE.md).

Exit statuses:

| Status | Meaning |
|---|---|
| `0` | Confirmed current-boot slide `0x50000` on a clean perf stream |
| `2` | No stable exact-window candidate |
| `3` | Invalid perf stream, setup failure, or slide mismatch |
| `125` | Identity or acknowledgement gate failed |
