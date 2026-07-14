#include "Time.h"

#include <chrono>

u64 Time::endTime;
int Time::pollCnt;
bool Time::alarmWentOff;

u64 Time::getTimeMillis() {
  auto time = std::chrono::system_clock::now();
  auto since_epoch = time.time_since_epoch();
  auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(since_epoch);
  return millis.count();
}

void Time::setAlarm(int millis) {
  endTime = millis + getTimeMillis();
  pollCnt = 0;
}

bool Time::checkAlarm() {
  if (alarmWentOff) {
    // Once it goes off, it will always go off.
    return true;
  }

  if (++pollCnt < POLL_EVERY) {
    return false;
  }

  pollCnt = 0;
  alarmWentOff = (getTimeMillis() >= endTime);

  return alarmWentOff;
}
