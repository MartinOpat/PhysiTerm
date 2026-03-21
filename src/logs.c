#include "logs.h"
#include <ncurses.h>
#include <stdarg.h>
#include <stdio.h>

FILE *file;

void init_logging() { file = fopen("debug.log", "w"); }

void close_logging() { fclose(file); }

void log_str(const char *s, ...) {
  va_list args;
  va_start(args, s);
  vfprintf(file, s, args);
  va_end(args);

  fflush(file);
}
