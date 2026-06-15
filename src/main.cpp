#include "Board.h"
#include "Config.h"
#include "Log.h"
#include "Move.h"
#include "RandomAgent.h"

int main(int argc, char** argv) {
  Board::precomputeJumpDomains();

  Board board;
  board.readFromStdin();
  board.print();

  Move m;

  if (STRATEGY == STRAT_RANDOM) {
    RandomAgent agent(&board);
    m = agent.getMove();
  } else {
    Log::fatal("Unknown value STRATEGY = %d.", STRATEGY);
  }

  switch(m.type) {
    case M_PASS: printf("0\n"); break;
    case M_CLONE: printf("1 %d\n", m.dest); break;
    case M_JUMP: printf("2 %d %d\n", m.src, m.dest); break;
  }

  return 0;
}
