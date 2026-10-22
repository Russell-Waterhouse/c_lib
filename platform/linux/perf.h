#ifndef LINUX_PERF_H

unsigned long long rdtsc();
double get_cycles_per_ns(unsigned long long sample_time_ns);

#endif // !LINUX_PERF_H
