#pragma once

#include "Constants.h"
#include <string>

class Move {
public:
  u8 type;
  u8 src, dest;

  std::string toString();
  friend bool operator==(const Move& a, const Move& b);
};
