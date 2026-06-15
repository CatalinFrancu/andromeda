#include "Board.h"

#include "AnsiColors.h"
#include "Log.h"
#include <stdio.h>
#include <sstream>
#include <string>

u64 Board::jumpDomains[BOARD_SIZE * BOARD_SIZE];

void Board::precomputeJumpDomains() {
  const int DELTA[16][2] = {
    {-2, -2}, {-2, -1}, {-2, 0}, {-2, +1}, {-2, +2},
    {-1, -2}, {-1, +2},
    {0, -2}, {0, +2},
    {+1, -2}, {+1, +2},
    {+2, -2}, {+2, -1}, {+2, 0}, {+2, +1}, {+2, +2} };
  for (int r = 0; r < BOARD_SIZE; r++) {
    for (int c = 0; c < BOARD_SIZE; c++) {
      int bit = r * BOARD_SIZE + c;
      for (int i = 0; i < 16; i++) {
        int r2 = r + DELTA[i][0];
        int c2 = c + DELTA[i][1];
        if ((r2 >= 0) && (r2 < BOARD_SIZE) &&
            (c2 >= 0) && (c2 < BOARD_SIZE)) {
          jumpDomains[bit] |= 1ll << (r2 * BOARD_SIZE + c2);
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
  printTopSeparatorLine();

  for (int r = 0; r < BOARD_SIZE; r++) {
    std::string s = "│";
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
  std::string s = left;
  for (int i = 0; i < BOARD_SIZE; i++) {
    s += "───";
    s += (i < BOARD_SIZE - 1) ? center : right;
  }
  Log::debug(s.c_str());
}

int Board::getNumPiecesToMove() {
  return __builtin_popcountll(pieces[side]);
}

int Board::getNumEmpty() {
  return __builtin_popcountll(empty);
}
