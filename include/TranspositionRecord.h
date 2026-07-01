#pragma once

#include "Constants.h"
#include "Move.h"

class TranspositionRecord {
public:
  // Store a partial key only. With 16 bits, there is 1/2^{16} chance of a
  // collision, in which case we will return an incorrect record. This is
  // acceptable in the grand scheme of things.
  u16 fingerprint;
  short score;
  short move; // index in move list (assumes the move generator is consistent)
  u8 depth;
  u8 type;
};
