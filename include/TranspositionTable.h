#pragma once

#include "Board.h"
#include "TranspositionRecord.h"
#include <unordered_map>

class TranspositionTable {
public:

  TranspositionRecord probe(u64 key);
  void add(u64 key, int score, short move, u8 depth, u8 type);

private:
  std::unordered_map<u64, TranspositionRecord> map;

};
