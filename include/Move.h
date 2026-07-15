#pragma once

#include "Constants.h"
#include <string>

class Move {
public:
  u8 src, dest;

  bool isClone();
  bool isJump();
  int getMonteCarloWeight();
  std::string toString();
  friend bool operator==(const Move& a, const Move& b);
};
