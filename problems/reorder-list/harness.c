#include <stdio.h>
#include <stdlib.h>
#include "nlc.h"

struct ListNode { int val; struct ListNode* next; };

#include "solution.c"

static struct ListNode* build(const int* a, int n) {
    struct ListNode dummy = {0, NULL};
    struct ListNode* tail = &dummy;
    for (int i = 0; i < n; i++) {
        struct ListNode* nd = malloc(sizeof(struct ListNode));
        nd->val = a[i];
        nd->next = NULL;
        tail->next = nd;
        tail = nd;
    }
    return dummy.next;
}
static int to_array(struct ListNode* h, int* out, int cap) {
    int n = 0;
    while (h && n < cap) { out[n++] = h->val; h = h->next; }
    return n;
}
static void check(const char* name, const char* input, const int* a, int n, const int* exp, int en) {
    struct ListNode* head = build(a, n);
    nlc_capture_begin();
    reorderList(head);
    char* out = nlc_capture_end();
    int got[256];
    int gn = to_array(head, got, 256);
    int ok = (gn == en);
    for (int i = 0; ok && i < en; i++) if (got[i] != exp[i]) ok = 0;
    char gb[256], eb[256];
    nlc_fmt_ints(gb, sizeof gb, got, gn);
    nlc_fmt_ints(eb, sizeof eb, exp, en);
    nlc_case(name, ok, input, eb, gb, out);
    free(out);
}

int main(void) {
    nlc_begin();
    { int a[] = {1, 2, 3, 4};    int e[] = {1, 4, 2, 3};    check("even", "[1,2,3,4]", a, 4, e, 4); }
    { int a[] = {1, 2, 3, 4, 5}; int e[] = {1, 5, 2, 4, 3}; check("odd", "[1,2,3,4,5]", a, 5, e, 5); }
    { int a[] = {1};             int e[] = {1};             check("single", "[1]", a, 1, e, 1); }
    { int a[] = {1, 2};          int e[] = {1, 2};          check("two", "[1,2]", a, 2, e, 2); }
    { int a[] = {1, 2, 3};       int e[] = {1, 3, 2};       check("three", "[1,2,3]", a, 3, e, 3); }
    nlc_end();
    return 0;
}
