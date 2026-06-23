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
static void check(const char* name, const char* input, const int* a, int n, int rm, const int* exp, int en) {
    struct ListNode* head = build(a, n);
    nlc_capture_begin();
    struct ListNode* res = removeNthFromEnd(head, rm);
    char* out = nlc_capture_end();
    int got[256];
    int gn = to_array(res, got, 256);
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
    { int a[] = {1, 2, 3, 4, 5}; int e[] = {1, 2, 3, 5}; check("remove 2nd from end", "[1,2,3,4,5], n=2", a, 5, 2, e, 4); }
    { int a[] = {1};             int e[] = {0};          check("remove only node", "[1], n=1", a, 1, 1, e, 0); }
    { int a[] = {1, 2};          int e[] = {1};          check("remove last", "[1,2], n=1", a, 2, 1, e, 1); }
    { int a[] = {1, 2};          int e[] = {2};          check("remove head", "[1,2], n=2", a, 2, 2, e, 1); }
    { int a[] = {1, 2, 3, 4, 5}; int e[] = {2, 3, 4, 5}; check("remove head of 5", "[1,2,3,4,5], n=5", a, 5, 5, e, 4); }
    nlc_end();
    return 0;
}
