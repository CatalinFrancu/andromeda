#include "MonteCarloScore.h"

MonteCarloScore::MonteCarloScore() {
  s[0] = s[1] = 0.0;
}

MonteCarloScore::MonteCarloScore(double s0, double s1) {
  s[0] = s0;
  s[1] = s1;
}

MonteCarloScore& MonteCarloScore::operator+=(const MonteCarloScore& other) {
  s[0] += other.s[0];
  s[1] += other.s[1];
  return *this;
}
