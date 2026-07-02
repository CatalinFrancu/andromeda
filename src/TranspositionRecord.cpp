#include "TranspositionRecord.h"

bool TranspositionRecord::isCutoff(int depth, int alpha, int beta) {
  return
    (this->depth >= depth) &&
    ((this->type == TT_EXACT) ||
     ((this->type == TT_LOWER_BOUND) && (this->score >= beta)) ||
     ((this->type == TT_UPPER_BOUND) && (this->score < alpha)));
}
