#include <stddef.h>

typedef int T_a;
typedef int *T_b;
typedef int **T_c;
typedef int T_d[10];
typedef int *T_e[10];
typedef int (*T_f)[10];
typedef int (*T_g)(int);
typedef int (*T_h[10])(int);

typedef int (*binop_fn)(int a, int b, int *out);

static int op_add(int a, int b, int *out) { *out = a + b; return 0; }
static int op_sub(int a, int b, int *out) { *out = a - b; return 0; }
static int op_mul(int a, int b, int *out) { *out = a * b; return 0; }

static int op_div(int a, int b, int *out) {
    if (b == 0) return -1;
    *out = a / b;
    return 0;
}

int dispatch(unsigned op, int a, int b, int *out) {
    static const binop_fn table[] = { op_add, op_sub, op_mul, op_div };

    if (out == NULL || op >= sizeof table / sizeof table[0]) return -1;
    return table[op](a, b, out);
}
