#include "Time.h"

#include <chrono>

u64 Time::t0;

void Time::startClock() {
  t0 = getTimeMillis();
}

u64 Time::checkClock() {
  return getTimeMillis() - t0;
}

u64 Time::getTimeMillis() {
  auto time = std::chrono::system_clock::now();
  auto since_epoch = time.time_since_epoch();
  auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(since_epoch);
  return millis.count();
}
