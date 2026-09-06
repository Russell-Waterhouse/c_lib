#include <stdio.h>
#include "../platform/linux/perf.h"
#include "../types/types.h"
#include "../core/pretty_print.h"

Status test_perf() {
  const unsigned long long sample_time_ns = 500000;
  double cycles_per_ns = get_cycles_per_ns(sample_time_ns);
  char msg[1024];
  snprintf(msg, sizeof(msg),
           "Based on a sample of %lluns we would expect your processor to be running at %.3f GHZ. manually verify that's close to correct.",
           sample_time_ns, (double)cycles_per_ns);
  print_yellow(msg);
  return SUCCESS;
}
