#include "Board.h"

#pragma GCC target("popcnt")

#include "AnsiColors.h"
#include "Config.h"
#include "Log.h"
#include <stdio.h>
#include <sstream>
#include <string>

const int Board::CLONE_DELTA[8][2] = {
  {-1, -1}, {-1, 0}, {-1, +1},
  {0, -1}, {0, +1},
  {+1, -1}, {+1, 0}, {+1, +1},
};

const int Board::JUMP_DELTA[16][2] = {
  {-2, -2}, {-2, -1}, {-2, 0}, {-2, +1}, {-2, +2},
  {-1, -2}, {-1, +2},
  {0, -2}, {0, +2},
  {+1, -2}, {+1, +2},
  {+2, -2}, {+2, -1}, {+2, 0}, {+2, +1}, {+2, +2},
};

u64 Board::cloneDomains[BOARD_SIZE * BOARD_SIZE];
u64 Board::jumpDomains[BOARD_SIZE * BOARD_SIZE];

void Board::precomputeDomains() {
  precomputeTypeDomains(Board::CLONE_DELTA, 8, Board::cloneDomains);
  precomputeTypeDomains(Board::JUMP_DELTA, 16, Board::jumpDomains);
}

void Board::precomputeTypeDomains(const int (*delta)[2], int count, u64* dest) {
  for (int r = 0; r < BOARD_SIZE; r++) {
    for (int c = 0; c < BOARD_SIZE; c++) {
      int bit = r * BOARD_SIZE + c;
      for (int i = 0; i < count; i++) {
        int r2 = r + delta[i][0];
        int c2 = c + delta[i][1];
        if ((r2 >= 0) && (r2 < BOARD_SIZE) &&
            (c2 >= 0) && (c2 < BOARD_SIZE)) {
          dest[bit] |= 1ll << (r2 * BOARD_SIZE + c2);
        }
      }
    }
  }
}

void Board::readFromStdin() {
  int x;
  scanf("%d ", &x);
  side = x;

  pieces[0] = pieces[1] = empty = 0;

  for (int r = 0; r < BOARD_SIZE; r++) {
    for (int c = 0; c < BOARD_SIZE; c++) {
      int bit = r * BOARD_SIZE + c;
      u64 mask = 1ll << bit;
      char ch = getchar();
      switch (ch) {
        case 'x':
          pieces[0] |= mask;
          boardCoefs[0] += BOARD_COEFS[bit];
          break;
        case 'o':
          pieces[1] |= mask;
          boardCoefs[1] += BOARD_COEFS[bit];
          break;
        case '.':
          empty |= mask;
          break;
      }
    }
    getchar(); // '\n'
  }
}

void Board::print() {
  Log::debug("    a   b   c   d   e   f   g");
  printTopSeparatorLine();

  for (int r = 0; r < BOARD_SIZE; r++) {
    std::string s;
    s = (char)('1' + r);
    s += " │";
    for (int c = 0; c < BOARD_SIZE; c++) {
      u64 mask = 1ll << (r * BOARD_SIZE + c);
      if (empty & mask) {
        s += "   ";
      } else {
        const char* color = (pieces[0] & mask)
          ? AnsiColors::PIECE[0]
          : AnsiColors::PIECE[1];
        s += color;
        s += " ⬤ ";
        s += AnsiColors::DEFAULT;
      }
      s += "│";
    }
    Log::debug(s.c_str());
    if (r < BOARD_SIZE - 1) {
      printMiddleSeparatorLine();
    }
  }
  printBottomSeparatorLine();
  Log::debug("Side to move: %s⬤%s", AnsiColors::PIECE[side], AnsiColors::DEFAULT);
  Log::debug("popcount %d/%d coefs %d/%d group %d/%d eval %d",
             __builtin_popcountll(pieces[side]),
             __builtin_popcountll(pieces[!side]),
             boardCoefs[side],
             boardCoefs[!side],
             groupEval(side),
             groupEval(!side),
             eval());
}

