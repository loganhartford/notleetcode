# ADC 10-Sample Moving Average Filter

Raw ADC readings are noisy. A cheap, effective way to smooth them in firmware is a
fixed-length **moving average**: keep the last `N` samples and report their mean each time a
new one arrives. You are filtering 8-bit ADC samples with a window of `N = 10`.

Implement the update function so it can be called once per new sample:

```c
filteredValue = movingAverage(&filter, newSample);
```

Each call ingests one sample and returns the current filtered value. The filter state lives in
a caller-owned struct so many independent channels can share one function:

```c
#define CAPACITY 10

typedef struct {
    uint8_t  buf[CAPACITY];   /* the last CAPACITY raw samples          */
    uint8_t  head;            /* next slot to overwrite (oldest sample) */
    uint32_t sum;             /* running sum of the buffer contents     */
} avgBuffer_t;

float movingAverage(avgBuffer_t *filter, uint8_t newSample);
```

### Rules

- The struct is **zero-initialized** before the first call (`avgBuffer_t f = {0};`), so the
  window starts as ten zeros.
- The divisor is always `CAPACITY` (10) — *not* the number of real samples seen so far. The
  filter "warms up" over its first 10 samples as the initial zeros are pushed out. This is the
  intended behavior for a fixed-window ADC filter.
- Each call must run in **O(1)**: maintain a running `sum`, subtracting the sample being
  evicted and adding the new one — do not re-sum the buffer every call.

### Example

Feeding the samples `0, 1, 2, …, 14` into a freshly zeroed filter:

```
newSample:  0    1    2    3    4    5    6    7    8    9   10   11   12   13   14
returns  : 0.0  0.1  0.3  0.6  1.0  1.5  2.1  2.8  3.6  4.5  5.5  6.5  7.5  8.5  9.5
```

For the first 10 samples the sum grows (0+1+…+k)/10. On the 11th sample (`10`) the window is
full: the oldest value (the initial `0` at slot 0) is evicted, so the average becomes
`(0+1+…+9 - 0 + 10)/10 = 5.5`, then tracks the true 10-sample mean thereafter.

### Constraints

- `0 <= newSample <= 255` (8-bit ADC).
- No dynamic allocation; the running sum fits in `uint32_t` (max `10 * 255 = 2550`).

**Optimal complexity:** O(1) time and O(1) extra space per sample.
