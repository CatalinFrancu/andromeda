#include "Board.h"

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
      u64 mask = 1ll << (r * BOARD_SIZE + c);
      char ch = getchar();
      switch (ch) {
        case 'x': pieces[0] |= mask; break;
        case 'o': pieces[1] |= mask; break;
        case '.': empty |= mask; break;
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
  }

  // Fill the destination.
  pieces[side] ^= 1ll << m.dest;
  empty ^= 1ll << m.dest;

  // Flip the neighbors.
  pieces[side] |= pieces[!side] & cloneDomains[m.dest];
  pieces[!side] &= ~cloneDomains[m.dest];

  side = !side;
}

int Board::eval() {
  return __builtin_popcountll(pieces[side]) - __builtin_popcountll(pieces[!side]);
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

int Board::getNumPiecesToMove() {
  return __builtin_popcountll(pieces[side]);
}

int Board::getNumEmpty() {
  return __builtin_popcountll(empty);
}

int Board::estimateRemainingMoves() {
  return getNumEmpty() / FRACTION_CLONES;
}

u64 Board::getHashCode() {
  return
    pieces[0] * HASH1 +
    pieces[1] * HASH2 +
    empty * HASH3 +
    side * HASH4;
}
