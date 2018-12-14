#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "boardwrapper.h"

struct board* create(const int n, const int w, const int h) {
  struct board* b = malloc(sizeof(struct board) + w * h);
  if (b) {
    b->n = n;
    b->w = w;
    b->h = h;
    char* d = (void*)b + sizeof(struct board);
    memset(d, '.', w * h);
  }
  return b;
}

void destroy(struct board* b) {
  free(b);
}

static char get(const struct board* b, const int i, const int j) {
  assert(0 <= i && i < b->h);
  assert(0 <= j && j < b->w);
  const char* d = (const void*)b + sizeof(struct board);
  return d[i * b->w + j];
}

static void set(struct board* b, const int i, const int j, const char c) {
  assert(0 <= i && i < b->h);
  assert(0 <= j && j < b->w);
  char* d = (void*)b + sizeof(struct board);
  d[i * b->w + j] = c;
}

bool can_play(const struct board* b, const int i, const int j) {
  return get(b, i, j) == '.';
}

void play(struct board* b, const int i, const int j, const char c) {
  assert(c == 'X' || c == 'O');
  set(b, i, j, c);
}

static bool is_winner_at(const struct board* b, const char c, const int i, const int j, const int u, const int v) {
  int k = 0;
  while (k < b->n && get(b, i + u*k, j + v*k) == c) {
    ++k;
  }
  return k == b->n;
}

bool is_winner(const struct board* b, const char c) {
  // check horizontal '-'
  for (int i = 0; i < b->h; ++i) {
    for (int j = 0; j <= b->w - b->n; ++j) {
      if (is_winner_at(b, c, i, j, 0, 1)) {
        return true;
      }
    }
  }
  // check vertical '|'
  for (int i = 0; i <= b->h - b->n; ++i) {
    for (int j = 0; j < b->w; ++j) {
      if (is_winner_at(b, c, i, j, 1, 0)) {
        return true;
      }
    }
  }
  // check diagonal '\'
  for (int i = 0; i <= b->h - b->n; ++i) {
    for (int j = 0; j <= b->w - b->n; ++j) {
      if (is_winner_at(b, c, i, j, 1, 1)) {
        return true;
      }
    }
  }
  // check diagonal '/'
  for (int i = 0; i <= b->h - b->n; ++i) {
    for (int j = b->n - 1; j < b->w; ++j) {
      if (is_winner_at(b, c, i, j, 1, -1)) {
        return true;
      }
    }
  }
  return false;
}

bool is_draw(const struct board* b) {
  for (int i = 0; i < b->h; ++i) {
    for (int j = 0; j < b->w; ++j) {
      if (can_play(b, i, j)) {
        return false;
      }
    }
  }
  return true;
}

int get_input_vector_length(const struct board* b) {
  return 3 * b->w * b->h;
}

int* to_input_vector(const struct board* b) {
  int* v = malloc(get_input_vector_length(b) * sizeof(int));
  if (v) {
    static const char c[] = { '.', 'X', 'O' };
    int* ptr = v;
    for (int k = 0; k < 3; ++k) {
      for (int i = 0; i < b->h; ++i) {
        for (int j = 0; j < b->w; ++j) {
          *ptr++ = get(b, i , j) == c[k] ? 1 : 0;
        }
      }
    }
  }
  return v;
}

void destroy_input_vector(int* v) {
  free(v);
}