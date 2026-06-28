// Time: O(1) sched_add · O(T) sched_tick  — iterates all T registered tasks each tick
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
    s->count = 0;
    s->tick = 0;
}

int sched_add(scheduler_t* s, task_fn fn, uint32_t period) {
    if (s->count >= 16) return -1;
    s->tasks[s->count].fn = fn;
    s->tasks[s->count].period = period;
    return s->count++;
}

void sched_tick(scheduler_t* s) {
    s->tick++;
    for (int i = 0; i < s->count; i++) {
        if (s->tick % s->tasks[i].period == 0) {
            s->tasks[i].fn();
        }
    }
}
