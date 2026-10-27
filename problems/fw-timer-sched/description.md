# Wraparound-Safe Software Timer Scheduler

Implement a software timer scheduler that correctly handles 32-bit millisecond counter wraparound. The system tick counter wraps around from `UINT32_MAX` back to 0; timers must still fire at the correct time.

The shared types `timer_sched_t`, `timer_slot_t`, `timer_cb_t`, and constant `TS_MAX_TIMERS` are provided. Implement:

```c
void ts_init(timer_sched_t *ts);
int  ts_start(timer_sched_t *ts, uint32_t now, uint32_t delay_ms,
              bool periodic, timer_cb_t cb, void *ctx);
bool ts_cancel(timer_sched_t *ts, int timer_id);
void ts_poll(timer_sched_t *ts, uint32_t now);
```

- **`ts_start`** — start a timer in the lowest-numbered inactive slot, due `delay_ms` after `now`; return the slot index, or `-1` if every slot is in use
- **`ts_poll`** — fire the callback of every active timer that is due at `now`. One-shot timers deactivate after firing. A periodic timer's next deadline is one period after its previous deadline, not after `now`, so it never drifts
- **`ts_cancel`** — deactivate the slot; return `false` if `timer_id` is out of range or already inactive

Delays are less than `2^31` ticks, and polling never misses a deadline by `2^31` ticks or more. Periodic timers require a nonzero period. This simplified exercise does not support callbacks that mutate the scheduler.

## Examples

**Example 1** — one-shot timer
```
ts_init(&ts);
int id = ts_start(&ts, 1000, 50, false, my_cb, NULL);
ts_poll(&ts, 1049);  // not yet
ts_poll(&ts, 1050);  // fires my_cb, timer deactivated
ts_poll(&ts, 1100);  // does not fire again
```

**Example 2** — periodic timer
```
id = ts_start(&ts, 0, 100, true, my_cb, NULL);
ts_poll(&ts, 100);   // fires, deadline → 200
ts_poll(&ts, 200);   // fires again, deadline → 300
ts_cancel(&ts, id);
ts_poll(&ts, 300);   // does not fire
```

**Example 3** — wraparound
```
ts_start(&ts, 0xFFFFFFF0, 32, false, my_cb, NULL);
// deadline = 0xFFFFFFF0 + 32 = 0x00000010 (wraps)
ts_poll(&ts, 0x00000010);  // fires correctly
```

## Constraints

- `TS_MAX_TIMERS` slots, indexed 0 to `TS_MAX_TIMERS-1`
- `ts_cancel(-1)` or out-of-range id returns `false`

**Optimal complexity:** O(T) per poll, where T = TS_MAX_TIMERS
