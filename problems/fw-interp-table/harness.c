#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct { int32_t x; int32_t y; } point_t;

#include "nlc.h"
#include "solution.c"

static void chk_ok(const char *nm, const char *in, int32_t got, int32_t exp) {
    char g[16], e[16];
    snprintf(g, sizeof g, "%d", (int)got);
    snprintf(e, sizeof e, "%d", (int)exp);
    nlc_case(nm, got == exp, in, e, g, "");
}
static void chk_false(const char *nm, const char *in, bool got) {
    nlc_case(nm, !got, in, "false", got ? "true" : "false", "");
}

int main(void) {
    nlc_begin();

    point_t pts3[] = {{0,0},{100,200},{200,100}};
    int32_t y;
    bool r;

    /* exact matches */
    r = interp_i32(pts3, 3, 0,   &y); chk_ok("exact first", "x=0",   y, 0);
    r = interp_i32(pts3, 3, 100, &y); chk_ok("exact mid",   "x=100", y, 200);
    r = interp_i32(pts3, 3, 200, &y); chk_ok("exact last",  "x=200", y, 100);
    (void)r;

    /* interpolation */
    point_t pts2[] = {{0,0},{100,100}};
    interp_i32(pts2, 2, 50, &y); chk_ok("interp 50%",  "x=50",  y, 50);
    interp_i32(pts2, 2, 25, &y); chk_ok("interp 25%",  "x=25",  y, 25);
    interp_i32(pts2, 2, 75, &y); chk_ok("interp 75%",  "x=75",  y, 75);

    /* negative slope */
    point_t pts_neg[] = {{0,100},{100,0}};
    interp_i32(pts_neg, 2, 50, &y); chk_ok("neg slope 50", "x=50 neg", y, 50);

    /* out of range */
    point_t pts_r[] = {{10,100},{20,200}};
    chk_false("below range", "x=9",  interp_i32(pts_r, 2, 9,  &y));
    chk_false("above range", "x=21", interp_i32(pts_r, 2, 21, &y));

    /* large values (test int64_t intermediate) */
    point_t pts_big[] = {{0, 0}, {1000000, 2000000}};
    interp_i32(pts_big, 2, 500000, &y);
    chk_ok("large values", "x=500000", y, 1000000);

    nlc_end();
    return 0;
}
