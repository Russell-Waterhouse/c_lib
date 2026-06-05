#include "../types/types.h"
#include <stdio.h>
#include "logging.h"

// file_private char log_file_path[255];
file_private FILE* log_file = NULL;

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
      return "VERBOSE";
    case INFO:
      return "INFO";
    case WARN:
      return "WARNING";
    case ERROR:
      return "ERROR";
    case CRITICAL:
      return "CRITICAL";
    default:
      return "unknown";
  }
}

void log_event(LogLevel log_lvl, char* msg) {
  if (!log_file) {
    // if no log file is initialized, just print to stdout
    printf("%s: %s\n", log_level_to_str(log_lvl), msg);
    return;
  }
  fprintf(log_file, "%s: %s\n", log_level_to_str(log_lvl), msg);
  fflush(log_file); // For now, just write every log
}

