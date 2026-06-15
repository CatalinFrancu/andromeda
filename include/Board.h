#pragma once

#include "Constants.h"
#include "Move.h"
#include <string>

class Board {
public:
  // For every square, precompute the squares at distance 2.
  static u64 jumpDomains[BOARD_SIZE * BOARD_SIZE];

  u64 pieces[2];
  u64 empty;
  bool side;

  static void precomputeJumpDomains();

  void readFromStdin();
  void print();
  std::string translateMove(Move m);

  int getNumPiecesToMove();
  int getNumEmpty();

private:
  void printTopSeparatorLine();
  void printMiddleSeparatorLine();
  void printBottomSeparatorLine();
  void printSeparatorLine(const char* left, const char* center,
                          const char* right);
};
