#include "TranspositionTable.h"

TranspositionTable::TranspositionTable() {
  for (int i = 0; i < TRANSPOSITION_TABLE_SIZE; i++) {
    for (int j = 0; j < TT_BUCKET; j++) {
      table[i][j].type = TT_UNKNOWN;
    }
  }
  evictions = 0;
}

TranspositionRecord TranspositionTable::probe(u64 key) {
  int ind = getIndex(key);
  u16 fp = getFingerprint(key);

  for (int i = 0; i < TT_BUCKET; i++) {
    if (table[ind][i].fingerprint == fp) {
      return table[ind][i];
    }
  }

  return { .type = TT_UNKNOWN };
}

void TranspositionTable::add(u64 key, short score, short move, u8 depth, u8 type) {
  int ind = getIndex(key);
  u16 fp = getFingerprint(key);
  int min_i = -1;

  TranspositionRecord rec = {
    .fingerprint = fp,
    .score = score,
    .move = move,
    .depth = depth,
    .type = type,
  };

  // This could be a collision because the table only has limited space.
  // Commit the data if there is no previous data or the new data is deeper.
  for (int i = 0; i < TT_BUCKET; i++) {
    if (table[ind][i].type == TT_UNKNOWN) {
      table[ind][i] = rec;
      return;
    }

    if (table[ind][i].fingerprint == fp) {
      if (depth >= table[ind][i].depth) {
        table[ind][i] = rec;
      }
      return;
    }

    if ((min_i == -1) || (table[ind][i].depth < table[ind][min_i].depth)) {
      min_i = i;
    }
  }

  if (depth >= table[ind][min_i].depth) {
    evictions++;
    table[ind][min_i] = rec;
  }
}

void TranspositionTable::prefetch(u64 key) {
  int ind = getIndex(key);
  __builtin_prefetch(table[ind]);
}

int TranspositionTable::getIndex(u64 key) {
  return key & (TRANSPOSITION_TABLE_SIZE - 1);
}

int TranspositionTable::getFingerprint(u64 key) {
  return (key >> TRANSPOSITION_TABLE_BITS) & 0xffff;
}
