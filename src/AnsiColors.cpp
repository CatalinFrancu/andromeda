#include "AnsiColors.h"

const char AnsiColors::FATAL[10] = "\e[91m";
const char AnsiColors::ERROR[10] = "\e[91m";
const char AnsiColors::WARN[10] = "\e[93m";
const char AnsiColors::SUCCESS[10] = "\e[92m";
const char AnsiColors::INFO[10] = "\e[39m";
const char AnsiColors::DEBUG[10] = "\e[37m";

const char AnsiColors::DEFAULT[10] = "\e[00m";

const char AnsiColors::PIECE[2][10] = {
  "\e[91m", "\e[94m",
};
