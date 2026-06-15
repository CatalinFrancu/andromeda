#include "Util.h"

std::random_device Util::rd;
std::mt19937 Util::rng(rd());

int Util::rand(int lo, int hi) {
  std::uniform_int_distribution<> distrib(lo, hi);
  return distrib(rng);
}
