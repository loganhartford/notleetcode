#include <stdint.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include "nlc.h"
#include "solution.c"

static unsigned mask, saves, restores;
static uint32_t before, after;
static volatile uint32_t *word;

irq_state_t irq_save(void) {
    unsigned old = mask;
    if (!mask) *word |= before;
    before = 0;
    mask = 1;
    ++saves;
    return old;
}

void irq_restore(irq_state_t previous) {
    mask = previous;
    ++restores;
    if (!mask) { *word |= after; after = 0; }
}

static void check(const char *name, uint32_t start, unsigned initial_mask,
                  uint32_t early, uint32_t late) {
    volatile uint32_t pending = start;
    word = &pending;
    mask = initial_mask; saves = restores = 0; before = early; after = late;
    uint32_t got = take_pending_events(&pending);
    uint32_t expected = start | (initial_mask ? 0 : early);
    uint32_t remaining = initial_mask ? 0 : late;
    bool ok = got == expected && pending == remaining && mask == initial_mask && saves == 1 && restores == 1;
    char input[256], g[160], e[160];
    snprintf(input, sizeof input,
             "take_pending_events: initial pending=0x%08" PRIX32 ", interrupts=%s, event before mask=0x%08" PRIX32 ", event at restore=0x%08" PRIX32,
             start, initial_mask ? "disabled" : "enabled", early, late);
    snprintf(g, sizeof g, "snapshot=0x%08" PRIX32 ", pending=0x%08" PRIX32 ", interrupt mask=%u, save/restore calls=%u/%u", got, pending, mask, saves, restores);
    snprintf(e, sizeof e, "snapshot=0x%08" PRIX32 ", pending=0x%08" PRIX32 ", interrupt mask=%u, save/restore calls=1/1", expected, remaining, initial_mask);
    nlc_case(name, ok, input, e, g, "");
}

int main(void) {
    nlc_begin();
    volatile uint32_t p = 0;
    post_events_from_isr(&p, 1); post_events_from_isr(&p, 4); post_events_from_isr(&p, 1);
    char output[64];
    snprintf(output, sizeof output, "pending=0x%08" PRIX32, p);
    nlc_case("OR and coalesce", p == 5, "initial pending=0x00000000; post_events_from_isr events=[0x1, 0x4, 0x1]", "pending=0x00000005", output, "");
    p = 5;
    post_events_from_isr(&p, 0);
    snprintf(output, sizeof output, "pending=0x%08" PRIX32, p);
    nlc_case("posting zero preserves events", p == 5, "initial pending=0x00000005; post_events_from_isr events=[0x0]", "pending=0x00000005", output, "");
    check("empty", 0, 0, 0, 0);
    check("basic snapshot", 5, 0, 0, 0);
    check("high bit", UINT32_C(0x80000000), 0, 0, 0);
    check("all bits", UINT32_MAX, 0, 0, 0);
    check("interrupt before masking", 1, 0, 4, 0);
    check("interrupt at restore", 1, 0, 0, 8);
    check("same bit arrives again at restore", 1, 0, 0, 1);
    check("both boundaries", 1, 0, 2, 4);
    check("preserve disabled mask", 5, 1, 0, 8);
    check("empty with disabled mask", 0, 1, 0, 0);
    nlc_end();
    return 0;
}
