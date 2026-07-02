#include "MoveGen.h"

#include "Log.h"
#include "Util.h"
#include <algorithm>

MoveGen::MoveGen(Board* board, Move* moves) {
  this->board = board;
  this->moves = moves;
  numMoves = 0;
}

void MoveGen::run() {
  genMasks();
  genClones();
  genJumps();
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

void MoveGen::genClones() {
  u64 m = cloneMask;
  while (m) {
    int bit = __builtin_ctzll(m);
    m ^= 1ll << bit;
    pushMove(M_CLONE, 0, bit);
  }
}

void MoveGen::genJumps() {
  if (board->getNumPiecesToMove() < board->getNumEmpty()) {
    genJumpsFromSrc();
  } else {
    genJumpsFromDest();
  }
}

// Preferable when the number of sources (player pieces) is small.
void MoveGen::genJumpsFromSrc() {
  u64 src = board->pieces[board->side];
  while (src) {
    int sbit = 63 - __builtin_clzll(src);
    src ^= 1ll << sbit;
    u64 dest = Board::jumpDomains[sbit] & jumpMask;
    while (dest) {
      int dbit = 63 - __builtin_clzll(dest);
      dest ^= 1ll << dbit;
      pushMove(M_JUMP, sbit, dbit);
    }
  }
}

// Preferable when the number of destinations (empty squares) is small.
void MoveGen::genJumpsFromDest() {
  u64 dest = jumpMask;
  while (dest) {
    int dbit = 63 - __builtin_clzll(dest);
    dest ^= 1ll << dbit;
    u64 src = Board::jumpDomains[dbit] & board->pieces[board->side];
    while (src) {
      int sbit = 63 - __builtin_clzll(src);
      src ^= 1ll << sbit;
      pushMove(M_JUMP, sbit, dbit);
    }
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

void MoveGen::pushMove(u8 type, u8 src, u8 dest) {
  Move& m = moves[numMoves++];
  m.type = type;
  m.src = src;
  m.dest = dest;
  // Log::debug("pushed move #%d %s", numMoves - 1, m.toString().c_str());
}
