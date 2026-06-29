# 1D Kalman Filter (predict / update)

Implement a scalar (1D) **Kalman filter** with the classic predict/update cycle.
A constant model is assumed (the state is a single scalar, e.g. position).

```python
class KalmanFilter1D:
    def __init__(self, x, P, Q, R):
        # x: state estimate, P: estimate variance,
        # Q: process noise variance, R: measurement noise variance
    def predict(self, u=0.0) -> float:
        # x = x + u
        # P = P + Q
        # return x
    def update(self, z) -> float:
        # K = P / (P + R)        (Kalman gain)
        # x = x + K * (z - x)
        # P = (1 - K) * P
        # return x
```

Use **exactly** these equations so results are reproducible:
- **predict(u):** `x ← x + u`, `P ← P + Q`, return `x`.
- **update(z):** `K = P / (P + R)`, `x ← x + K·(z − x)`, `P ← (1 − K)·P`, return `x`.

### Example
```
KalmanFilter1D(x=0, P=1, Q=0.0, R=1.0)
update(z=2)  -> 1.0     # K = 1/(1+1) = 0.5; x = 0 + 0.5*(2-0) = 1.0
update(z=2)  -> 1.5     # P shrank to 0.5; K = 0.5/1.5 = 1/3; x = 1 + (1/3)(2-1)
```

### Notes
- `[author]`-defined robotics practice; small float tolerance allowed.
- (Stretch, not tested: extend the same predict/update structure to a 2D pose.)

**Optimal complexity:** O(1) per predict / update step
