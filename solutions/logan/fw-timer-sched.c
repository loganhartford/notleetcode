#include <stdint.h>
#include <stdbool.h>


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
    // way cleaner way to initialize
    memset(ts, 0, sizeof(*ts));
    
    // for (int i = 0; i < TS_MAX_TIMERS; i++) {
    //     timer_slot_t *slot = &ts->slots[i];
    //     slot->active = false;
    //     slot->periodic = false;
    //     slot->deadline = 0;
    //     slot->cb = NULL;
    //     slot->ctx = NULL;
    // }
}

int ts_start(timer_sched_t *ts, uint32_t now, uint32_t delay_ms,
             bool periodic, timer_cb_t cb, void *ctx) {
    for (int i = 0; i < TS_MAX_TIMERS; i++) {
        timer_slot_t *slot = &ts->slots[i];
        if (!slot->active) {
            slot->active = true;
            slot->periodic = periodic;
            slot->deadline = now + delay_ms;
            slot->period = delay_ms;
            slot->cb = cb;
            slot->ctx = ctx;
            return i;
        }
    }
    return -1;
}

bool ts_cancel(timer_sched_t *ts, int timer_id) {
    if ((timer_id < 0) || (timer_id > TS_MAX_TIMERS)) return false;
    timer_slot_t *slot = &ts->slots[timer_id];
    if (!slot->active) return false;
    slot->active = false;
    return true;
}

void ts_poll(timer_sched_t *ts, uint32_t now) {
    for (int i = 0; i < TS_MAX_TIMERS; i++) {
        timer_slot_t *slot = &ts->slots[i];
        if (slot->active) {
            if ((uint32_t)(now - slot->deadline) < 0x80000000u) {
                slot->cb(slot->ctx);
                if (slot->periodic) {
                    slot->deadline += slot->period;
                } else {
                    slot->active = false;
                }
            }
        }
    }
}
