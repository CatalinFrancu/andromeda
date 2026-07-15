#include "MonteCarloNode.h"

#include "Constants.h"
#include "Log.h"
#include "MoveGen.h"
#include "Util.h"
#include <math.h>

MonteCarloNode::MonteCarloNode(MonteCarloNode* parent) {
  this->parent = parent;
  this->numSimulations = 0;
  this->numChildren = 0;
}

bool MonteCarloNode::expand(Board* board, Move* moves) {
  MoveGen gen(board, moves);
  gen.genAll();

  if (!gen.numMoves) {
    return false;
  }

  int size = sizeof(MonteCarloNode);
  this->numChildren = gen.numMoves;
  this->children = (MonteCarloNode*)malloc(size * gen.numMoves);

  for (int i = 0; i < gen.numMoves; i++) {
    children[i] = MonteCarloNode(this);
    children[i].move = gen.moves[i];
  }

  return true;
}

MonteCarloNode* MonteCarloNode::selectRandomChild() {
  if (!numChildren) {
    Log::fatal("Called selectRandomChild() on unexpanded node!");
  }

  int index = Util::rand(0, numChildren - 1);
  return children + index;
}

MonteCarloNode* MonteCarloNode::uctSelectChild(bool side) {
  double logParent = log(numSimulations);

  double best = -INFINITY;
  int besti = 0;

  for (int i = 0; i < numChildren; i++) {
    MonteCarloNode* c = children + i; // syntactic sugar
    double uct;
    if (!c->numSimulations) {
      uct = INFINITY;
    } else {
      double exploitation = (double)c->score.s[side] / c->numSimulations;
      double exploration = sqrt(logParent / c->numSimulations);
      uct = exploitation + MCTS_EXPLORATION_CONSTANT * exploration;
    }

    if (uct > best) {
      best = uct;
      besti = i;
    }
  }

  return children + besti;
}

double MonteCarloNode::getWinProbability(bool side) {
  return numSimulations
    ? score.s[side] / numSimulations
    : -INFINITY;
}

MonteCarloNode* MonteCarloNode::getChildWithBestRatio(bool side) {
  double best = -INFINITY;
  int besti = 0;

  for (int i = 0; i < numChildren; i++) {
    MonteCarloNode* c = children + i; // syntactic sugar
    double r = c->getWinProbability(side);

    Log::debug("Child %d move %s has %d simulations, win chance %0.5lf",
               i, c->move.toString().c_str(), c->numSimulations, r);

    if (r > best) {
      best = r;
      besti = i;
    }
  }

  return &children[besti];
}

MonteCarloNode* MonteCarloNode::getHeaviestChild() {
  double best = -INFINITY;
  int besti = 0;

  for (int i = 0; i < numChildren; i++) {
    int sims = children[i].numSimulations;
    if (sims > best) {
      best = sims;
      besti = i;
    }
  }

  return children + besti;
}
