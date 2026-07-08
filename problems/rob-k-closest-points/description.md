# K Closest Points to Origin

Given a list of 2D points and an integer `k`, return the `k` points closest to the origin `(0, 0)` by Euclidean distance.

```cpp
vector<vector<int>> kClosest(vector<vector<int>> points, int k);
```

The order of points in the result does not matter. When distances are equal, any valid selection is accepted.

## Example

```
kClosest([[1,3],[-2,2]], 1)        → [[-2,2]]    (dist²=8 < dist²=10)
kClosest([[3,3],[5,-1],[-2,4]], 2) → [[3,3],[-2,4]]
```

## Constraints

- `1 <= k <= points.length <= 10^4`
- `-10^4 <= x, y <= 10^4`
- Use squared distance to avoid floating point: `dist² = x² + y²`

**Optimal complexity:** O(n log k) — max-heap of size k, or O(n) with nth_element / quickselect.
