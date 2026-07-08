# Polygon Area (Shoelace)

Given the vertices of a simple polygon in order (either clockwise or counterclockwise), compute its area using the **Shoelace formula**.

```cpp
// vertices in order (clockwise or counterclockwise)
// returns area (always positive)
double polygonArea(vector<pair<double,double>> vertices);
```

The Shoelace formula:

```
area = |Σ (x_i * y_{i+1} - x_{i+1} * y_i)| / 2
```

where indices wrap around (vertex[n] = vertex[0]).

## Examples

```
polygonArea([(0,0),(4,0),(4,3),(0,3)]) → 12.0   (rectangle 4×3)
polygonArea([(0,0),(1,0),(0,1)])        → 0.5    (right triangle)
polygonArea([(0,0),(0,1),(1,0)])        → 0.5    (CCW order, same area)
```

## Constraints

- `3 <= vertices.size() <= 10^4`
- Polygon is simple (non-self-intersecting)
- Both clockwise and counterclockwise orderings are accepted

**Optimal complexity:** O(n) — single pass over all edges.
