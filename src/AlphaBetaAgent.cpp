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

  // Stop either when the current iteration exceeds the time or when we
  // estimate that the next iteration would be too slow.
  do {
    prevMillis = millis;
    prevPosCount = posCount;
    posCount = moveCount = tt_cutoffs = 0;
    Time::startClock();
    bool inTime = alphaBetaWrapper(++depth, moveGen.numMoves, move, score);
    millis = Time::checkClock();
    time -= millis;
    if (inTime) {
      putMoveFirst(move, 0);
      logStats(depth, score, millis);
    }
  } while ((score > -WIN_SCORE) && (score < WIN_SCORE) && haveTime());

  return moves[0][0];
}

bool AlphaBetaAgent::haveTime() {
  if (!prevMillis || !millis) {
    return true;
  }

  if (time <= 0) {
    return false;
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

bool AlphaBetaAgent::alphaBetaWrapper(int depth, int numMoves, Move& move, int& score) {
  score = -INFTY;

  int i = 0;
  while ((i < numMoves) && (Time::checkClock() < (unsigned)time)) {
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
    i++;
  }

  if (i < numMoves) {
    Log::notice("alpha-beta ran out of time mid-iteration");
    return false;
  } else {
    return true;
  }
}

int AlphaBetaAgent::alphaBeta(Board b, int depth, int alpha, int beta) {
  if (depth <= 0) {
    posCount++;
    return b.eval();
  }

  u64 hash = b.getHashCode();
  TranspositionRecord rec = tt.probe(hash);
  if ((rec.depth >= depth) &&
      ((rec.type == TT_EXACT) ||
       ((rec.type == TT_LOWER_BOUND) && (rec.score >= beta)) ||
       ((rec.type == TT_UPPER_BOUND) && (rec.score < alpha)))) {
    tt_cutoffs++;
    return rec.score;
  }

  MoveGen moveGen(&b, moves[depth]);
  moveGen.run();
  moveCount++;

  if (!moveGen.numMoves) {
    return b.finalEval();
  }

  if ((rec.type == TT_EXACT) || (rec.type == TT_LOWER_BOUND)) {
    putMoveFirst(rec.move, depth);
  }

  u8 type = TT_UPPER_BOUND;
  Move bestMove;
  for (int i = 0; i < moveGen.numMoves; i++) {
    Move m = moves[depth][i];
    Board new_b = b;
    new_b.makeMove(m);
    int child = -alphaBeta(new_b, depth - 1, -beta, -alpha);

    if (child >= beta) {
      tt.add(hash, child, m, depth, TT_LOWER_BOUND);
      return beta;
    } else if (child > alpha) {
      type = TT_EXACT;
      bestMove = m;
      alpha = child;
    }
  }

  tt.add(hash, alpha, bestMove, depth, type);
  return alpha;
}

void AlphaBetaAgent::putMoveFirst(Move move, int depth) {
  int i = 0;
  while (moves[depth][i] != move) {
    i++;
  }

  Move tmp = moves[depth][0];
  moves[depth][0] = moves[depth][i];
  moves[depth][i] = tmp;
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

  Log::info("depth %d:    %d millis    score %s    %llu positions    %llu calls to movegen    %llu tt cutoffs",
            depth, millis, s, posCount, moveCount, tt_cutoffs);
  fprintf(stderr, "kibitz depth %d / score %s / %llu positions / %llu calls to movegen / %llu tt cutoffs\n",
            depth, s, posCount, moveCount, tt_cutoffs);
}
