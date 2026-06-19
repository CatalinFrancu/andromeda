#include "AlphaBetaAgent.h"

#include "Log.h"
#include "MoveGen.h"
#include "Util.h"

AlphaBetaAgent::AlphaBetaAgent(Board* board) {
  this->board = board;
}

Move AlphaBetaAgent::getMove() {
  int depth = 7;
  Move move;
  int score;
  alphaBetaWrapper(depth, move, score);
  return move;
}

void AlphaBetaAgent::alphaBetaWrapper(int depth, Move& move, int& score) {
  score = -INFTY;
  posCount = moveCount = 0;

  // Randomize the moves at the first level so that the agent picks a random
  // choice from among equals.
  MoveGen moveGen(board, moves[depth]);
  moveGen.run();
  moveGen.randomizeMoves();

  for (int i = 0; i < moveGen.numMoves; i++) {
    moveCount++;
    Move m = moves[depth][i];
    Board new_b = *board;
    new_b.makeMove(m);
    int child = -alphaBeta(new_b, depth - 1, -INFTY, -score);
    Log::debug("Move %s has score %d at depth %d",
               m.toString().c_str(), child, depth - 1);
    if (child > score) {
      score = child;
      move = m;
    }
  }

  logStats(depth, score);
}

int AlphaBetaAgent::alphaBeta(Board b, int depth, int alpha, int beta) {
  if (depth <= 0) {
    posCount++;
    return b.eval();
  }

  MoveGen moveGen(&b, moves[depth]);
  moveGen.run();

  if (!moveGen.numMoves) {
    return b.finalEval();
  }

  for (int i = 0; i < moveGen.numMoves; i++) {
    moveCount++;
    Move m = moves[depth][i];
    Board new_b = b;
    new_b.makeMove(m);
    int child = -alphaBeta(new_b, depth - 1, -beta, -alpha);

    if (child >= beta) {
      return beta;
    } else if (child > alpha) {
      alpha = child;
    }
  }

  return alpha;
}

void AlphaBetaAgent::logStats(int depth, int score) {
  if (score >= WIN_SCORE) {
    Log::info("depth %d    score win(%d-%d)    %llu positions    %llu moves",
              depth, score - WIN_SCORE, BOARD_SIZE * BOARD_SIZE - (score - WIN_SCORE),
              posCount, moveCount);
  } else if (score <= -WIN_SCORE) {
    Log::info("depth %d    score lose(%d-%d)    %llu positions    %llu moves",
              depth, BOARD_SIZE * BOARD_SIZE + score + WIN_SCORE, -score - WIN_SCORE,
              posCount, moveCount);
  } else {
    Log::info("depth %d    score %d    %llu positions    %llu moves",
              depth, score, posCount, moveCount);
  }
}
