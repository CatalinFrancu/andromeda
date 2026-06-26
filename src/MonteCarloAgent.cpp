#include "MonteCarloAgent.h"

#include "Log.h"
#include "MoveGen.h"
#include "Time.h"

MonteCarloAgent::MonteCarloAgent(Board* board, int time) {
  rootBoard = *board;
  this->time = time;
  this->root = new MonteCarloNode(NULL);
}

Move MonteCarloAgent::getMove() {
  Time::startClock();
  while (Time::checkClock() < (unsigned)time) {
    select();
    expand();
    MonteCarloScore reward = simulate();
    backpropagate(reward);
  }

  int side = rootBoard.side;
  MonteCarloNode* child = root->getChildWithBestRatio(side);
  fprintf(stderr, "kibitz Ran %d simulations. Win probability %.3lf.\n",
          root->numSimulations,
          child->getWinProbability(side));

  MonteCarloNode* heavy = root->getHeaviestChild();
  if (heavy != child) {
    fprintf(stderr, "kibitz Chose a child with %d simulations; heaviest child has %d.\n",
            child->numSimulations,
            heavy->numSimulations);
  }

  return child->move;
}

void MonteCarloAgent::select() {
  node = root;
  board = rootBoard;

  while (node->numChildren) {
    node = node->uctSelectChild(board.side);
    board.makeMove(node->move);
  }
}

void MonteCarloAgent::expand() {
  if ((node->numSimulations) && node->expand(&board, moves)) {
    node = node->selectRandomChild();
    board.makeMove(node->move);
  }
}

MonteCarloScore MonteCarloAgent::simulate() {
  MonteCarloScore reward;

  for (int i = 0; i < MCTS_ROLLOUTS_PER_NODE; i++) {
    Board b = board;
    bool anyMoves = true;
    while (anyMoves) {
      MoveGen gen(&b, moves);
      gen.run();
      anyMoves = (gen.numMoves > 0);
      if (anyMoves) {
        Move move = gen.getRandomMove();
        b.makeMove(move);
      }
    }

    // Log::info("Simulation took %d turns", turn);
    reward += b.getMonteCarloScore();
  }

  return reward;
}

void MonteCarloAgent::backpropagate(MonteCarloScore reward) {
  while (node) {
    node->numSimulations += MCTS_ROLLOUTS_PER_NODE;
    node->score += reward;
    node = node->parent;
  }
}
