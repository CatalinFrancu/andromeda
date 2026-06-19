#include "Move.h"

#include "Util.h"

std::string Move::toString() {
  return (type == M_CLONE)
    ? Util::squareName(dest)
    : Util::squareName(src) + '-' + Util::squareName(dest);
}
