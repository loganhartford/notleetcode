#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "nlc.h"
#include "solution.c"

#define STR_(x) #x
#define XSTR(x) STR_(x)

struct sample {
    char     tag;
    uint32_t id;
    uint16_t flags;
    double   value;
    uint8_t  bytes[3];
    char     tail;
};

struct node { struct node *next; };

struct task {
    uint8_t     priority;
    uint32_t    id;
    struct node link;
    char        name[8];
};

static void off_case(const char *name, const char *in, size_t got, size_t exp) {
    char g[24], e[24];
    snprintf(g, sizeof g, "%zu", got);
    snprintf(e, sizeof e, "%zu", exp);
    nlc_case(name, got == exp, in, e, g, "");
}

#define OFF_CASE(name, type, member) \
    off_case(name, "MY_OFFSETOF(" #type ", " #member ")", \
             (size_t)MY_OFFSETOF(type, member), offsetof(type, member))

static void fmt_rel(char *buf, size_t cap, const void *got, const void *base, const char *base_name) {
    intptr_t d = (intptr_t)got - (intptr_t)base;
    long mag = (long)(d < 0 ? -d : d);
    if (!got) snprintf(buf, cap, "NULL");
    else if (d == 0) snprintf(buf, cap, "%s", base_name);
    else snprintf(buf, cap, "%s %c %ld byte%s", base_name, d > 0 ? '+' : '-', mag, mag == 1 ? "" : "s");
}

static void ptr_case(const char *name, const char *in, const void *got, const void *exp, const char *exp_name) {
    char g[48];
    fmt_rel(g, sizeof g, got, exp, exp_name);
    nlc_case(name, got == exp, in, exp_name, g, "");
}

static struct sample *from_tag(char *p)       { return MY_CONTAINER_OF(p, struct sample, tag); }
static struct sample *from_flags(uint16_t *p) { return MY_CONTAINER_OF(p, struct sample, flags); }
static struct sample *from_value(double *p)   { return MY_CONTAINER_OF(p, struct sample, value); }

static struct sample *from_either(int pick, struct sample *a, struct sample *b) {
    return MY_CONTAINER_OF(pick ? &a->flags : &b->flags, struct sample, flags);
}

static void list_case(void) {
    struct task t[3] = {
        {1, 10, {NULL}, "rx"},
        {2, 20, {NULL}, "tx"},
        {3, 30, {NULL}, "idle"},
    };
    static const int order[3] = {2, 0, 1};
    t[2].link.next = &t[0].link;
    t[0].link.next = &t[1].link;

    int ids[3], count = 0;
    char got[64] = "";
    for (struct node *n = &t[2].link; n && count < 3; n = n->next) {
        struct task *tp = MY_CONTAINER_OF(n, struct task, link);
        struct task *want = &t[order[count]];
        if (tp != want) {
            char base[16], rel[48];
            snprintf(base, sizeof base, "&t[%d]", order[count]);
            fmt_rel(rel, sizeof rel, tp, want, base);
            snprintf(got, sizeof got, "node %d -> %s", count, rel);
            break;
        }
        ids[count++] = (int)tp->id;
    }
    if (count == 3) nlc_fmt_ints(got, sizeof got, ids, 3);

    int exp_ids[3] = {30, 10, 20};
    char exp[32];
    nlc_fmt_ints(exp, sizeof exp, exp_ids, 3);
    nlc_case("container_of: walk an intrusive list", count == 3,
             "list t[2] -> t[0] -> t[1] via struct node link; ids visited", exp, got, "");
}

static void no_offsetof_case(void) {
    const char *a = XSTR(MY_OFFSETOF(struct sample, value));
    const char *b = XSTR(MY_CONTAINER_OF(p, struct sample, value));
    const char *bad = strstr(a, "offsetof") ? a : strstr(b, "offsetof") ? b : NULL;
    char got[256];
    if (bad) snprintf(got, sizeof got, "expands to %s", bad);
    else snprintf(got, sizeof got, "does not use offsetof");
    nlc_case("macros do not use offsetof", !bad, "preprocessor expansion of both macros",
             "does not use offsetof", got, "");
}

int main(void) {
    nlc_begin();

    OFF_CASE("offsetof: first member is at 0", struct sample, tag);
    OFF_CASE("offsetof: uint32_t after a char", struct sample, id);
    OFF_CASE("offsetof: uint16_t after a uint32_t", struct sample, flags);
    OFF_CASE("offsetof: double after a uint16_t", struct sample, value);
    OFF_CASE("offsetof: array member", struct sample, bytes);
    OFF_CASE("offsetof: char after an array", struct sample, tail);
    OFF_CASE("offsetof: struct member holding a pointer", struct task, link);

    nlc_case("offsetof: result has type size_t",
             _Generic(MY_OFFSETOF(struct sample, id), size_t: 1, default: 0),
             "_Generic(MY_OFFSETOF(struct sample, id), ...)", "size_t",
             _Generic(MY_OFFSETOF(struct sample, id), size_t: "size_t", default: "another type"), "");

    no_offsetof_case();

    struct sample s1, s2;

    ptr_case("container_of: from the first member",
             "MY_CONTAINER_OF(&s.tag, struct sample, tag)", from_tag(&s1.tag), &s1, "&s");
    ptr_case("container_of: from a middle member",
             "MY_CONTAINER_OF(&s.flags, struct sample, flags)", from_flags(&s1.flags), &s1, "&s");
    ptr_case("container_of: from a padded double member",
             "MY_CONTAINER_OF(&s.value, struct sample, value)", from_value(&s1.value), &s1, "&s");
    ptr_case("container_of: ptr argument is an expression",
             "MY_CONTAINER_OF(pick ? &a->flags : &b->flags, struct sample, flags), pick = 1",
             from_either(1, &s1, &s2), &s1, "a");

    nlc_case("container_of: result has type struct sample *",
             _Generic(MY_CONTAINER_OF(&s1.flags, struct sample, flags), struct sample *: 1, default: 0),
             "_Generic(MY_CONTAINER_OF(&s.flags, struct sample, flags), ...)", "struct sample *",
             _Generic(MY_CONTAINER_OF(&s1.flags, struct sample, flags),
                      struct sample *: "struct sample *", default: "another type"), "");

    list_case();

    nlc_end();
    return 0;
}
