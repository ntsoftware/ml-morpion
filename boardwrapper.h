#pragma once

#include <stdbool.h>

struct board {
  // number of crosses/circles to win
  int n;
  // board width
  int w;
  // board height
  int h;
  // board data follows right after this struct
  // [...]
};

extern struct board* create(const int n, const int w, const int h);
extern void destroy(struct board* b);
extern bool can_play(const struct board* b, const int i, const int j);
extern void play(struct board* b, const int i, const int j, const char c);
extern bool is_winner(const struct board* b, const char c);
extern bool is_draw(const struct board* b);
extern int get_input_vector_length(const struct board* b);
extern int* to_input_vector(const struct board* b);
extern void destroy_input_vector(int* v);