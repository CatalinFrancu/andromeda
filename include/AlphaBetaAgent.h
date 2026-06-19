#pragma once

#include "Board.h"
#include "Constants.h"
#include "Move.h"

class AlphaBetaAgent {
public:
  Board* board;

  AlphaBetaAgent(Board* board);
  Move getMove();

private:
  Move moves[MAX_ALPHA_BETA_DEPTH + 1][MAX_MOVES];
  u64 posCount, moveCount;

  void alphaBetaWrapper(int depth, Move& move, int& score);
  int alphaBeta(Board b, int depth, int alpha, int beta);
  void logStats(int depth, int score);
};
