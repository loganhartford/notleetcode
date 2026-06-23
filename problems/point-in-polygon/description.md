# Point-in-Polygon Test

Given a polygon as a list of `(x, y)` vertices (in order, either winding) and a
query point, determine whether the point is **inside** the polygon.

```python
def point_in_polygon(polygon, point) -> bool
```

Use the **ray casting** (even-odd) rule: shoot a ray from the point and count how
many polygon edges it crosses; an odd count means inside.

### Example
```
square = [(0,0), (4,0), (4,4), (0,4)]
point_in_polygon(square, (2, 2)) -> True
point_in_polygon(square, (5, 2)) -> False

concave = [(0,0), (4,0), (4,4), (2,2), (0,4)]
point_in_polygon(concave, (2, 3)) -> False   # inside the notch
point_in_polygon(concave, (1, 1)) -> True
```

### Constraints
- `3 <= len(polygon) <= 1000`
- The polygon is simple (non-self-intersecting).
- Query points are **strictly interior or strictly exterior** (the checker avoids
  exact boundary/vertex cases, where the convention is ambiguous).
- `[author]`-defined robotics practice.
