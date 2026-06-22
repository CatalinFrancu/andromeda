#include "TranspositionTable.h"

TranspositionRecord TranspositionTable::probe(u64 key) {
  auto it = map.find(key);
  if (it == map.end()) {
    return { .type = TT_UNKNOWN };
  } else {
    return it->second;
  }
}

void TranspositionTable::add(u64 key, int score, u8 depth, u8 type) {
  map[key] = { .score = score, .depth = depth, .type = type };
}
