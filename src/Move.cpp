#include "Move.h"

#include "Util.h"

std::string Move::toString() {
  return (type == M_CLONE)
    ? Util::squareName(dest)
    : Util::squareName(src) + '-' + Util::squareName(dest);
}

bool operator==(const Move& a, const Move& b) {
  return (a.type == b.type) &&
    (a.src == b.src) &&
    (a.dest == b.dest);
}
