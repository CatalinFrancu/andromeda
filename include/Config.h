#pragma once

/**
 * Use this file for values that the user may wish to control.
 **/

#include "Constants.h"

const int LOG_LEVEL = LOG_DEBUG;

// Fraction of moves that are clones. We use this to estimate the number of
// remaining moves, which dictates time management.
const double FRACTION_CLONES = 0.65;

const int STRATEGY = STRAT_ALPHA_BETA;
