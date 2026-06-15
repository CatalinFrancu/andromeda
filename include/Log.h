#pragma once

#include <stdarg.h>

class Log {
public:
  static void fatal(const char* fmt, ...);
  static void error(const char* fmt, ...);
  static void warn(const char* fmt, ...);
  static void success(const char* fmt, ...);
  static void info(const char* fmt, ...);
  static void debug(const char* fmt, ...);

 private:
  static char buffer[10'000];
  static void write(int level, const char* ansiColor, const char* fmt, va_list args);

  // Replace AnsiColors::DEFAULT with our current logging color. This allows
  // us to print colored messages inside a log message that has its own color.
  static void interceptDefaultColor(const char* ansiColor);
};
