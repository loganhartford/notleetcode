# Swept AABB Collision

Detect continuous collision between two axis-aligned bounding boxes (AABBs) moving at constant velocity over a time interval `[0, T]`.

```cpp
struct AABB {
    double min_x, min_y;  // lower-left corner
    double max_x, max_y;  // upper-right corner
    double vx, vy;        // velocity (units per time)
};

// Returns the earliest collision time in [0, T], or -1.0 if no collision
// Touching at boundary counts as collision (use <= comparisons)
double sweptAABBCollision(AABB a, AABB b, double T);
```

**Algorithm (separating axis):** Compute relative velocity `rvx = b.vx - a.vx`, `rvy = b.vy - a.vy`. For each axis, compute entry and exit times when the gap closes and re-opens. Global entry = max of axis entries, exit = min of axis exits. Collision if `entry <= exit` and `entry <= T` and `exit >= 0`.

- If no relative velocity on an axis: if already overlapping on that axis, entry = -∞, exit = +∞. Otherwise no collision.

## Constraints

- Boxes do not rotate
- Touching at a boundary (entry time = exit time) counts as a collision

**Optimal complexity:** O(1) — two-axis separating axis test.
