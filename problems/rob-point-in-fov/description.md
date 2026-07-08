# Point in Field of View

Determine whether a target point lies within a sensor's field of view (FOV).

```cpp
struct Vec2 { double x, y; };

// Returns true if target is within FOV:
//   - within max_range distance from observer
//   - within half_angle_rad of the heading direction
bool pointInFOV(Vec2 observer, Vec2 heading, double half_angle_rad,
                double max_range, Vec2 target);
```

Use the **dot product** test to avoid `acos`: the angle between two unit vectors `u` and `v` satisfies `cos(θ) = dot(u, v)`. The target is within the FOV angle if:

```
cos(angle_to_target) >= cos(half_angle_rad)
```

A target at the observer's exact position (distance = 0) is considered inside the FOV.

## Constraints

- `heading` need not be normalized (normalize internally)
- `half_angle_rad` is in `(0, π)`
- `max_range > 0`
- Use `<=` for range and `>=` for angle (boundary is included)

**Optimal complexity:** O(1)
