#pragma once

#include "Constants.h"

class Time {
public:
  static void startClock();

  // returns the number of milliseconds elapsed since startClock()
  static u64 stopClock();

private:
  static u64 t0;

  static u64 getTimeMillis();
};
