#include <inttypes.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// number of rows
static const int M = 5;
// number of columns
static const int N = 5;
// number of x/o to align to win
static const int K = 4;
// depth of search
static const int D = 6;

static inline int min(int a, int b) {
    return a < b ? a : b;
}

static inline int max(int a, int b) {
    return a > b ? a : b;
}

struct state {
    uint8_t x[M];
    uint8_t o[M];
};

// look-up table to get maximum number of consecutive bits set in a row
static int table[1 << N];

static void fill_table() {
    const int n = 1 << N;
    for (int i=0; i<n; ++i) {
        uint8_t r = (uint8_t)i;
        int m = 0;
        int n = 0;
        for (int j=0; j<N; ++j) {
            n = (r & 1) == 1 ? n+1 : 0;
            m = max(m, n);
            r >>= 1;
        }
        table[i] = m;
    }
}

static void init_state(struct state* s) {
    for (int i=0; i<M; ++i) {
        s->x[i] = 0;
        s->o[i] = 0;
    }
}

static void print_state(const struct state* s) {
    printf(" ");
    for (int i=0; i<N; ++i) {
        printf(" %d", i);
    }
    printf("\n");
    for (int i=0; i<M; ++i) {
        printf("%d", i);
        for (int j=0; j<N; ++j) {
            const uint8_t r = 1 << j;
            if ((s->x[i] & r) != 0)
                printf(" x");
            else if ((s->o[i] & r) != 0)
                printf(" o");
            else
                printf(" .");
        }
        printf("\n");
    }
}

static bool is_draw(const struct state* s) {
    const uint8_t r = (1 << N) - 1;
    for (int i=0; i<M; ++i) {
        if ((s->x[i] | s->o[i]) != r)
            return false;
    }
    return true;
}

static bool is_winner(const uint8_t* rows) {
    for (int i=0; i<M; ++i) {
        if (table[rows[i]] >= K)
            return true;
    }
    for (int i=0; i<=M-K; ++i) {
        uint8_t r0, r1, r2;
        r0 = r1 = r2 = rows[i];
        for (int j=1; j<K; ++j) {
            const uint8_t r = rows[i+j];
            r0 &= r;
            r1 &= r << j;
            r2 &= r >> j;
        }
        if (r0 != 0 || r1 != 0 || r2 != 0)
            return true;
    }
    return false;
}

static bool can_play(const struct state* s, const int i, const int j) {
    const uint8_t r = 1 << j;
    return (s->x[i] & r) == 0 && (s->o[i] & r) == 0;
}

static void play(uint8_t* rows, const int i, const int j) {
    rows[i] |= 1 << j;
}

static void back(uint8_t* rows, const int i, const int j) {
    rows[i] &= ~(1 << j);
}

struct context {
    clock_t start;
    clock_t end;
    size_t n;
    struct state s;
    int v[M][N];
    int v_max;
};

void init_context(struct context* c, const struct state* s) {
    c->start = 0;
    c->end = 0;
    c->n = 0;
    c->s = *s;
    for (int i=0; i<M; ++i) {
        for (int j=0; j<N; ++j) {
            c->v[i][j] = INT_MIN;
        }
    }
    c->v_max = INT_MIN;
}

void print_context(const struct context* c) {
    printf("%lu nodes (%.3gs, max=%+d)\n", c->n, 1.0 * (c->end - c->start) / CLOCKS_PER_SEC, c->v_max);
    printf(" ");
    for (int i=0; i<N; ++i) {
        printf("  %d", i);
    }
    printf("\n");
    for (int i=0; i<M; ++i) {
        printf("%d", i);
        for (int j=0; j<N; ++j) {
            const int v = c->v[i][j];
            if (v == INT_MIN)
                printf(" ..");
            else
                printf(" %+d", v);
        }
        printf("\n");
    }
}

static int minimax(struct context* c, int d, int a, int b, bool x) {
    ++c->n;
    if (is_winner(c->s.x))
        return -1;
    if (is_winner(c->s.o))
        return 1 + D - d;
    if (d == D)
        return 0;
    int u = x ? INT_MAX : INT_MIN;
    if (x) {
        for (int i=0; i<M; ++i) {
            for (int j=0; j<N; ++j) {
                if (can_play(&c->s, i, j)) {
                    play(c->s.x, i, j);
                    const int v = minimax(c, d+1, a, b, false);
                    back(c->s.x, i, j);
                    u = min(u, v);
                    b = min(b, v);
                    if (a >= b)
                        return u;
                }            
            }
        }
    } else {
        for (int i=0; i<M; ++i) {
            for (int j=0; j<N; ++j) {
                if (can_play(&c->s, i, j)) {
                    play(c->s.o, i, j);
                    const int v = minimax(c, d+1, a, b, false);
                    back(c->s.o, i, j);
                    u = max(u, v);
                    a = max(a, v);
                    if (a >= b)
                        return u;
                }            
            }
        }
    }
    return u;
}

static void ai(struct context* c) {
    c->start = clock();
    for (int i=0; i<M; ++i) {
        for (int j=0; j<N; ++j) {
            if (can_play(&c->s, i, j)) {
                play(c->s.o, i, j);
                const int v = minimax(c, 1, INT_MIN, INT_MAX, true);
                back(c->s.o, i, j);
                c->v[i][j] = v;
                c->v_max = max(c->v_max, v);
            }
        }
    }
    c->end = clock();
}

static void random_pick(const struct context* c, int* out_i, int* out_j) {
    bool first = true;
    for (int i=0; i<M; ++i) {
        for (int j=0; j<N; ++j) {
            if (c->v[i][j] == c->v_max && (first || (rand() & 1) == 1)) {
                *out_i = i;
                *out_j = j;
                first = false;
            }
        }
    }
}

int main() {
    srand((unsigned)clock());
    fill_table();

    struct state s;
    init_state(&s);

    char c = 'x';
    while (true) {
        print_state(&s);

        if (is_winner(s.x)) {
            printf("x WINS!\n");
            break;
        }
        if (is_winner(s.o)) {
            printf("o WINS!\n");
            break;
        }
        if (is_draw(&s)) {
            printf("DRAW!\n");
            break;
        }

        printf("[%c]> ", c);
        int i = -1;
        int j = -1;
        if (c == 'x') {
            scanf("%d %d", &i, &j);
            while (getchar() != '\n');
            if (i < 0 || i >= M || j < 0 || j >= N) {
                printf("location (%d,%d) does not exist, try again\n", i, j);
                continue;
            }
            if (!can_play(&s, i, j)) {
                printf("cannot play at (%d,%d), try again\n", i, j);
                continue;
            }
        } else {
            struct context c;
            init_context(&c, &s);
            ai(&c);
            random_pick(&c, &i, &j);
            if (c.v_max < 0)
                printf("looks like you already won, yolo let's play (%d,%d)\n", i, j);
            else
                printf("I play (%d,%d)\n", i, j);
            print_context(&c);
        }
        printf("\n");

        if (c == 'x') {
            play(s.x, i, j);
            c = 'o';
        } else {
            play(s.o, i, j);
            c = 'x';
        }
    }

    return 0;
}