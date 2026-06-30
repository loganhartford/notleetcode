# Piecewise Linear Interpolation

Implement piecewise linear interpolation over a sorted calibration table. This is the standard technique for converting raw ADC readings to engineering units in embedded systems.

The `point_t` struct is provided. Implement:

```c
bool interp_i32(const point_t *pts, size_t n, int32_t x, int32_t *y_out);
```

- `pts` is sorted ascending by `.x`
- Returns `false` if `x < pts[0].x` (below range) or `x > pts[n-1].x` (above range)
- If `x` exactly matches a point's `.x`, return that point's `.y`
- Otherwise, find the surrounding segment and linearly interpolate:
  `y = y0 + (int64_t)(y1 - y0) * (x - x0) / (x1 - x0)`

## Examples

**Example 1** — exact match
```
pts = [{0,0}, {100,200}, {200,100}]
interp_i32(pts, 3, 0,   &y) → true, y=0
interp_i32(pts, 3, 100, &y) → true, y=200
interp_i32(pts, 3, 200, &y) → true, y=100
```

**Example 2** — interpolation
```
pts = [{0,0}, {100,100}]
interp_i32(pts, 2, 50, &y) → true, y=50
interp_i32(pts, 2, 25, &y) → true, y=25
```

**Example 3** — out of range
```
pts = [{10,100}, {20,200}]
interp_i32(pts, 2, 9,  &y) → false  (below)
interp_i32(pts, 2, 21, &y) → false  (above)
```

## Constraints

- Table has `n >= 2` points (you may assume valid input)
- Use `int64_t` for the slope calculation to avoid overflow: `(int64_t)(y1-y0)*(x-x0)/(x1-x0)`
- Integer division truncates toward zero (consistent with C99)
- `n == 0` or `n == 1` may be treated as below-range (return false)

**Optimal complexity:** O(n) linear scan (or O(log n) binary search — both accepted)
