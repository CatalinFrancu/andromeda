#pragma once

/**
 * Use this file for values that are built into the game rules or the code
 * architecture and that the user should not mess with.
 **/

typedef unsigned char u8;
typedef unsigned long long u64;
typedef unsigned __int128 u128;

// Keep these in sync.
const int BOARD_SIZE = 7;
const u64 LEFT_COL =   0b0000001'0000001'0000001'0000001'0000001'0000001'0000001ll;
const u64 RIGHT_COL =  0b1000000'1000000'1000000'1000000'1000000'1000000'1000000ll;
const u64 TOP_ROW =    0b0000000'0000000'0000000'0000000'0000000'0000000'1111111ll;
const u64 BOTTOM_ROW = 0b1111111'0000000'0000000'0000000'0000000'0000000'0000000ll;

const int MAX_MOVES = 1'000; // TBD
const int MAX_ALPHA_BETA_DEPTH = 50; // Something that will never be exceeded

// Board evaluation constants.
const int POP_COEF = 100;   // Coefficient of the pure piece count.
const int GROUP_COEF = 3;   // Coefficient of pairs of neighboring pieces.
const int BOARD_COEFS[BOARD_SIZE * BOARD_SIZE] = {
  200, 50, 50, 50, 50, 50, 200,
  50, 10, 10, 10, 10, 10, 50,
  50, 10, 80, 50, 80, 10, 50,
  50, 10, 50, 80, 50, 10, 50,
  50, 10, 80, 50, 80, 10, 50,
  50, 10, 10, 10, 10, 10, 50,
  200, 50, 50, 50, 50, 50, 200,
};

// Base score for winning / losing a game. We add the piece difference on
// top of this.
const int WIN_SCORE = 32'000;

// More than winning 49-0.
const int INFTY = WIN_SCORE + BOARD_SIZE * BOARD_SIZE + 1;

const int M_CLONE = 1;
const int M_JUMP = 2;

// Move generation constants.

// Iff this is true, we allow jumps to destinations that we can also reach via
// cloning. CAUTION: Disabling this seems to weaken the engine considerably.
const bool ALLOW_JUMPS_OVER_CLONES = true;

const int STRAT_RANDOM = 0;
const int STRAT_ALPHA_BETA = 1;
const int STRAT_MONTE_CARLO = 2;

// Board hash code constants.
const u128 HASH1 = 0xce2d'8606'6491'af5cull;
const u128 HASH2 = 0xfb5e'bf76'a2a6'3120ull;
const u128 HASH3 = 0x71c8'203d'77eb'd4a4ull;

// Transposition table constants.
const int TT_UNKNOWN = 0;
const int TT_EXACT = 1;
const int TT_LOWER_BOUND = 2;
const int TT_UPPER_BOUND = 3;

// Monte Carlo Tree Search constants.
const int MCTS_EXPLORATION_CONSTANT = 1.41;
const int MCTS_ROLLOUTS_PER_NODE = 1;
// Weights of M_CLONE and M_JUMP.
const int MCTS_WEIGHTS[3] = { 0, 5, 1 };

// Logging constants.
const int LOG_FATAL = 0;
const int LOG_ERROR = 1;
const int LOG_WARN = 2;
const int LOG_SUCCESS = 3;
const int LOG_NOTICE = 4;
const int LOG_INFO = 5;
const int LOG_DEBUG = 6;
