#pragma once

#include "Board.h"
#include "Config.h"
#include "TranspositionRecord.h"

class TranspositionTable {
public:

  TranspositionTable();
  TranspositionRecord probe(u64 key);
  void add(u64 key, short score, short move, u8 depth, u8 type);
  void prefetch(u64 key);
  int evictions;

private:
  TranspositionRecord table[TRANSPOSITION_TABLE_SIZE][TT_BUCKET];

  int getIndex(u64 key);
  int getFingerprint(u64 key);

};
