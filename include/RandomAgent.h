#pragma once

#include "Board.h"
#include "Constants.h"
#include "Move.h"

class RandomAgent {
public:
  Board* board;
  Move moves[MAX_MOVES];

  RandomAgent(Board* board);
  Move getMove();
};
