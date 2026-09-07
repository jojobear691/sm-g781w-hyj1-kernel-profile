#ifndef R8Q_G781W_HYJ1_DIAGNOSTIC_TARGET_H
#define R8Q_G781W_HYJ1_DIAGNOSTIC_TARGET_H

/*
 * Boot-bound, read-only diagnostic profile for Samsung SM-G781W / r8q.
 * Firmware: G781WVLSLHYJ1
 * Kernel:   4.19.113-27223811
 * Boot:     502d0979-02d1-4692-996b-d516dc6d7dce
 *
 * This header contains only identity gates and the instruction offsets needed
 * by the IP/TID-only perf slide sampler. It is not an exploit target header.
 * Current-boot fields expire on reboot.
 */

#define TARGET_PROFILE_SCHEMA 2
#define TARGET_PROFILE_INCOMPLETE 1
#define TARGET_DIAGNOSTIC_ONLY 1
#define TARGET_PRIVATE_BOOT_FIXTURE 1
#define TARGET_CURRENT_BOOT_FIELDS_EXPIRE_ON_REBOOT 1

#if defined(TARGET_CAPABILITY_READ_ONLY_SLIDE) || \
    defined(TARGET_CAPABILITY_TYPED_RECLAIM) || \
    defined(TARGET_CAPABILITY_SAFE_WRITER) || \
    defined(TARGET_CAPABILITY_KERNEL_WRITE) || \
    defined(TARGET_CAPABILITY_ARBITRARY_RW) || \
    defined(TARGET_CAPABILITY_ROOT)
#error "target capability flags must not be overridden"
#endif

#define TARGET_CAPABILITY_READ_ONLY_SLIDE 1
#define TARGET_CAPABILITY_TYPED_RECLAIM 0
#define TARGET_CAPABILITY_SAFE_WRITER 0
#define TARGET_CAPABILITY_KERNEL_WRITE 0
#define TARGET_CAPABILITY_ARBITRARY_RW 0
#define TARGET_CAPABILITY_ROOT 0

#define BUILD_VARIANT_LABEL \
  "r8q-G781WVLSLHYJ1-4.19-502d-read-only-slide"
#define BUILD_FINGERPRINT \
  "samsung/r8qcsx/r8q:13/TP1A.220624.014/G781WVLSLHYJ1:user/release-keys"
#define TARGET_EXPECTED_MODEL "SM-G781W"
#define TARGET_EXPECTED_DISPLAY "TP1A.220624.014.G781WVLSLHYJ1"
#define TARGET_EXPECTED_FINGERPRINT BUILD_FINGERPRINT
#define TARGET_EXPECTED_KERNEL_RELEASE "4.19.113-27223811"
#define TARGET_MEASURED_BOOT_ID \
  "502d0979-02d1-4692-996b-d516dc6d7dce"

/* Refuse silent promotion into a writer or privilege-handoff build. */
#if defined(APP_PAYLOAD) && APP_PAYLOAD
#error "HYJ1 diagnostic profile is not an app-payload target"
#endif
#if defined(G781W_ENABLE_KERNEL_WRITE) && G781W_ENABLE_KERNEL_WRITE
#error "HYJ1 controlled kernel write is not established"
#endif
#if defined(G781W_ENABLE_ROOT) && G781W_ENABLE_ROOT
#error "HYJ1 privilege handoff is not established"
#endif
#if defined(SLIDE_STACK_WRITER) || \
    defined(APP_RECLAIM_SKB_WRITE_DIAG) || \
    defined(APP_PHYS_P0_ORACLE) || \
    defined(APP_PHYS_VIRTUAL_BASE_ORACLE) || \
    defined(ROOT_UMH_PATH)
#error "HYJ1 writer/oracle/root knobs are unavailable in this profile"
#endif

#define KIMAGE_TEXT_BASE 0xffffff8008080000ULL
#define TARGET_KASLR_SLIDE_MAX 0x3fe00000ULL
#define TARGET_KASLR_ALIGNMENT 0x00008000ULL

/* User-supplied read-only result for TARGET_MEASURED_BOOT_ID. */
#define TARGET_EVIDENCE_TOTAL_SLIDE 0x00050000ULL
#define TARGET_EVIDENCE_RUNTIME_TEXT_VA 0xffffff80080d0000ULL

/* Exact stock-Image symbol and instruction offsets used only for sampled-IP
 * matching. Unslid and current-boot VAs are mapped in docs/EVIDENCE.md.
 */
#define PERF_SLIDE_SYMBOL_NAME "__arm64_sys_getresuid"
#define PERF_SLIDE_SYMBOL_OFF 0x000544dcULL
#define PERF_SLIDE_SYMBOL_BODY_END_OFF 0x00054650ULL
#define PERF_SLIDE_NEXT_SYMBOL_OFF 0x00054654ULL
#define PERF_SLIDE_PC_0_OFF 0x000544e4ULL
#define PERF_SLIDE_PC_1_OFF 0x000544e8ULL
#define PERF_SLIDE_PC_2_OFF 0x000544ecULL
#define PERF_SLIDE_PC_3_OFF 0x000544f0ULL
#define PERF_SLIDE_PC_4_OFF 0x000544f4ULL
#define PERF_SLIDE_PC_5_OFF 0x000544f8ULL
#define PERF_SLIDE_PC_6_OFF 0x000544fcULL
#define PERF_SLIDE_FIRST_PC_OFF PERF_SLIDE_PC_0_OFF
#define PERF_SLIDE_LAST_PC_OFF PERF_SLIDE_PC_6_OFF

#define PERF_SLIDE_REQUIRED_ACK \
  "I_ACCEPT_READ_ONLY_CURRENT_BOOT_PERF_CAPTURE"

#endif /* R8Q_G781W_HYJ1_DIAGNOSTIC_TARGET_H */
