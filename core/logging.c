#include "logging.h"
#include "../types/types.h"
#include <stdarg.h>
#include <stdio.h>

#define RCL_DBG_MSG_SIZE 256

// file_private char log_file_path[255];
file_private FILE *log_file = NULL;

Status set_logfile(const char *log_file_path) {
  log_file = fopen(log_file_path, "a+");
  if (NULL == log_file) {
    return FAIL;
  }

  return SUCCESS;
}

void close_logfile() { fclose(log_file); }

file_private char *log_level_to_str(LogLevel l) {
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

void log_event(LogLevel log_lvl, char *msg) {
  if (!log_file) {
    // if no log file is initialized, just print to stdout
    printf("%s: %s\n", log_level_to_str(log_lvl), msg);
    return;
  }
  fprintf(log_file, "%s: %s\n", log_level_to_str(log_lvl), msg);
  fflush(log_file); // For now, just write every log
}

void log_formatted_msg(LogLevel log_lvl, char *fmt, ...) {
  char dbg_msg[RCL_DBG_MSG_SIZE] = {0};
  va_list args;
  va_start(args, fmt);
  vsnprintf(dbg_msg, RCL_DBG_MSG_SIZE, fmt, args);
  va_end(args);
  log_event(log_lvl, dbg_msg);
}
