#pragma once

#include "Board.h"
#include "Constants.h"
#include "MonteCarloNode.h"
#include "MonteCarloScore.h"
#include "Move.h"

class MonteCarloAgent {
public:
  MonteCarloAgent(Board* board, int time);
  Move getMove();

private:
  int time; // time remaining for this move, in milliseconds

  // info about the root and the current node
  Board rootBoard, board;
  MonteCarloNode *root, *node;

  // reusable buffer for move generation
  Move moves[MAX_MOVES];

  void select();
  void expand();
  MonteCarloScore simulate();
  void backpropagate(MonteCarloScore reward);
};
