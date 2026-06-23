#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "nlc.h"

struct ListNode { int val; struct ListNode* next; };

#include "solution.c"

/* Build a list from a[0..n); if pos >= 0, link the tail back to node[pos]. */
static struct ListNode* build_cycle(const int* a, int n, int pos) {
    if (n == 0) return NULL;
    struct ListNode* nodes[1024];
    for (int i = 0; i < n; i++) {
        nodes[i] = malloc(sizeof(struct ListNode));
        nodes[i]->val = a[i];
        nodes[i]->next = NULL;
    }
    for (int i = 0; i < n - 1; i++) nodes[i]->next = nodes[i + 1];
    if (pos >= 0) nodes[n - 1]->next = nodes[pos];
    return nodes[0];
}

static void check(const char* name, const char* input, const int* a, int n, int pos, bool exp) {
    struct ListNode* head = build_cycle(a, n, pos);
    nlc_capture_begin();
    bool got = hasCycle(head);
    char* out = nlc_capture_end();
    nlc_case(name, got == exp, input, exp ? "true" : "false", got ? "true" : "false", out);
    free(out);
}

int main(void) {
    nlc_begin();
    { int a[] = {3, 2, 0, -4}; check("cycle at 1", "[3,2,0,-4], pos=1", a, 4, 1, true); }
    { int a[] = {1, 2};        check("cycle at 0", "[1,2], pos=0", a, 2, 0, true); }
    { int a[] = {1};           check("single no cycle", "[1], pos=-1", a, 1, -1, false); }
    { int a[] = {1, 2, 3};     check("no cycle", "[1,2,3], pos=-1", a, 3, -1, false); }
    { int a[] = {1};           check("self loop", "[1], pos=0", a, 1, 0, true); }
    { check("empty", "[], pos=-1", NULL, 0, -1, false); }
    nlc_end();
    return 0;
}
