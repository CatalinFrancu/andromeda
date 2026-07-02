#pragma once

#include "Constants.h"
#include <random>

class Util {
public:
  static std::mt19937 rng;

  static int rand(int lo, int hi);
  static void precomputeMsb();
  static int clz(u64 x);
  static std::string squareName(int sq);

private:
  static std::random_device rd;
  static char msb[1 << 16];
};
