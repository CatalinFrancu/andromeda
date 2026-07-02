#include "Util.h"

#include "Constants.h"

std::random_device Util::rd;
std::mt19937 Util::rng(rd());
char Util::msb[1 << 16];

int Util::rand(int lo, int hi) {
  std::uniform_int_distribution<> distrib(lo, hi);
  return distrib(rng);
}

void Util::precomputeMsb() {
  for (int i = 2; i <= 0xffff ; i++) {
    msb[i] = msb[i / 2] + 1;
  }
}

int Util::clz(u64 x) {
  return __builtin_clzll(x);
  int res = 0;
  if (x <= 0xffff'ffffull) {
    res += 32;
  } else {
    x >>= 32;
  }
  if (x <= 0xffffull) {
    res += 16;
  } else {
    x >>= 16;
  }
  return res + 15 - msb[x];
  // x |= x >> 1;
  // x |= x >> 2;
  // x |= x >> 4;
  // x |= x >> 8;
  // x |= x >> 16;
  // x |= x >> 32;
  // return 64 - __builtin_popcountll(x);
}

std::string Util::squareName(int sq) {
  std::string s;
  s = ('a' + sq % BOARD_SIZE);
  s += ('1' + sq / BOARD_SIZE);
  return s;
}
