#include "Util.h"

#include "Constants.h"

std::random_device Util::rd;
std::mt19937 Util::rng(rd());

int Util::rand(int lo, int hi) {
  std::uniform_int_distribution<> distrib(lo, hi);
  return distrib(rng);
}

std::string Util::squareName(int sq) {
  std::string s;
  s = ('a' + sq % BOARD_SIZE);
  s += ('1' + sq / BOARD_SIZE);
  return s;
}
