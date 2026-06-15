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
  Log::debug("======== side: %d", side);
  printSeparatorLine();

  for (int r = 0; r < BOARD_SIZE; r++) {
    std::string s = "|";
    for (int c = 0; c < BOARD_SIZE; c++) {
      u64 mask = 1ll << (r * BOARD_SIZE + c);
      s += " ";
      if (piece[0] & mask) {
        s += AnsiColors::PIECE[0];
        s += "o";
        s += AnsiColors::DEFAULT;
      } else if (piece[1] & mask) {
        s += AnsiColors::PIECE[1];
        s += "o";
        s += AnsiColors::DEFAULT;
      } else {
        s += " ";
      }
      s += " |";
    }
    Log::debug(s.c_str());
    printSeparatorLine();
  }
}

void Board::printSeparatorLine() {
  std::string s = "+";
  for (int i = 0; i < BOARD_SIZE; i++) {
    s += "---+";
  }
  Log::debug(s.c_str());
}
