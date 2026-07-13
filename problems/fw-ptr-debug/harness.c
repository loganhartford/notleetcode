#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "nlc.h"
#include "solution.c"

static void chk_int(const char *nm, const char *in, int got, int exp) {
    char g[24], e[24];
    snprintf(g, sizeof g, "%d", got);
    snprintf(e, sizeof e, "%d", exp);
    nlc_case(nm, got == exp, in, e, g, "");
}
static void chk_ptr(const char *nm, const char *in, void *got, void *exp) {
    nlc_case(nm, got == exp, in, "correct addr", got == exp ? "correct addr" : "wrong addr", "");
}

int main(void) {
    nlc_begin();

    /* ptr_write_int */
    int x = 0;
    ptr_write_int(&x, 42);
    chk_int("write_int 42",  "ptr_write_int(&x,42) → x==42",  x, 42);
    ptr_write_int(&x, -1);
    chk_int("write_int -1",  "ptr_write_int(&x,-1) → x==-1",  x, -1);

    /* ptr_read_int */
    int y = 99;
    chk_int("read_int 99",   "ptr_read_int(&99) == 99",  ptr_read_int(&y), 99);
    int z = -5;
    chk_int("read_int -5",   "ptr_read_int(&-5) == -5",  ptr_read_int(&z), -5);

    /* ptr_store_ptr / ptr_load_ptr */
    static uint8_t slot[sizeof(void *)] __attribute__((aligned(8)));
    int a, b;
    ptr_store_ptr(slot, &a);
    chk_ptr("store &a",      "ptr_store_ptr(slot,&a)",        ptr_load_ptr(slot), &a);
    ptr_store_ptr(slot, &b);
    chk_ptr("store &b",      "ptr_store_ptr(slot,&b)",        ptr_load_ptr(slot), &b);
    ptr_store_ptr(slot, NULL);
    chk_ptr("store NULL",    "ptr_store_ptr(slot,NULL)",      ptr_load_ptr(slot), NULL);

    /* ptr_next_byte */
    uint8_t arr[4] = {0};
    chk_ptr("next_byte 0",  "ptr_next_byte(&arr[0])==&arr[1]", ptr_next_byte(&arr[0]), &arr[1]);
    chk_ptr("next_byte 2",  "ptr_next_byte(&arr[2])==&arr[3]", ptr_next_byte(&arr[2]), &arr[3]);

    nlc_end();
    return 0;
}
