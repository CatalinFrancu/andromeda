#pragma once

#include "Board.h"
#include "Move.h"

class MonteCarloNode {
public:
  int numSimulations;
  MonteCarloScore score;

  MonteCarloNode* parent;
  int numChildren;
  MonteCarloNode* children; // array of children, allocated dynamically

  Move move; // move that got us here from the parent

  MonteCarloNode(MonteCarloNode* parent);

  // Select a child based on the UCT formula.
  MonteCarloNode* uctSelectChild(bool side);

  double getWinProbability(bool side);
  MonteCarloNode* getChildWithBestRatio(bool side);
  MonteCarloNode* getHeaviestChild();

  // Returns true iff there are legal moves.
  bool expand(Board* board, Move* moves);
  MonteCarloNode* selectRandomChild();
};
