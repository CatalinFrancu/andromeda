#pragma once

class MonteCarloScore {
public:
  double s[2];

  MonteCarloScore();
  MonteCarloScore(double s0, double s1);
  MonteCarloScore& operator+=(const MonteCarloScore& other);
};
