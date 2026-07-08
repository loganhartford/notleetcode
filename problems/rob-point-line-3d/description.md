# Point to Line Distance 3D

Implement two distance functions in 3D space.

```cpp
struct Vec3 { double x, y, z; };

// Distance from point P to the infinite line through A and B
double pointToLineDistance(Vec3 P, Vec3 A, Vec3 B);

// Distance from point P to the finite segment AB
double pointToSegmentDistance(Vec3 P, Vec3 A, Vec3 B);
```

**Line distance:** Use the cross product formula. Let `d = B - A` (direction), `w = P - A`. Then:
```
dist = |w × d| / |d|
```

**Segment distance:** Project P onto the line. If the projection parameter `t = dot(w, d) / dot(d, d)` is in `[0, 1]`, the closest point is on the segment. Otherwise, clamp to the nearest endpoint.

## Constraints

- A ≠ B (line is well-defined)
- If A == B for the segment case, return distance from P to A
- Coordinates in range [-1000, 1000]

**Optimal complexity:** O(1) — direct formula evaluation.
