# Integer EMA Filter

Implement an exponential moving average (EMA) low-pass filter for `int16_t` samples on a core with no FPU and no hardware divider. The smoothing factor is `alpha = 1 / 2^k` for an integer `k` from 1 to 8. Your filter approximates this ideal, real-valued filter, where `x_n` is the n-th sample passed to `ema_update` after `ema_init`:

```
y_0 = initial
y_n = y_(n-1) + (x_n - y_(n-1)) / 2^k        for n >= 1
```

All filter state lives in the caller's `ema_t`. The struct layout is fixed; how you use `state` and `k` is up to you.

```c
typedef struct {
    int32_t  state;
    uint8_t  k;
} ema_t;

void    ema_init(ema_t *f, uint8_t k, int16_t initial);
int16_t ema_update(ema_t *f, int16_t x);
```

- **`ema_init(f, k, initial)`** makes `f` a filter with smoothing factor `1 / 2^k` whose output starts at `initial`. `f` may contain garbage bytes, or be a filter that was already in use.
- **`ema_update(f, x)`** feeds one sample and returns the filter's new output, an integer that stands for `y_n`. The outputs must meet both requirements:
  - **Accuracy:** every output is within 1 of the ideal value: `|output_n - y_n| < 1`. When `y_n` is not an integer, either neighbouring integer is accepted; when it is an integer, the output must equal it.
  - **No steady-state error:** when the same value `x` is fed on consecutive updates, the output is exactly `x` on the `16 * 2^k`-th of those updates and on every one after it, for as long as `x` keeps being fed. This holds whatever the filter's history before the run of `x` began, for rising and falling steps, and for every `int16_t` value of `x`.

Python: implement `ema_init(f, k, initial)` and `ema_update(f, x) -> int` on an `Ema` object (defined in the starter) with `state` and `k` attributes, with the same contract on ints in `[-32768, 32767]`.

### Example 1
```
ema_init(&f, 4, 0);         alpha = 1/16
ema_update(&f, 1000)        ideal y_1 = 62.5          returns 62 or 63
ema_update(&f, 1000)        ideal y_2 = 121.09375     returns 121 or 122
ema_update(&f, 1000)        ideal y_3 = 176.0254...   returns 176 or 177
...
ema_update(&f, 1000)        the 256th update and every one after it return exactly 1000
```
The ideal `y_n` never reaches 1000, but the output must.

### Example 2
```
ema_init(&f, 2, 100);       alpha = 1/4
ema_update(&f, -100)        ideal y_1 = 50            returns 50
ema_update(&f, -100)        ideal y_2 = 12.5          returns 12 or 13
ema_update(&f, -100)        ideal y_3 = -15.625       returns -16 or -15
...
ema_update(&f, -100)        the 64th update and every one after it return exactly -100
```

### Example 3
```
ema_init(&f, 8, -5);
ema_update(&f, -5)          ideal y_n = -5            returns -5, on every update
```

### Constraints
- `1 <= k <= 8`
- `initial` and every `x` can be any `int16_t` value (Python: any int in `[-32768, 32767]`)
- No floating point, no division or modulo, no 64-bit types (Python: plain ints only, no floats, `/`, `//` or `%`; shifts are fine)
- No static or global state: every filter is independent, and several may be updated interleaved
- No undefined behavior for any input; you may assume `>>` on a negative signed value is an arithmetic shift (rounds toward negative infinity)

**Optimal complexity:** O(1) time and state per sample
