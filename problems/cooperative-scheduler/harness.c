#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "nlc.h"
#include "solution.c"

static int countA, countB, countC;
static void taskA(void) { countA++; }
static void taskB(void) { countB++; }
static void taskC(void) { countC++; }

static void case_int(const char* name, const char* input, int got, int exp, const char* out) {
    char g[16], e[16];
    snprintf(g, sizeof g, "%d", got);
    snprintf(e, sizeof e, "%d", exp);
    nlc_case(name, got == exp, input, e, g, out);
}

int main(void) {
    countA = countB = countC = 0;
    int idA, idB, idC;

    nlc_capture_begin();
    scheduler_t s;
    sched_init(&s);
    idA = sched_add(&s, taskA, 2);
    idB = sched_add(&s, taskB, 3);
    idC = sched_add(&s, taskC, 4);
    for (int t = 0; t < 12; t++) sched_tick(&s);
    char* out = nlc_capture_end();

    nlc_begin();
    case_int("task ids assigned in order", "sched_add x3", (idA == 0 && idB == 1 && idC == 2) ? 1 : 0, 1, out);
    case_int("period 2 over 12 ticks", "taskA runs", countA, 6, "");
    case_int("period 3 over 12 ticks", "taskB runs", countB, 4, "");
    case_int("period 4 over 12 ticks", "taskC runs", countC, 3, "");

    /* fill the table to verify the -1 full case */
    scheduler_t s2;
    sched_init(&s2);
    int ok_full = 1;
    for (int i = 0; i < 16; i++) if (sched_add(&s2, taskA, 1) != i) ok_full = 0;
    int overflow = sched_add(&s2, taskA, 1);
    case_int("17th add returns -1", "table full", (ok_full && overflow == -1) ? 1 : 0, 1, "");
    nlc_end();
    free(out);
    return 0;
}
