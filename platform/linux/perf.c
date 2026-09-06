#include "perf.h"
#include <bits/time.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include <x86intrin.h> // Required header for GCC/Clang intrinsics

#define ONE_SECOND_NS (1000 * 1000 * 1000)

unsigned long long rdtsc() { return __rdtsc(); }

double get_cycles_per_ns(unsigned long long sample_time_ns) {
  if (sample_time_ns > ONE_SECOND_NS) {
    puts("I didn't design for this");
    exit(1);
  }

  double cycles_per_ns = 0;
  unsigned long long start = rdtsc();
  struct timespec start_os_time;
  struct timespec tp;
  long elapsed_ns = 0;
  clock_gettime(CLOCK_MONOTONIC, &start_os_time);
  while (elapsed_ns < sample_time_ns) {
    clock_gettime(CLOCK_MONOTONIC, &tp);
    elapsed_ns = tp.tv_nsec - start_os_time.tv_nsec;
  }
  unsigned long long end = rdtsc();
  unsigned long long total_cycles = end - start;
  long total_cycles_long = (long) total_cycles;

  if ((unsigned long long)total_cycles_long != total_cycles) {
    puts("oops");
    exit(1);
  }
  cycles_per_ns = (double)total_cycles_long / (double)elapsed_ns;
  printf("for %llu total cycles in %lu ns (%lu ms), we calculate %f cycles/ns "
         "for a frequency of %f\n",
         total_cycles, elapsed_ns, elapsed_ns / 1000, cycles_per_ns,
         cycles_per_ns * ONE_SECOND_NS);
  return cycles_per_ns;
}
