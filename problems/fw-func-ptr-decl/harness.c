#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "nlc.h"
#include "solution.c"

#define SAME(T, U) __builtin_types_compatible_p(T, U)

#define IS_A(T) SAME(T, int)
#define IS_B(T) SAME(T, int *)
#define IS_C(T) SAME(T, int **)
#define IS_D(T) (SAME(T, int[10]) && !SAME(T, int[11]))
#define IS_E(T) (SAME(T, int *[10]) && !SAME(T, int *[11]))
#define IS_F(T) (SAME(T, int (*)[10]) && !SAME(T, int (*)[11]))
#define IS_G(T) (SAME(T, int (*)(int)) && !SAME(T, int (*)(long)))
#define IS_H(T) (SAME(T, int (*[10])(int)) && !SAME(T, int (*[11])(int)) \
                 && !SAME(T, int (*[10])(long)))

#define DESCRIBE(T) (                                                              \
    IS_A(T) ? "int" :                                                              \
    IS_B(T) ? "int *" :                                                            \
    IS_C(T) ? "int **" :                                                           \
    IS_D(T) ? "int [10]" :                                                         \
    IS_E(T) ? "int *[10]" :                                                        \
    IS_F(T) ? "int (*)[10]" :                                                      \
    IS_G(T) ? "int (*)(int)" :                                                     \
    IS_H(T) ? "int (*[10])(int)" :                                                 \
    SAME(T, void) ? "void (placeholder)" :                                         \
    (SAME(T, int[10]) && SAME(T, int[11])) ? "int [] (array with no length)" :     \
    (SAME(T, int (*)[10]) && SAME(T, int (*)[11]))                                 \
        ? "int (*)[] (pointer to array with no length)" :                          \
    (SAME(T, int (*)(int)) && SAME(T, int (*)(long)))                              \
        ? "pointer to a function with no prototype" :                              \
    SAME(T, int[]) ? "int array of the wrong length" :                             \
    SAME(T, int *[]) ? "array of int * of the wrong length" :                      \
    SAME(T, int (*[])(int)) ? "array of function pointers of the wrong length" :   \
    SAME(T, int (int)) ? "int (int): a function type, not a pointer" :             \
    SAME(T, int *(int)) ? "int *(int): a function returning int *" :               \
    SAME(T, int *(*)(int)) ? "int *(*)(int): returns int *, not int" :             \
    SAME(T, int (**)(int)) ? "int (**)(int): pointer to a function pointer" :      \
    SAME(T, int (*)(void)) ? "int (*)(void): takes no argument" :                  \
    SAME(T, int *(*)[10]) ? "int *(*)[10]: pointer to array of int *" :            \
    "a different type")

#define CHECK_TYPE(name, T, IS, expect)                                            \
    nlc_case(name, IS(T), #T, expect, DESCRIBE(T), "")

#define SENTINEL 12345

static void chk_dispatch(const char *name, const char *input, unsigned op,
                         int a, int b, int exp_ret, int exp_out) {
    int out = SENTINEL;
    int ret = dispatch(op, a, b, &out);
    char g[64], e[64];
    snprintf(g, sizeof g, "ret=%d, out=%d", ret, out);
    if (exp_ret == 0)
        snprintf(e, sizeof e, "ret=0, out=%d", exp_out);
    else
        snprintf(e, sizeof e, "ret=-1, out=%d (unchanged)", SENTINEL);
    nlc_case(name, ret == exp_ret && out == exp_out, input, e, g, "");
}

int main(void) {
    nlc_begin();

    CHECK_TYPE("(a) an integer", T_a, IS_A, "int");
    CHECK_TYPE("(b) a pointer to an integer", T_b, IS_B, "int *");
    CHECK_TYPE("(c) a pointer to a pointer to an integer", T_c, IS_C, "int **");
    CHECK_TYPE("(d) an array of 10 integers", T_d, IS_D, "int [10]");
    CHECK_TYPE("(e) an array of 10 pointers to integers", T_e, IS_E, "int *[10]");
    CHECK_TYPE("(f) a pointer to an array of 10 integers", T_f, IS_F, "int (*)[10]");
    CHECK_TYPE("(g) a pointer to a function taking int, returning int",
               T_g, IS_G, "int (*)(int)");
    CHECK_TYPE("(h) an array of 10 pointers to functions taking int, returning int",
               T_h, IS_H, "int (*[10])(int)");

    chk_dispatch("op 0: add", "dispatch(0, 7, 5, &out)", 0, 7, 5, 0, 12);
    chk_dispatch("op 1: subtract (a - b)", "dispatch(1, 7, 5, &out)", 1, 7, 5, 0, 2);
    chk_dispatch("op 2: multiply", "dispatch(2, -6, 7, &out)", 2, -6, 7, 0, -42);
    chk_dispatch("op 3: divide", "dispatch(3, 17, 5, &out)", 3, 17, 5, 0, 3);
    chk_dispatch("divide truncates toward zero", "dispatch(3, -17, 5, &out)", 3, -17, 5, 0, -3);
    chk_dispatch("b = 0 is fine for add", "dispatch(0, 9, 0, &out)", 0, 9, 0, 0, 9);
    chk_dispatch("op one past the table", "dispatch(4, 1, 1, &out)", 4, 1, 1, -1, SENTINEL);
    chk_dispatch("op = UINT_MAX", "dispatch(UINT_MAX, 1, 1, &out)", UINT_MAX, 1, 1, -1, SENTINEL);
    chk_dispatch("divide by zero", "dispatch(3, 5, 0, &out)", 3, 5, 0, -1, SENTINEL);
    {
        int ret = dispatch(0, 1, 2, NULL);
        char g[32];
        snprintf(g, sizeof g, "ret=%d", ret);
        nlc_case("NULL out", ret == -1, "dispatch(0, 1, 2, NULL)", "ret=-1", g, "");
    }
    {
        int out = SENTINEL;
        int r1 = dispatch(1, 2, 9, &out);
        int o1 = out;
        int r2 = dispatch(0, out, 10, &out);
        char g[64];
        snprintf(g, sizeof g, "ret=%d, out=%d; ret=%d, out=%d", r1, o1, r2, out);
        nlc_case("chained calls", r1 == 0 && o1 == -7 && r2 == 0 && out == 3,
                 "dispatch(1, 2, 9, &out); dispatch(0, out, 10, &out)",
                 "ret=0, out=-7; ret=0, out=3", g, "");
    }

    nlc_end();
    return 0;
}
