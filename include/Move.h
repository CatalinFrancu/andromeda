#pragma once

#include "Constants.h"
#include <string>

class Move {
public:
  u8 type;
  u8 src, dest;

  std::string toString();
};
