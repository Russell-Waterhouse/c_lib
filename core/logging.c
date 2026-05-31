#include "../types/types.h"
#include <stdio.h>
#include "logging.h"

// file_private char log_file_path[255];
file_private FILE* log_file;

Status set_logfile(const char* log_file_path) {
  log_file = fopen(log_file_path, "a+");
  if (NULL == log_file) {
    return FAIL;
  }

  return SUCCESS;
}

void close_logfile() {
  fclose(log_file);
}

file_private char* log_level_to_str(LogLevel l) {
  switch (l) {
    case VERBOSE:
      return "verbose";
    case INFO:
      return "info";
    case WARN:
      return "warning";
    case ERROR:
      return "error";
    case CRITICAL:
      return "critical";
    default:
      return "unknown";
  }
}

void log_event(LogLevel log_lvl, char* msg) {
  fprintf(log_file, "LOG LEVEL %s: %s\n", log_level_to_str(log_lvl), msg);
}

