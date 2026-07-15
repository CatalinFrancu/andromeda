#pragma once

/**
 * Use this file for values that are built into the game rules or the code
 * architecture and that the user should not mess with.
 **/

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned long long u64;

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

// Move generation constants.

// Iff this is true, we allow jumps to destinations that we can also reach via
// cloning. CAUTION: Disabling this seems to weaken the engine considerably.
const bool ALLOW_JUMPS_OVER_CLONES = true;

const int STRAT_RANDOM = 0;
const int STRAT_ALPHA_BETA = 1;
const int STRAT_MONTE_CARLO = 2;

// Zobrist hash constants.
const u64 ZOBRIST[2][BOARD_SIZE * BOARD_SIZE] = {
  {
    0x3dec'8c56'fb42'9ed9ull, 0x9ab5'b8c4'caf6'3635ull, 0xad3e'4027'86be'234cull,
    0x2904'2c78'fdbd'2708ull, 0x70f1'1fc1'9308'2493ull, 0x29a3'a6c7'23a8'9698ull,
    0xeaff'a811'06c1'2b9bull, 0x42f4'd576'e559'cd84ull, 0x250a'052f'd82a'ad3cull,
    0x5ed3'06f1'd855'ebb5ull, 0xc5f6'2c0a'9158'e238ull, 0xb88d'f497'5f37'b2ebull,
    0x9a03'dba8'9867'8f9eull, 0x0bd3'fbcf'829c'bc5cull, 0xeb33'648f'f6aa'ec91ull,
    0x0d69'06a9'533e'42c4ull, 0x118d'd703'e01e'3ff3ull, 0x6b96'dc9e'98cb'026bull,
    0x91bd'8a48'fb20'a26aull, 0x7df4'3044'a648'acd8ull, 0x4f1b'a105'eab6'a2deull,
    0x72a2'bcec'7d95'8db5ull, 0x980d'56b1'69c0'ab6bull, 0x9bf8'd7fd'045c'ee73ull,
    0x32c0'9a10'46ac'940dull, 0xf99e'9f4e'5903'f3ffull, 0x6d72'7c5b'52c9'3d2bull,
    0xac9d'5f10'e0bc'5dc2ull, 0xb6d9'533f'd7cf'1370ull, 0xd076'a9c5'1bfe'ff46ull,
    0x8c23'5163'2e48'f52cull, 0xfe36'd28c'55a0'fe01ull, 0x44c0'2852'2d87'a283ull,
    0x32af'2d91'5e09'1723ull, 0xdc3e'c966'fa02'175eull, 0xf0c0'03df'a17e'5b7bull,
    0x3534'6fbc'86c6'd1aaull, 0x56ec'ac54'2b27'8d29ull, 0x9a75'fc50'716f'd499ull,
    0x73e6'8f7a'908f'afffull, 0x6804'4d95'a16d'ccc4ull, 0x66fa'9687'fa9f'5390ull,
    0x3994'4054'6311'24bcull, 0x272e'62dd'bc3c'9e9full, 0xc0a5'7e01'09d5'8575ull,
    0x2abc'9a1c'633b'bbd1ull, 0xa6b9'e8a9'3688'e0bcull, 0x2df3'4d14'df34'4e69ull,
    0xcab9'98c0'aed0'14e4ull,
  }, {
    0x9b6e'11ea'9d14'7aa3ull, 0x9c75'b323'c2ab'7b67ull, 0xa294'8d2f'62ed'12afull,
    0x3204'1920'4b6b'9548ull, 0x3288'3a53'9343'6d0bull, 0x0e3e'1562'4c28'dc8bull,
    0x93df'4a5f'b1c1'e6ddull, 0x7320'da83'8903'1c89ull, 0x47b4'29e8'4045'ae42ull,
    0x2f45'c0c8'6d50'0645ull, 0x5174'27ef'772a'5d9aull, 0x8749'50e9'eb23'e32aull,
    0xf252'e0d4'be19'2400ull, 0xdfbb'f0aa'7527'd17cull, 0x5936'902e'23f8'a248ull,
    0xd4fc'8ef9'58ef'b279ull, 0x0f80'a308'647a'e26eull, 0x6ff9'd9a5'b895'5c8eull,
    0xc050'5410'307a'c041ull, 0xd766'66d4'8ebf'9c75ull, 0xf12f'92c1'3416'97dcull,
    0xda7e'5b7f'e00d'8132ull, 0xf40e'8483'7052'bb43ull, 0x966a'2e73'fa8f'aea4ull,
    0x8da8'a9c2'd3b6'0770ull, 0x1e74'8823'6e80'7165ull, 0x34a9'5a8f'49ad'f56cull,
    0xefe4'ec36'c99b'cce0ull, 0x27b3'4dab'e234'5098ull, 0xfd73'011c'2c90'e9b6ull,
    0x17f0'20b5'3301'5f73ull, 0xb44d'15f3'018a'd62full, 0x7222'c170'f680'08beull,
    0xf8d5'556f'a52a'2db9ull, 0xc23e'824c'a5f8'c9c0ull, 0x3147'f23a'78e3'2fd7ull,
    0x536b'4d8a'd11f'f6daull, 0xecc2'3c8f'9be9'7788ull, 0x1a5c'd990'64d1'cbc9ull,
    0x04e9'3f2c'6ac1'4302ull, 0x3574'7e55'ce90'87baull, 0x2d12'0baf'f177'a999ull,
    0xf62a'40b6'c17a'f57cull, 0x6348'9284'deff'8261ull, 0x0880'9d11'856d'c2e5ull,
    0x5a42'575a'2654'a84dull, 0xc668'021f'a2ea'dad9ull, 0x91b8'ee54'379a'0f7aull,
    0xc1aa'cede'b919'db2aull,
  }
};

const u64 ZOBRIST_SIDE = 0x3401'd30a'85ba'04b1ull;

// Transposition table constants.
const int TT_UNKNOWN = 0;
const int TT_EXACT = 1;
const int TT_LOWER_BOUND = 2;
const int TT_UPPER_BOUND = 3;

const int TT_BUCKET = 3;
const int TT_PREFETCH_MOVES = 4;

// Monte Carlo Tree Search constants.
const int MCTS_EXPLORATION_CONSTANT = 1.41;
const int MCTS_ROLLOUTS_PER_NODE = 1;
const int MCTS_WEIGHT_CLONE = 5;
const int MCTS_WEIGHT_JUMP = 1;

// Time measurement constants.
// Only poll the clock every this many calls to the function.
const int POLL_EVERY = 2048;

// Logging constants.
const int LOG_FATAL = 0;
const int LOG_ERROR = 1;
const int LOG_WARN = 2;
const int LOG_SUCCESS = 3;
const int LOG_NOTICE = 4;
const int LOG_INFO = 5;
const int LOG_DEBUG = 6;
