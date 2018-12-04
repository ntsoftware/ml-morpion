#pragma once

#include <cassert>
#include <iostream>
#include <vector>

class Board {

private:
  const size_t w;
  const size_t h;
  const size_t n;
  std::vector<char> d;

public:
  Board(const size_t w, const size_t h, const size_t n) : w(w), h(h), n(n), d(w * h, '.') {}

  bool can_play(const size_t i, const size_t j) const {
    if (i >= w || j >= h) {
      return false;
    } else {
      return d[i * w + j] == '.';
    }
  }

  void play(const size_t i, const size_t j, const char c) {
    assert(can_play(i, j));
    d[i * w + j] = c;
  }

  bool is_winner_at(const char c, const size_t i, const size_t j, const int u, const int v) const {
    assert(0 <= i && i < h);
    assert(0 <= j && j < w);
    assert(0 <= i + u*(n - 1) && i + u*(n - 1) < h);
    assert(0 <= j + v*(n - 1) && j + v*(n - 1) < w);
    size_t k = 0;
    while (k < n && d[(i + u*k) * w + (j + v*k)] == c) {
      ++k;
    }
    return k == n;
  }

  bool is_winner(const char c) const {
    // check horizontal '-'
    for (size_t i = 0; i < h; ++i) {
      for (size_t j = 0; j <= w - n; ++j) {
        if (is_winner_at(c, i, j, 0, 1)) {
          return true;
        }
      }
    }
    // check vertical '|'
    for (size_t i = 0; i <= h - n; ++i) {
      for (size_t j = 0; j < w; ++j) {
        if (is_winner_at(c, i, j, 1, 0)) {
          return true;
        }
      }
    }
    // check diagonal '\'
    for (size_t i = 0; i <= h - n; ++i) {
      for (size_t j = 0; j <= w - n; ++j) {
        if (is_winner_at(c, i, j, 1, 1)) {
          return true;
        }
      }
    }
    // check diagonal '/'
    for (size_t i = 0; i <= h - n; ++i) {
      for (size_t j = n - 1; j < w; ++j) {
        if (is_winner_at(c, i, j, 1, -1)) {
          return true;
        }
      }
    }
    return false;
  }

  bool is_draw() const {
    for (size_t i = 0; i < h; ++i) {
      for (size_t j = 0; j < w; ++j) {
        if (can_play(i, j)) {
          return false;
        }
      }
    }
    return true;
  }

  std::vector<int> to_input_vector() const {
    std::vector<int> v;
    v.reserve(3 * w * h);
    static const char c[] = { '.', 'X', 'O' };
    for (size_t k = 0; k < 3; ++k) {
      for (size_t i = 0; i < h; ++i) {
        for (size_t j = 0; j < w; ++j) {
          v.push_back(d[i * w + j] == c[k] ? 1 : 0);
        }
      }
    }
    return v;
  }

  friend std::ostream& operator<< (std::ostream&, const Board&);
};

std::ostream& operator<< (std::ostream& os, const Board& b) {
  // header
  os << " ";
  for (size_t j = 0; j < b.w; ++j) {
    os << " " << j;
  }
  os << std::endl;
  // board
  for (size_t i = 0; i < b.h; ++i) {
    os << i;
    for (size_t j = 0; j < b.w; ++j) {
      os << " " << b.d[i * b.w + j];
    }
    os << std::endl;
  }
  return os;
}
