#ifndef LOGGING_H
#define LOGGING_H

#include "../types/types.h"

typedef enum LogLevel { VERBOSE, INFO, WARN, ERROR, CRITICAL } LogLevel;

Status set_logfile(const char *log_file_path);
void log_event(LogLevel log_lvl, char *msg);
void log_formatted_msg(LogLevel log_lvl, char* fmt, ...);
void close_logfile();

#endif
