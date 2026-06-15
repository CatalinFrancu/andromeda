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

const int MAX_MOVES = 200; // TBD

const int M_PASS = 0;
const int M_CLONE = 1;
const int M_JUMP = 2;

const int STRAT_RANDOM = 0;

const int LOG_FATAL = 0;
const int LOG_ERROR = 1;
const int LOG_WARN = 2;
const int LOG_SUCCESS = 3;
const int LOG_INFO = 4;
const int LOG_DEBUG = 5;
