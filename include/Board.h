#pragma once

#include "Constants.h"

class Board {
public:
  u64 piece[2];
  u64 empty;
  bool side;

  void readFromStdin();
  void print();

private:
  void printSeparatorLine();
};
