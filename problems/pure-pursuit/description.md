# Waypoint Follower (Pure Pursuit)

Implement the steering law of a **pure-pursuit** path follower. Given the robot's
pose, a path (list of waypoints), and a lookahead distance, compute the steering
**curvature** that drives the robot toward the lookahead point.

```python
def pure_pursuit(pose, path, lookahead) -> float
```

Algorithm:
1. `pose = (x, y, theta)`. Pick the **lookahead point**: the **first** waypoint in
   `path` whose Euclidean distance from `(x, y)` is `>= lookahead`. If no waypoint
   is that far, use the **last** waypoint.
2. Transform that point into the robot frame:
   `dx = lx - x`, `dy = ly - y`,
   `x_r =  cos(theta)·dx + sin(theta)·dy`,
   `y_r = -sin(theta)·dx + cos(theta)·dy`.
3. Return the curvature `kappa = 2·y_r / lookahead²`.

A positive curvature means steer left; `0` means go straight.

### Example
```
pure_pursuit((0, 0, 0), [(1, 0), (2, 0)], 1.0)  -> 0.0    # target straight ahead
pure_pursuit((0, 0, 0), [(2, 2)], 2.0)          -> 1.0    # target up-left: y_r=2, k=2*2/4
```

### Notes
- `[author]`-defined robotics practice; small float tolerance allowed.
