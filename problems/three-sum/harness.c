#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "nlc.h"
#include "solution.c"

static int icmp(const void* a, const void* b) {
    int x = *(const int*)a, y = *(const int*)b;
    return (x > y) - (x < y);
}

/* Compare two triplets lexicographically (for sorting the list of triplets). */
static int tcmp(const void* a, const void* b) {
    const int* x = *(const int* const*)a;
    const int* y = *(const int* const*)b;
    for (int k = 0; k < 3; k++)
        if (x[k] != y[k]) return (x[k] > y[k]) - (x[k] < y[k]);
    return 0;
}

static void fmt_triplets(char* buf, size_t cap, int** t, int n) {
    size_t off = 0;
    off += (size_t)snprintf(buf + off, cap - off, "[");
    for (int i = 0; i < n && off < cap; i++)
        off += (size_t)snprintf(buf + off, cap - off, "%s[%d, %d, %d]",
                                i ? ", " : "", t[i][0], t[i][1], t[i][2]);
    snprintf(buf + off, cap - off, "]");
}

static void check(const char* name, int* nums, int n, const char* input, int exp[][3], int en) {
    int* a = malloc(n * sizeof(int));
    memcpy(a, nums, n * sizeof(int)); /* solution sorts in place */
    int rs = 0, *cols = NULL;

    nlc_capture_begin();
    int** r = threeSum(a, n, &rs, &cols);
    char* out = nlc_capture_end();

    /* Normalize the result: sort within each triplet, then sort the list. */
    for (int i = 0; i < rs; i++) qsort(r[i], 3, sizeof(int), icmp);
    qsort(r, rs, sizeof(int*), tcmp);

    /* Normalize a copy of the expected triplets the same way. */
    int** e = malloc((en > 0 ? en : 1) * sizeof(int*));
    for (int i = 0; i < en; i++) {
        e[i] = malloc(3 * sizeof(int));
        memcpy(e[i], exp[i], 3 * sizeof(int));
        qsort(e[i], 3, sizeof(int), icmp);
    }
    qsort(e, en, sizeof(int*), tcmp);

    int ok = (rs == en);
    for (int i = 0; ok && i < en; i++)
        if (memcmp(r[i], e[i], 3 * sizeof(int)) != 0) ok = 0;

    char gbuf[512], ebuf[512];
    fmt_triplets(gbuf, sizeof gbuf, r, rs);
    fmt_triplets(ebuf, sizeof ebuf, e, en);
    nlc_case(name, ok, input, ebuf, gbuf, out);

    for (int i = 0; i < rs; i++) free(r[i]);
    free(r);
    free(cols);
    for (int i = 0; i < en; i++) free(e[i]);
    free(e);
    free(a);
    free(out);
}

int main(void) {
    nlc_begin();
    { int a[] = {-1, 0, 1, 2, -1, -4}; int e[][3] = {{-1, -1, 2}, {-1, 0, 1}};
      check("example 1", a, 6, "nums=[-1, 0, 1, 2, -1, -4]", e, 2); }
    { int a[] = {0, 1, 1}; int e[][3] = {{0, 0, 0}};
      check("no triplet", a, 3, "nums=[0, 1, 1]", e, 0); }
    { int a[] = {0, 0, 0}; int e[][3] = {{0, 0, 0}};
      check("all zeros", a, 3, "nums=[0, 0, 0]", e, 1); }
    { int a[] = {-2, 0, 0, 2, 2}; int e[][3] = {{-2, 0, 2}};
      check("many dups", a, 5, "nums=[-2, 0, 0, 2, 2]", e, 1); }
    { int a[] = {-4, -2, -2, -2, 0, 1, 2, 2, 2, 3, 3, 4, 4, 6, 6};
      int e[][3] = {{-4, -2, 6}, {-4, 0, 4}, {-4, 1, 3}, {-4, 2, 2}, {-2, -2, 4}, {-2, 0, 2}};
      check("mixed", a, 15, "nums=[-4,-2,-2,-2,0,1,2,2,2,3,3,4,4,6,6]", e, 6); }
    nlc_end();
    return 0;
}
