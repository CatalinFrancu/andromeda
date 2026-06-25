#pragma once

#include "Constants.h"
#include "Move.h"

class TranspositionRecord {
public:
  u64 key;
  int score;
  short move; // index in move list (assumes the move generator is consistent)
  u8 depth;
  u8 type;
};
