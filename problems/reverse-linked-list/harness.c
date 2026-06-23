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
static void check(const char* name, const int* in, int n, const int* exp, int en) {
    char inbuf[256];
    nlc_fmt_ints(inbuf, sizeof inbuf, in, n);
    struct ListNode* head = build(in, n);
    nlc_capture_begin();
    struct ListNode* res = reverseList(head);
    char* out = nlc_capture_end();
    int got[256];
    int gn = to_array(res, got, 256);
    int ok = (gn == en);
    for (int i = 0; ok && i < en; i++) if (got[i] != exp[i]) ok = 0;
    char gb[256], eb[256];
    nlc_fmt_ints(gb, sizeof gb, got, gn);
    nlc_fmt_ints(eb, sizeof eb, exp, en);
    nlc_case(name, ok, inbuf, eb, gb, out);
    free(out);
}

int main(void) {
    nlc_begin();
    { int a[] = {1, 2, 3, 4, 5}; int e[] = {5, 4, 3, 2, 1}; check("five", a, 5, e, 5); }
    { int a[] = {1, 2};          int e[] = {2, 1};          check("two", a, 2, e, 2); }
    { int e[] = {0};             check("empty", NULL, 0, e, 0); }
    { int a[] = {7};             int e[] = {7};             check("single", a, 1, e, 1); }
    nlc_end();
    return 0;
}
