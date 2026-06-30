#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node { struct node *next; int value; } node_t;

#include "nlc.h"
#include "solution.c"

/* Build list from array */
static void build(node_t *nodes, const int *vals, int n) {
    for (int i = 0; i < n; i++) {
        nodes[i].value = vals[i];
        nodes[i].next  = (i+1 < n) ? &nodes[i+1] : NULL;
    }
}
/* Check list has expected sequence */
static void chk_seq(const char *nm, const char *in,
                    node_t *head, const int *exp, int n) {
    node_t *cur = head;
    int ok = 1;
    char g[64]="", e[64]="";
    for (int i = 0; i < n; i++) {
        char tmp[8];
        snprintf(tmp, sizeof tmp, "%d ", exp[i]);
        strncat(e, tmp, sizeof e - strlen(e) - 1);
        if (!cur) { ok = 0; strncat(g, "NULL ", sizeof g - strlen(g) - 1); }
        else {
            snprintf(tmp, sizeof tmp, "%d ", cur->value);
            strncat(g, tmp, sizeof g - strlen(g) - 1);
            if (cur->value != exp[i]) ok = 0;
            cur = cur->next;
        }
    }
    if (cur) ok = 0; /* extra nodes */
    nlc_case(nm, ok, in, e, g, "");
}

int main(void) {
    nlc_begin();

    /* both empty */
    node_t *r = list_merge_sorted(NULL, NULL);
    nlc_case("both NULL", r == NULL, "merge(NULL,NULL)", "NULL", r ? "non-NULL" : "NULL", "");

    /* one empty */
    node_t b1[3];
    const int bv[] = {2,4,6};
    build(b1, bv, 3);
    r = list_merge_sorted(NULL, b1);
    const int exp1[] = {2,4,6};
    chk_seq("a=NULL merged", "merge(NULL,[2,4,6])", r, exp1, 3);

    /* interleaved */
    node_t na[3], nb[3];
    const int av[] = {1,3,5}, nv[] = {2,4,6};
    build(na, av, 3); build(nb, nv, 3);
    r = list_merge_sorted(na, nb);
    const int exp2[] = {1,2,3,4,5,6};
    chk_seq("interleaved", "merge([1,3,5],[2,4,6])", r, exp2, 6);

    /* duplicates (stable: a first) */
    node_t da[2], db[2];
    const int dav[] = {1,2}, dbv[] = {1,2};
    build(da, dav, 2); build(db, dbv, 2);
    r = list_merge_sorted(da, db);
    const int exp3[] = {1,1,2,2};
    chk_seq("duplicates", "merge([1,2],[1,2])", r, exp3, 4);

    /* negative values */
    node_t na2[2], nb2[2];
    const int av2[] = {-3,-1}, bv2[] = {-2,0};
    build(na2, av2, 2); build(nb2, bv2, 2);
    r = list_merge_sorted(na2, nb2);
    const int exp4[] = {-3,-2,-1,0};
    chk_seq("negative vals", "merge([-3,-1],[-2,0])", r, exp4, 4);

    nlc_end();
    return 0;
}
