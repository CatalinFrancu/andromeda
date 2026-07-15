#include "RandomAgent.h"

#include "Log.h"
#include "MoveGen.h"
#include "Util.h"

RandomAgent::RandomAgent(Board* board) {
  this->board = board;
}

Move RandomAgent::getMove() {
  MoveGen moveGen(board, moves);
  moveGen.genAll();
  int i = Util::rand(0, moveGen.numMoves - 1);
  return moveGen.moves[i];
}
