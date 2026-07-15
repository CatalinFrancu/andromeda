#include "Move.h"

#include "Util.h"

bool Move::isClone() {
  return (src == dest);
}

bool Move::isJump() {
  return (src != dest);
}

int Move::getMonteCarloWeight() {
  return isClone() ? MCTS_WEIGHT_CLONE : MCTS_WEIGHT_JUMP;
}

std::string Move::toString() {
  return isClone()
    ? Util::squareName(dest)
    : Util::squareName(src) + '-' + Util::squareName(dest);
}

bool operator==(const Move& a, const Move& b) {
  return (a.src == b.src) && (a.dest == b.dest);
}
