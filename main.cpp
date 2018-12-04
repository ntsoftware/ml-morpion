#include <iostream>

#include "board.h"

int main() {
  Board b(5, 5, 3);

  char c = 'X';
  while (!b.is_draw() && !b.is_winner('X') && !b.is_winner('O')) {
    std::cout << b;
    std::cout << "[" << c << "](row,col)> ";
    int i, j;
    std::cin >> i >> j;
    if (b.can_play(i, j)) {
      b.play(i, j, c);
      if (c == 'X') {
        c = 'O';
      } else {
        c = 'X';
      }
    } else {
      std::cout << "ERROR: cannot play (" << i << "," << j << "), try again" << std::endl;
    }
    std::cout << std::endl;
  }

  if (b.is_draw()) {
    std::cout << "DRAW GAME!" << std::endl;
  } else if (b.is_winner('X')) {
    std::cout << "PLAYER X WINS!" << std::endl;
  } else if (b.is_winner('O')) {
    std::cout << "PLAYER O WINS!" << std::endl;
  }
  return 0;
}
