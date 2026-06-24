#pragma once

#include "Constants.h"
#include "Move.h"

class TranspositionRecord {
public:
  int score;
  Move move;
  u8 depth;
  u8 type;
};
