# Tiny Cooperative Scheduler

Implement a minimal **cooperative scheduler** that runs several tasks at different
rates off a single tick. No preemption — tasks are plain functions called in turn
when they are due.

```c
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

void sched_init(scheduler_t* s);
int  sched_add(scheduler_t* s, task_fn fn, uint32_t period);  // returns task id, or -1 if full
void sched_tick(scheduler_t* s);                              // advance one tick, run due tasks
```

Semantics:
- `sched_init` zeroes the scheduler (`count = 0`, `tick = 0`).
- `sched_add` registers a task with a period (in ticks); returns its index, or
  `-1` if the table (16 slots) is full.
- `sched_tick` **increments the tick counter first**, then runs every task whose
  period divides the new tick value (`tick % period == 0`). Tasks run in the order
  they were added.

### Example
```
add task A (period 2), task B (period 3)
run 6 ticks:
  tick 1: (none)
  tick 2: A
  tick 3: B
  tick 4: A
  tick 5: (none)
  tick 6: A, B
-> A ran 3 times, B ran 2 times
```

### Constraints
- At most 16 tasks; periods are `>= 1`.
- `[author]`-defined firmware practice.
