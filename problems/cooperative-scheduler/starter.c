#include <stdint.h>

typedef void (*task_fn)(void);

typedef struct {
    task_fn  fn;
    uint32_t period;
} task_t;

typedef struct {
    task_t   tasks[16];
    int      count;
    uint32_t tick;
} scheduler_t;

void sched_init(scheduler_t* s) {
}

int sched_add(scheduler_t* s, task_fn fn, uint32_t period) {
    return -1;
}

void sched_tick(scheduler_t* s) {
}
