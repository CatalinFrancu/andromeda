#include "AlphaBetaAgent.h"
#include "Board.h"
#include "Config.h"
#include "Log.h"
#include "Move.h"
#include "RandomAgent.h"

int main(int argc, char** argv) {
  Board::precomputeDomains();

  Board board;
  board.readFromStdin();
  board.print();

  int time[2];
  scanf("%d %d", &time[0], &time[1]);
  int my_time = time[board.side];
  int remaining_moves = board.estimateRemainingMoves();
  Log::debug("Time: %d milliseconds for an estimated %d moves.",
             my_time, remaining_moves);

  Move m;

  if (STRATEGY == STRAT_RANDOM) {
    RandomAgent agent(&board);
    m = agent.getMove();
  } else if (STRATEGY == STRAT_ALPHA_BETA) {
    AlphaBetaAgent agent(&board, my_time / remaining_moves);
    m = agent.getMove();
  } else {
    Log::fatal("Unknown value STRATEGY = %d.", STRATEGY);
  }

  printf("%s\n", m.toString().c_str());

  return 0;
}
