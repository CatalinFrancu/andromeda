#include "MoveGen.h"

#include "Log.h"
#include "Util.h"
#include <algorithm>

MoveGen::MoveGen(Board* board, Move* moves) {
  this->board = board;
  this->moves = moves;
  numMoves = 0;

  genMasks();

  jmask = (board->getNumPiecesToMove() < board->getNumEmpty())
    ? board->pieces[board->side]
    : jumpMask;
  jbit = 0;
  neighborMask = 0;
}

void MoveGen::genMasks() {
  cloneMask = board->pieces[board->side];
  cloneMask |= (cloneMask & ~LEFT_COL) >> 1;
  cloneMask |= (cloneMask & ~RIGHT_COL) << 1;
  cloneMask |= (cloneMask & ~TOP_ROW) >> BOARD_SIZE;
  cloneMask |= (cloneMask & ~BOTTOM_ROW) << BOARD_SIZE;
  cloneMask &= board->empty;

  jumpMask = ALLOW_JUMPS_OVER_CLONES
    ? board->empty
    : (board->empty & ~cloneMask);
}

void MoveGen::genAll() {
  genUpTo(MAX_MOVES);
}

bool MoveGen::exists(int i) {
  genUpTo(i);
  return numMoves >= i + 1;
}

Move MoveGen::get(int i) {
  genUpTo(i);
  return moves[i];
}

void MoveGen::genUpTo(int i) {
  while ((jmask || neighborMask) && (numMoves < i + 1)) {
    genNext();
  }
}

void MoveGen::genNext() {
  if (cloneMask) {
    int bit = __builtin_ctzll(cloneMask);
    cloneMask ^= 1ll << bit;
    pushMove(bit, bit);
  } else if (board->getNumPiecesToMove() < board->getNumEmpty()) {
    genNextJumpFromSrc();
  } else {
    genNextJumpToDest();
  }
}

// Preferable when the number of sources (player pieces) is small.
void MoveGen::genNextJumpFromSrc() {
  while (!neighborMask && jmask) {
    jbit = 63 - __builtin_clzll(jmask);
    jmask ^= 1ll << jbit;
    neighborMask = Board::jumpDomains[jbit] & jumpMask;
  }

  if (neighborMask) {
    int bit = 63 - __builtin_clzll(neighborMask);
    neighborMask ^= 1ll << bit;
    pushMove(jbit, bit);
  }
}

// Preferable when the number of destinations (empty squares) is small.
void MoveGen::genNextJumpToDest() {
  while (!neighborMask && jmask) {
    jbit = 63 - __builtin_clzll(jmask);
    jmask ^= 1ll << jbit;
    neighborMask = Board::jumpDomains[jbit] & board->pieces[board->side];
  }

  if (neighborMask) {
    int bit = 63 - __builtin_clzll(neighborMask);
    neighborMask ^= 1ll << bit;
    pushMove(bit, jbit);
  }
}

void MoveGen::randomizeMoves() {
  std::shuffle(moves, moves + numMoves, Util::rng);
}

Move MoveGen::getRandomMove() {
  int sumWeights = 0;
  for (int i = 0; i < numMoves; i++) {
    sumWeights += moves[i].getMonteCarloWeight();
  }

  int left = Util::rand(0, sumWeights - 1);
  int i = 0;
  while (left >= moves[i].getMonteCarloWeight()) {
    left -= moves[i].getMonteCarloWeight();
    i++;
  }

  return moves[i];
}

void MoveGen::pushMove(u8 src, u8 dest) {
  Move& m = moves[numMoves++];
  m.src = src;
  m.dest = dest;
}
