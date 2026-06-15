#include "Move.h"

#include <sstream>

std::string Move::toString() {
  std::stringstream ss;
  if (type == M_PASS) {
    ss << "[pass]";
  } else if (type == M_CLONE) {
    ss << "[clone:" << (int)dest << "]";
  } else {
    ss << "[jump:" << (int)src << "-" << (int)dest << "]";
  }

  return ss.str();
}
