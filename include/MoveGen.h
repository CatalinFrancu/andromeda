#pragma once

#include "Board.h"
#include "Move.h"

class MoveGen {
public:
  Board* board;
  Move* moves;
  int numMoves;

  MoveGen(Board* board, Move* moves);
  void run();
  void randomizeMoves(); // After generating them!

private:
  void genClones();
  void genJumps();
  void genJumpsFromSrc();
  void genJumpsFromDest();

  // When there are no other moves.
  void addNullMove();

  void pushMove(u8 type, u8 src, u8 dest);
};