void Board::printTopSeparatorLine() {
  printSeparatorLine("┌", "┬", "┐");
}

void Board::printMiddleSeparatorLine() {
  printSeparatorLine("├", "┼", "┤");
}

void Board::printBottomSeparatorLine() {
  printSeparatorLine("└", "┴", "┘");
}

void Board::printSeparatorLine(const char* left, const char* center,
                               const char* right) {
  std::string s = "  ";
  s += left;
  for (int i = 0; i < BOARD_SIZE; i++) {
    s += "───";
    s += (i < BOARD_SIZE - 1) ? center : right;
  }
  Log::debug(s.c_str());
}

void Board::makeMove(Move m) {
  if (m.type == M_JUMP) {
    // Clear the source.
    pieces[side] ^= 1ll << m.src;
    empty ^= 1ll << m.src;
    boardCoefs[side] -= BOARD_COEFS[m.src];
  }

  // Fill the destination.
  pieces[side] ^= 1ll << m.dest;
  empty ^= 1ll << m.dest;
  boardCoefs[side] += BOARD_COEFS[m.dest];

  // Flip the neighbors.
  u64 opp_neighbors = pieces[!side] & cloneDomains[m.dest];
  pieces[side] ^= opp_neighbors;
  pieces[!side] ^= opp_neighbors;
  for (u64 x = opp_neighbors; x; x &= (x - 1)) {
    int sq = __builtin_ctzll(x);
    boardCoefs[!side] -= BOARD_COEFS[sq];
    boardCoefs[side] += BOARD_COEFS[sq];
  }

  side = !side;
}

int Board::eval() {
  int delta = __builtin_popcountll(pieces[side]) - __builtin_popcountll(pieces[!side]);
  return
    delta * POP_COEF +
    (boardCoefs[side] - boardCoefs[!side]) +
    (groupEval(side) - groupEval(!side)) * GROUP_COEF;
}

int Board::groupEval(bool side) {
  u64 m = pieces[side];

  // There's no point in counting the same pair twice, so we only look for
  // neighbors in four directions: W, NW, N and NE.
  return
    __builtin_popcountll(m & ((m & ~LEFT_COL) >> 1)) +
    __builtin_popcountll(m & ((m & ~TOP_ROW & ~LEFT_COL) >> (BOARD_SIZE + 1))) +
    __builtin_popcountll(m & ((m & ~TOP_ROW) >> BOARD_SIZE)) +
    __builtin_popcountll(m & ((m & ~TOP_ROW & ~RIGHT_COL) >> (BOARD_SIZE - 1)));
}

int Board::finalEval() {
  // The side to move has no move. The other side gets all the empty squares.
  int us = __builtin_popcountll(pieces[side]);
  int them = __builtin_popcountll(pieces[!side]) + __builtin_popcountll(empty);
  if (us == them) {
    return 0;
  } else if (us > them) {
    return WIN_SCORE + us;
  } else {
    return -WIN_SCORE - them;
  }
}

MonteCarloScore Board::getMonteCarloScore() {
  double p[2];
  p[side] = __builtin_popcountll(pieces[side]);
  p[!side] = __builtin_popcountll(pieces[!side]) + __builtin_popcountll(empty);

  if (p[0] == p[1]) {
    return MonteCarloScore(0.5, 0.5);
  } else if (p[0] > p[1]) {
    return MonteCarloScore(1.0, 0.0);
  } else {
    return MonteCarloScore(0.0, 1.0);
  }
}

int Board::getNumPiecesToMove() {
  return __builtin_popcountll(pieces[side]);
}

int Board::getNumEmpty() {
  return __builtin_popcountll(empty);
}

int Board::estimateRemainingMoves() {
  // Divide in 2 because the opponent can also create clones and drive the
  // game forward.
  int n = getNumEmpty() / (2 * FRACTION_CLONES);
  return (n < 1) ? 1 : n;
}

u64 Board::getHashCode() {
  return
    (HASH1 * pieces[0] +
     HASH2 * pieces[1] +
     HASH3 * side) >> 32;
}
