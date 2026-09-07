#include "target.h"

_Static_assert(TARGET_PROFILE_INCOMPLETE == 1 &&
                   TARGET_DIAGNOSTIC_ONLY == 1 &&
                   TARGET_PRIVATE_BOOT_FIXTURE == 1 &&
                   TARGET_CURRENT_BOOT_FIELDS_EXPIRE_ON_REBOOT == 1,
               "diagnostic/private/expiry gates changed");
_Static_assert(TARGET_KASLR_ALIGNMENT == 0x8000ULL &&
                   (TARGET_KASLR_ALIGNMENT &
                    (TARGET_KASLR_ALIGNMENT - 1)) == 0,
               "unexpected KASLR alignment");
_Static_assert(TARGET_EVIDENCE_TOTAL_SLIDE == 0x50000ULL,
               "unexpected recorded slide");
_Static_assert((TARGET_EVIDENCE_TOTAL_SLIDE &
                (TARGET_KASLR_ALIGNMENT - 1)) == 0,
               "recorded slide is not aligned");
_Static_assert(TARGET_EVIDENCE_TOTAL_SLIDE <= TARGET_KASLR_SLIDE_MAX,
               "recorded slide exceeds configured range");
_Static_assert(KIMAGE_TEXT_BASE + TARGET_EVIDENCE_TOTAL_SLIDE ==
                   TARGET_EVIDENCE_RUNTIME_TEXT_VA,
               "runtime text address mismatch");

_Static_assert(PERF_SLIDE_PC_0_OFF == 0x544e4ULL &&
                   PERF_SLIDE_PC_1_OFF == PERF_SLIDE_PC_0_OFF + 4 &&
                   PERF_SLIDE_PC_2_OFF == PERF_SLIDE_PC_1_OFF + 4 &&
                   PERF_SLIDE_PC_3_OFF == PERF_SLIDE_PC_2_OFF + 4 &&
                   PERF_SLIDE_PC_4_OFF == PERF_SLIDE_PC_3_OFF + 4 &&
                   PERF_SLIDE_PC_5_OFF == PERF_SLIDE_PC_4_OFF + 4 &&
                   PERF_SLIDE_PC_6_OFF == PERF_SLIDE_PC_5_OFF + 4 &&
                   PERF_SLIDE_FIRST_PC_OFF == PERF_SLIDE_PC_0_OFF &&
                   PERF_SLIDE_LAST_PC_OFF == PERF_SLIDE_PC_6_OFF &&
                   PERF_SLIDE_LAST_PC_OFF == 0x544fcULL,
               "sampled instruction window mismatch");

_Static_assert(KIMAGE_TEXT_BASE + PERF_SLIDE_PC_0_OFF == 0xffffff80080d44e4ULL &&
                   KIMAGE_TEXT_BASE + PERF_SLIDE_PC_1_OFF ==
                       0xffffff80080d44e8ULL &&
                   KIMAGE_TEXT_BASE + PERF_SLIDE_PC_2_OFF ==
                       0xffffff80080d44ecULL &&
                   KIMAGE_TEXT_BASE + PERF_SLIDE_PC_3_OFF ==
                       0xffffff80080d44f0ULL &&
                   KIMAGE_TEXT_BASE + PERF_SLIDE_PC_4_OFF ==
                       0xffffff80080d44f4ULL &&
                   KIMAGE_TEXT_BASE + PERF_SLIDE_PC_5_OFF ==
                       0xffffff80080d44f8ULL &&
                   KIMAGE_TEXT_BASE + PERF_SLIDE_PC_6_OFF ==
                       0xffffff80080d44fcULL,
               "unslid sampled-IP map mismatch");
_Static_assert(TARGET_EVIDENCE_RUNTIME_TEXT_VA + PERF_SLIDE_PC_0_OFF ==
                   0xffffff80081244e4ULL &&
                   TARGET_EVIDENCE_RUNTIME_TEXT_VA + PERF_SLIDE_PC_1_OFF ==
                       0xffffff80081244e8ULL &&
                   TARGET_EVIDENCE_RUNTIME_TEXT_VA + PERF_SLIDE_PC_2_OFF ==
                       0xffffff80081244ecULL &&
                   TARGET_EVIDENCE_RUNTIME_TEXT_VA + PERF_SLIDE_PC_3_OFF ==
                       0xffffff80081244f0ULL &&
                   TARGET_EVIDENCE_RUNTIME_TEXT_VA + PERF_SLIDE_PC_4_OFF ==
                       0xffffff80081244f4ULL &&
                   TARGET_EVIDENCE_RUNTIME_TEXT_VA + PERF_SLIDE_PC_5_OFF ==
                       0xffffff80081244f8ULL &&
                   TARGET_EVIDENCE_RUNTIME_TEXT_VA + PERF_SLIDE_PC_6_OFF ==
                       0xffffff80081244fcULL,
               "current-boot sampled-IP map mismatch");

_Static_assert(TARGET_CAPABILITY_READ_ONLY_SLIDE == 1 &&
                   TARGET_CAPABILITY_TYPED_RECLAIM == 0 &&
                   TARGET_CAPABILITY_SAFE_WRITER == 0 &&
                   TARGET_CAPABILITY_KERNEL_WRITE == 0 &&
                   TARGET_CAPABILITY_ARBITRARY_RW == 0 &&
                   TARGET_CAPABILITY_ROOT == 0,
               "unsafe capability promotion");

int main(void) { return 0; }
