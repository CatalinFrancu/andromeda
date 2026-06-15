#include "Board.h"

int main(int argc, char** argv) {
  Board board;
  board.readFromStdin();
  board.print();

  return 0;
}
