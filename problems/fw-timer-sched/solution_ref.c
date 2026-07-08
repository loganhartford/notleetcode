// Time: O(T) per poll where T = TS_MAX_TIMERS; O(T) ts_start worst case
#include <stdint.h>
#include <stdbool.h>
#include <string.h>


#define TS_MAX_TIMERS 8
typedef void (*timer_cb_t)(void *ctx);
typedef struct {
    bool       active;
    bool       periodic;
    uint32_t   deadline;
    uint32_t   period;
    timer_cb_t cb;
    void      *ctx;
} timer_slot_t;
typedef struct {
    timer_slot_t slots[TS_MAX_TIMERS];
} timer_sched_t;

void ts_init(timer_sched_t *ts) {
    memset(ts, 0, sizeof *ts);
}

int ts_start(timer_sched_t *ts, uint32_t now, uint32_t delay_ms,
             bool periodic, timer_cb_t cb, void *ctx) {
    for (int i = 0; i < TS_MAX_TIMERS; i++) {
        if (!ts->slots[i].active) {
            ts->slots[i].active   = true;
            ts->slots[i].periodic = periodic;
            ts->slots[i].deadline = now + delay_ms;
            ts->slots[i].period   = delay_ms;
            ts->slots[i].cb       = cb;
            ts->slots[i].ctx      = ctx;
            return i;
        }
    }
    return -1;
}

bool ts_cancel(timer_sched_t *ts, int timer_id) {
    if (timer_id < 0 || timer_id >= TS_MAX_TIMERS) return false;
    if (!ts->slots[timer_id].active) return false;
    ts->slots[timer_id].active = false;
    return true;
}

void ts_poll(timer_sched_t *ts, uint32_t now) {
    for (int i = 0; i < TS_MAX_TIMERS; i++) {
        timer_slot_t *s = &ts->slots[i];
        if (!s->active) continue;
        if ((uint32_t)(now - s->deadline) < 0x80000000u) {
            s->cb(s->ctx);
            if (s->periodic) {
                s->deadline += s->period;
            } else {
                s->active = false;
            }
        }
    }
}
