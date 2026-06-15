#include "MoveGen.h"

#include "Log.h"
#include <algorithm>

MoveGen::MoveGen(Board* board, Move* moves) {
  this->board = board;
  this->moves = moves;
  numMoves = 0;
}

void MoveGen::run() {
  genClones();
  genJumps();
  addNullMove();
}

void MoveGen::genClones() {
  u64 mask = board->pieces[board->side];
  mask |= (mask & ~LEFT_COL) >> 1;
  mask |= (mask & ~RIGHT_COL) << 1;
  mask |= (mask & ~TOP_ROW) >> BOARD_SIZE;
  mask |= (mask & ~BOTTOM_ROW) << BOARD_SIZE;
  mask &= board->empty;
  while (mask) {
    int bit = __builtin_ctzll(mask);
    mask ^= 1ll << bit;
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
    int sbit = __builtin_ctzll(src);
    src ^= 1ll << sbit;
    u64 dest = Board::jumpDomains[sbit] & board->empty;
    while (dest) {
      int dbit = __builtin_ctzll(dest);
      dest ^= 1ll << dbit;
      pushMove(M_JUMP, sbit, dbit);
    }
  }
}

// Preferable when the number of destinations (empty squares) is small.
void MoveGen::genJumpsFromDest() {
  u64 dest = board->empty;
  while (dest) {
    int dbit = __builtin_ctzll(dest);
    dest ^= 1ll << dbit;
    u64 src = Board::jumpDomains[dbit] & board->pieces[board->side];
    while (src) {
      int sbit = __builtin_ctzll(src);
      src ^= 1ll << sbit;
      pushMove(M_JUMP, sbit, dbit);
    }
  }
}

void MoveGen::addNullMove() {
  if (!numMoves) {
    pushMove(M_PASS, 0, 0);
  }
}

void MoveGen::pushMove(u8 type, u8 src, u8 dest) {
  Move& m = moves[numMoves++];
  m.type = type;
  m.src = src;
  m.dest = dest;
  Log::debug("pushed move #%d %s", numMoves - 1, m.toString().c_str());
}
