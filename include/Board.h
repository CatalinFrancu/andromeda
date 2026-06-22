#pragma once

#include "Constants.h"
#include "Move.h"
#include <string>

class Board {
public:
  // For every square, precompute the squares at distances 1 and 2.
  static const int CLONE_DELTA[8][2];
  static const int JUMP_DELTA[16][2];
  static u64 cloneDomains[BOARD_SIZE * BOARD_SIZE];
  static u64 jumpDomains[BOARD_SIZE * BOARD_SIZE];

  u64 pieces[2];
  u64 empty;
  u64 boardCoefs[2]; // the sum of BOARD_COEFS for the red/blue pieces.
  bool side;

  static void precomputeDomains();

  void readFromStdin();
  void print();

  void makeMove(Move m);
  int eval();

  // Counts the pairs of adjacent pieces of a given player.
  int groupEval(bool side);

  // Returns the score of a position known to be final (no moves).
  int finalEval();

  int getNumPiecesToMove();
  int getNumEmpty();

  int estimateRemainingMoves();

  u64 getHashCode();

private:
  static void precomputeTypeDomains(const int (*delta)[2], int count, u64* dest);

  void printTopSeparatorLine();
  void printMiddleSeparatorLine();
  void printBottomSeparatorLine();
  void printSeparatorLine(const char* left, const char* center,
                          const char* right);
};
