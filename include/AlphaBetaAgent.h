#pragma once

#include "Board.h"
#include "Constants.h"
#include "Move.h"
#include "TranspositionRecord.h"
#include "TranspositionTable.h"

class AlphaBetaAgent {
public:
  AlphaBetaAgent(Board* board, int time);
  Move getMove();

private:
  Board* board;
  TranspositionTable* tt;
  int time; // time remaining for this move, in milliseconds
  int millis, prevMillis; // time used for the previous two iterations
  u64 posCount;  // positions evaluated
  u64 moveCount; // number of calls to the move generator
  u64 ttCutoffs; // num cutoffs produced by the transposition table

  Move moves[MAX_ALPHA_BETA_DEPTH + 1][MAX_MOVES];

  Move iterativeDeepening();
  bool haveTime();
  // Returns true iff it had enough time to complete.
  bool alphaBetaWrapper(int depth, int numMoves, short& move, int& score);
  int alphaBeta(Board b, int depth, int alpha, int beta);
  void logStats(int depth, int score, int millis);

  TranspositionRecord ttProbe(u64 hash);
  void ttPrefetch(u64 hash);
  void ttAdd(u64 key, short score, short move, u8 depth, u8 type);
  int ttEvictions();
};
