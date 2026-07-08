#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "nlc.h"
#include "solution.c"

static const char *ev_name(db_event_t e) {
    switch(e){ case DB_RISING: return "DB_RISING"; case DB_FALLING: return "DB_FALLING"; default: return "DB_NONE"; }
}
static void chk_ev(const char *nm, const char *in, db_event_t got, db_event_t exp) {
    nlc_case(nm, got == exp, in, ev_name(exp), ev_name(got), "");
}
static void chk_bool(const char *nm, const char *in, bool got, bool exp) {
    nlc_case(nm, got == exp, in, exp?"true":"false", got?"true":"false", "");
}

int main(void) {
    nlc_begin();

    debounce_t d;

    /* clean rising edge */
    db_init(&d, false, 10);
    chk_ev("rising: start none", "db_update(true,0)",   db_update(&d, true, 0),  DB_NONE);
    chk_ev("rising: still none", "db_update(true,9)",   db_update(&d, true, 9),  DB_NONE);
    chk_ev("rising: fires",      "db_update(true,10)",  db_update(&d, true, 10), DB_RISING);
    chk_bool("level=true", "db_level after rising", db_level(&d), true);
    chk_ev("rising: no refire",  "db_update(true,100)", db_update(&d, true, 100),DB_NONE);

    /* clean falling edge */
    db_init(&d, true, 10);
    chk_ev("falling: start",  "db_update(false,0)",  db_update(&d, false, 0),  DB_NONE);
    chk_ev("falling: fires",  "db_update(false,10)", db_update(&d, false, 10), DB_FALLING);
    chk_bool("level=false", "db_level after falling", db_level(&d), false);

    /* bounce that resets */
    db_init(&d, false, 10);
    db_update(&d, true,  0);   /* start tracking */
    db_update(&d, false, 5);   /* bounce back → reset */
    chk_ev("bounced: restart", "db_update(true,6)",  db_update(&d, true, 6),  DB_NONE);
    chk_ev("bounced: none 15", "db_update(true,15)", db_update(&d, true, 15), DB_NONE);
    chk_ev("bounced: fires 16","db_update(true,16)", db_update(&d, true, 16), DB_RISING);

    /* no change → DB_NONE */
    db_init(&d, true, 10);
    chk_ev("no change", "db_update(true,100)", db_update(&d, true, 100), DB_NONE);

    /* wraparound */
    db_init(&d, false, 20);
    db_update(&d, true, 0xFFFFFFF0u);
    chk_ev("wrap: not yet", "db_update(true,0xFFFFFFFF)", db_update(&d, true, 0xFFFFFFFFu), DB_NONE);
    chk_ev("wrap: fires",   "db_update(true,0x00000004)", db_update(&d, true, 0x00000004u), DB_RISING);

    nlc_end();
    return 0;
}
