# PID Controller (with anti-windup, fixed timestep)

Implement a discrete **PID controller** with a fixed timestep and integral
anti-windup. Implement the `PID` class:

```python
class PID:
    def __init__(self, kp, ki, kd, dt, out_min=None, out_max=None): ...
    def update(self, setpoint, measurement) -> float: ...
    def reset(self) -> None: ...
```

Each `update` performs **exactly** this (positional form):

```
error      = setpoint - measurement
integral  += error * dt
derivative = (error - prev_error) / dt
output     = kp*error + ki*integral + kd*derivative
```

**Output clamping + anti-windup:** if `out_max`/`out_min` are given and `output`
exceeds the limit, clamp the output to the limit **and undo this step's integral
accumulation** (`integral -= error * dt`) so the integrator doesn't wind up while
saturated. Then store `prev_error = error` and return the (possibly clamped)
output.

`prev_error` and `integral` start at `0`. `reset()` returns them to `0`.

### Example
```
PID(kp=1.0, ki=0.5, kd=0.1, dt=1.0)
update(setpoint=10, measurement=0)  -> 16.0
   error=10, integral=10, derivative=10  -> 1*10 + 0.5*10 + 0.1*10 = 16.0
```

### Notes
- Use the exact formula above so results are reproducible.
- `[author]`-defined robotics practice; the checker allows a tiny float
  tolerance.
