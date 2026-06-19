#pragma once

/**
 * Use this file for values that are built into the game rules or the code
 * architecture and that the user should not mess with.
 **/

typedef unsigned char u8;
typedef unsigned long long u64;

// Keep these in sync.
const int BOARD_SIZE = 7;
const u64 LEFT_COL =   0b0000001'0000001'0000001'0000001'0000001'0000001'0000001ll;
const u64 RIGHT_COL =  0b1000000'1000000'1000000'1000000'1000000'1000000'1000000ll;
const u64 TOP_ROW =    0b0000000'0000000'0000000'0000000'0000000'0000000'1111111ll;
const u64 BOTTOM_ROW = 0b1111111'0000000'0000000'0000000'0000000'0000000'0000000ll;

const int MAX_MOVES = 1'000; // TBD
const int MAX_ALPHA_BETA_DEPTH = 50; // Something that will never be exceeded

// Base score for winning / losing a game. We add the piece difference on
// top of this.
const int WIN_SCORE = 1'000'000;

// More than winning 49-0.
const int INFTY = WIN_SCORE + BOARD_SIZE * BOARD_SIZE + 1;

const int M_PASS = 0;
const int M_CLONE = 1;
const int M_JUMP = 2;

const int STRAT_RANDOM = 0;
const int STRAT_ALPHA_BETA = 1;

const int LOG_FATAL = 0;
const int LOG_ERROR = 1;
const int LOG_WARN = 2;
const int LOG_SUCCESS = 3;
const int LOG_INFO = 4;
const int LOG_DEBUG = 5;
