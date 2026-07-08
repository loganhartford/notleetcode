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
    // TODO
}

int ts_start(timer_sched_t *ts, uint32_t now, uint32_t delay_ms,
             bool periodic, timer_cb_t cb, void *ctx) {
    // TODO
    return -1;
}

bool ts_cancel(timer_sched_t *ts, int timer_id) {
    // TODO
    return false;
}

void ts_poll(timer_sched_t *ts, uint32_t now) {
    // TODO
}
