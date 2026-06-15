#include "Board.h"

#include "AnsiColors.h"
#include "Log.h"
#include <stdio.h>
#include <string>

void Board::readFromStdin() {
  int x;
  scanf("%d ", &x);
  side = x;

  piece[0] = piece[1] = empty = 0;

  for (int r = 0; r < BOARD_SIZE; r++) {
    for (int c = 0; c < BOARD_SIZE; c++) {
      u64 mask = 1ll << (r * BOARD_SIZE + c);
      char ch = getchar();
      switch (ch) {
        case 'x': piece[0] |= mask; break;
        case 'o': piece[1] |= mask; break;
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
        const char* color = (piece[0] & mask)
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
