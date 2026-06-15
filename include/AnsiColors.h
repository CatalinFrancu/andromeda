#pragma once

#include "Constants.h"

class AnsiColors {
public:
  // Keep these in sync with Constants.h::LOG_*
  static const char FATAL[10];
  static const char ERROR[10];
  static const char WARN[10];
  static const char SUCCESS[10];
  static const char INFO[10];
  static const char DEBUG[10];

  static const char DEFAULT[10];

  static const char PIECE[2][10];
};
