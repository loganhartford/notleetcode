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
static void check(const char* name, const char* input,
                  const int* a, int an, const int* b, int bn, const int* exp, int en) {
    struct ListNode* l1 = build(a, an);
    struct ListNode* l2 = build(b, bn);
    nlc_capture_begin();
    struct ListNode* res = mergeTwoLists(l1, l2);
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
    { int a[] = {1, 2, 4}; int b[] = {1, 3, 4}; int e[] = {1, 1, 2, 3, 4, 4};
      check("interleave", "list1=[1,2,4], list2=[1,3,4]", a, 3, b, 3, e, 6); }
    { int e[] = {0}; check("both empty", "list1=[], list2=[]", NULL, 0, NULL, 0, e, 0); }
    { int b[] = {0}; int e[] = {0}; check("one empty", "list1=[], list2=[0]", NULL, 0, b, 1, e, 1); }
    { int a[] = {5}; int b[] = {1, 2, 3}; int e[] = {1, 2, 3, 5};
      check("append", "list1=[5], list2=[1,2,3]", a, 1, b, 3, e, 4); }
    nlc_end();
    return 0;
}
