#pragma once

#include "Board.h"
#include "Move.h"

class MoveGen {
public:
  Board* board;
  Move* moves;
  int numMoves;
  u64 jmask; // The mask of jump sources/destinations left to explore.
  u64 jbit; // The bit currently being explored.
  u64 neighborMask; // The destinations/sources mask for jbit.

  MoveGen(Board* board, Move* moves);
  void genAll();
  bool exists(int i);
  Move get(int i); // No existence checks.
  void randomizeMoves(); // After generating them!
  Move getRandomMove();


private:
  u64 cloneMask; // squares where we can clone
  u64 jumpMask;  // squares where we can jump

  void genMasks();
  void genUpTo(int i);
  void genNext();
  void genJumps();
  void genNextJumpFromSrc();
  void genNextJumpToDest();

  void pushMove(u8 src, u8 dest);
};
