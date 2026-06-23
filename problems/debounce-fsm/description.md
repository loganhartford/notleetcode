# Debounce State Machine (FSM, no delay())

Implement a button **debouncer** as a state machine that you tick on every sample
(no blocking `delay()`). The output only changes once the raw input has held a new
value for `threshold` consecutive samples.

```c
typedef struct {
    int stable;      // current debounced output
    int candidate;   // value we're currently counting
    int count;       // how many consecutive candidate samples
    int threshold;   // samples required to accept a change
} debounce_t;

void debounce_init(debounce_t* db, int initial, int threshold);
int  debounce_update(debounce_t* db, int raw);   // returns the stable output
```

Logic for each `debounce_update(raw)`:
- If `raw == stable`: the candidate streak resets (`count = 0`).
- Else (raw differs from the stable output):
  - if `raw == candidate`, increment `count`; otherwise start a fresh streak
    (`candidate = raw`, `count = 1`);
  - then, if `count >= threshold`, accept the change (`stable = raw`, `count = 0`).
- Always return the current `stable`.

(So `threshold = 1` flips on the very first differing sample.)

### Example (threshold = 3, initial = 0)
```
raw:    0 1 1 0 1 1 1 1 0
stable: 0 0 0 0 0 0 1 1 1
```
The lone `1`s and the bounce back to `0` are rejected; only after three steady
`1`s does the output flip.

### Constraints
- Inputs are `0`/`1`, `threshold >= 1`.
- `[author]`-defined firmware practice.
