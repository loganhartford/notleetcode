# Power Monitor: 5-Second Moving Average

An RTOS firmware reads two sensors and reports electrical power:

| Task          | Rate  | Produces                              |
|---------------|-------|---------------------------------------|
| `voltageTask` | 1 Hz  | one voltage reading per second        |
| `currentTask` | 10 Hz | ten current readings per second       |
| `powerTask`   | 1 Hz  | `P = V · I`, averaged over a 5 s window|

Each producer pushes `SensorData` onto a FreeRTOS queue; `powerTask` drains them and,
once per second, computes power and updates a **5-second moving average**.

You implement the math `powerTask` runs each second. The two sample rates don't match, so
for each 1-second tick you get **one voltage** and the **batch of current samples** that
arrived during that second (up to ten). Power for the second uses the mean current:

```
P(second) = voltage * mean(current_samples)
```

and the reported value is the average of `P` over the **last 5 seconds** (the last 5
one-second power values — fewer while the window is still filling).

### What you implement

```c
typedef struct {
    float window[5];   /* last 5 one-second power values */
    int   count;       /* how many are valid so far (0..5) */
    int   head;        /* next write position             */
} PowerMonitor;

void  power_monitor_init(PowerMonitor *m);

/* Call once per second. `voltage` is this second's reading; current_samples[0..n)
   are the current readings collected this second (n up to 10, possibly 0).
   Return the moving average of power over the last 5 seconds. */
float power_monitor_update(PowerMonitor *m, float voltage,
                           const float *current_samples, int n);
```

### Rules

- Power for the second is `voltage * mean(current_samples)`. If `n == 0`, treat the mean
  current as `0`, so this second contributes `0` power.
- The window holds at most 5 one-second power values; the newest evicts the oldest.
- Return the mean of the values currently in the window. While fewer than 5 have arrived,
  average over just those.

### Example
```
init(m)
update(m, v=2, current=[2])       -> P=4   window=[4]              avg = 4.0
update(m, v=3, current=[2])       -> P=6   window=[4,6]            avg = 5.0
update(m, v=1, current=[2])       -> P=2   window=[4,6,2]          avg = 4.0
update(m, v=5, current=[2])       -> P=10  window=[4,6,2,10]       avg = 5.5
update(m, v=4, current=[2])       -> P=8   window=[4,6,2,10,8]     avg = 6.0
update(m, v=10, current=[2])      -> P=20  window=[6,2,10,8,20]    avg = 9.2   (4 evicted)
```

### Design discussion (worth thinking about, not graded)

- **Queue depths.** The reference sets the voltage queue to 5 and the current queue to 50.
  Why the asymmetry? The 10 Hz stream needs far more slack so a late `powerTask` run
  doesn't drop samples.
- **Task priorities.** `powerTask` runs highest, then `currentTask`, then `voltageTask` —
  the consumer must keep up with the fast producer or the queue backs up.
- **Draining the queue.** Each tick `powerTask` should pull *all* queued current samples,
  not just one, so it never falls behind the 10 Hz producer.

### Constraints
- `0 <= n <= 10`; voltage and current are finite floats.
- All arithmetic in `float`.

**Optimal complexity:** O(n) per update to average the current, O(1) amortized for the
5-slot power window.
