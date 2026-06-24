#include "TranspositionTable.h"

#include "Config.h"

TranspositionRecord TranspositionTable::probe(u64 key) {
  if (!USE_TRANSPOSITION_TABLES) {
    return { .type = TT_UNKNOWN };
  }
  auto it = map.find(key);
  if (it == map.end()) {
    return { .type = TT_UNKNOWN };
  } else {
    return it->second;
  }
}

void TranspositionTable::add(u64 key, int score, Move move, u8 depth, u8 type) {
  if (USE_TRANSPOSITION_TABLES) {
    map[key] = { .score = score, .move = move, .depth = depth, .type = type };
  }
}
