#include "Log.h"

#include "AnsiColors.h"
#include "Config.h"
#include "Constants.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char Log::buffer[10'000];

void Log::fatal(const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  write(LOG_FATAL, AnsiColors::FATAL, fmt, args);
  va_end(args);
  exit(1);
}

void Log::error(const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  write(LOG_ERROR, AnsiColors::ERROR, fmt, args);
  va_end(args);
}

void Log::warn(const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  write(LOG_WARN, AnsiColors::WARN, fmt, args);
  va_end(args);
}

void Log::success(const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  write(LOG_SUCCESS, AnsiColors::SUCCESS, fmt, args);
  va_end(args);
}

void Log::info(const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  write(LOG_INFO, AnsiColors::INFO, fmt, args);
  va_end(args);
}

void Log::debug(const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  write(LOG_DEBUG, AnsiColors::DEBUG, fmt, args);
  va_end(args);
}

void Log::write(int level, const char* ansiColor, const char* fmt, va_list args) {
  if (level <= LOG_LEVEL) {
    vsprintf(buffer, fmt, args);
    interceptDefaultColor(ansiColor);
    fprintf(stderr, "%s%s%s\n", ansiColor, buffer, AnsiColors::DEFAULT);
  }
}

void Log::interceptDefaultColor(const char* ansiColor) {
  int len = strlen(AnsiColors::DEFAULT);
  for (char* s = buffer; *s; s++) {
    if (!strncmp(s, AnsiColors::DEFAULT, len)) {
      strncpy(s, ansiColor, len);
    }
  }
}
