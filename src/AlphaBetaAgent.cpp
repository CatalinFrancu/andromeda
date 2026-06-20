#include "AlphaBetaAgent.h"

#include "Log.h"
#include "MoveGen.h"
#include "Time.h"
#include "Util.h"

AlphaBetaAgent::AlphaBetaAgent(Board* board, int time) {
  this->board = board;
  this->time = time;
  Log::debug("Thinking for %d milliseconds", time);
}

Move AlphaBetaAgent::getMove() {
  Move move = iterativeDeepening();
  return move;
}

Move AlphaBetaAgent::iterativeDeepening() {
  Move move;
  int score;
  int depth = 0;

  // Randomize the moves at the first level so that the agent picks a random
  // choice from among equals.
  MoveGen moveGen(board, moves[0]);
  moveGen.run();
  moveGen.randomizeMoves();
  moveCount++;

  do {
    prevMillis = millis;
    prevPosCount = posCount;
    posCount = moveCount = 0;
    Time::startClock();
    alphaBetaWrapper(++depth, moveGen.numMoves, move, score);
    millis = Time::stopClock();
    time -= millis;
    logStats(depth, score, millis);
  } while ((score > -WIN_SCORE) && (score < WIN_SCORE) && haveTime());

  return move;
}

bool AlphaBetaAgent::haveTime() {
  if (!prevMillis || !millis) {
    return true;
  }

  // Extrapolate the duration of the next iteration. Be conservative as it may
  // exceed our expectations.
  u64 nextMillis = (u64)millis * millis / prevMillis;
  Log::debug("%d millis remaining; previous 2 iterations took %d and %d millis",
             time, prevMillis, millis);
  Log::debug("Estimated that the next iteration would take %llu millis.",
             nextMillis);
  return (int)nextMillis <= time * 9 / 10;
}

void AlphaBetaAgent::alphaBetaWrapper(int depth, int numMoves, Move& move, int& score) {
  score = -INFTY;

  for (int i = 0; i < numMoves; i++) {
    Move m = moves[0][i];
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
}

int AlphaBetaAgent::alphaBeta(Board b, int depth, int alpha, int beta) {
  if (depth <= 0) {
    posCount++;
    return b.eval();
  }

  MoveGen moveGen(&b, moves[depth]);
  moveGen.run();
  moveCount++;

  if (!moveGen.numMoves) {
    return b.finalEval();
  }

  for (int i = 0; i < moveGen.numMoves; i++) {
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

void AlphaBetaAgent::logStats(int depth, int score, int millis) {
  char s[100];

  if (score >= WIN_SCORE) {
    sprintf(s, "win(%d-%d)",
            score - WIN_SCORE,
            BOARD_SIZE * BOARD_SIZE - (score - WIN_SCORE));
  } else if (score <= -WIN_SCORE) {
    sprintf(s, "lose(%d-%d)",
            BOARD_SIZE * BOARD_SIZE + score + WIN_SCORE,
            -score - WIN_SCORE);
  } else {
    sprintf(s, "%d", score);
  }

  Log::info("depth %d:    %d millis    score %s    %llu positions    %llu calls to movegen",
            depth, millis, s, posCount, moveCount);
}
