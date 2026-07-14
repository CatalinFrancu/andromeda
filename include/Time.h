#pragma once

#include "Constants.h"

class Time {
public:
  static u64 getTimeMillis();
  static void setAlarm(int millis);
  static bool checkAlarm();

private:
  static u64 endTime;
  static int pollCnt;
  static bool alarmWentOff;
};
