#include "TranspositionTable.h"

#include "Config.h"

TranspositionTable::TranspositionTable() {
  table = new TranspositionRecord[TRANSPOSITION_TABLE_SIZE];
  for (int i = 0; i < TRANSPOSITION_TABLE_SIZE; i++) {
    table[i].fingerprint = 0;
    table[i].type = TT_UNKNOWN;
  }
  evictions = 0;
}

TranspositionRecord TranspositionTable::probe(u64 key) {
  if (!USE_TRANSPOSITION_TABLES) {
    return { .type = TT_UNKNOWN };
  }

  int i = getIndex(key);
  u16 fp = getFingerprint(key);

  if (table[i].fingerprint == fp) {
    return table[i];
  } else {
    return { .type = TT_UNKNOWN };
  }
}

void TranspositionTable::add(u64 key, short score, short move, u8 depth, u8 type) {
  if (!USE_TRANSPOSITION_TABLES) {
    return;
  }

  int i = getIndex(key);
  u16 fp = getFingerprint(key);

  // This could be a collision because the table only has limited space.
  // Commit the data if there is no previous data or the new data is deeper.
  if ((table[i].type == TT_UNKNOWN) || (depth >= table[i].depth)) {
    evictions += (table[i].type != TT_UNKNOWN);
    table[i] = {
      .fingerprint = fp,
      .score = score,
      .move = move,
      .depth = depth,
      .type = type,
    };
  }
}

int TranspositionTable::getIndex(u64 key) {
  return key & (TRANSPOSITION_TABLE_SIZE - 1);
}

int TranspositionTable::getFingerprint(u64 key) {
  return (key >> TRANSPOSITION_TABLE_BITS) & 0xffff;
}
