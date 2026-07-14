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
  u64 startTime;
  u64 posCount;  // positions evaluated

  Move moves[MAX_ALPHA_BETA_DEPTH + 1][MAX_MOVES];

  Move iterativeDeepening();
  void alphaBetaWrapper(int depth, int numMoves, short& move, int& score);
  int alphaBeta(Board b, int depth, int alpha, int beta);
  bool bringBestMoveFirst(TranspositionRecord rec, int depth, int numMoves);
  void prefetchChild(Board& src, Move move, Board& dest);
  // Returns 0 when a = x, x when a = 0, a otherwise.
  int twiddle(int a, int x);

  TranspositionRecord ttProbe(u64 hash);
  void ttPrefetch(u64 hash);
  void ttAdd(u64 key, short score, short move, u8 depth, u8 type);
  int ttEvictions();

  void logStats(int depth, int score);
};
