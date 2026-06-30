#include <stdint.h>
#include <stdbool.h>

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
