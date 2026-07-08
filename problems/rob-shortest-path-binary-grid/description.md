# Shortest Path in Binary Occupancy Grid

Given an `n x m` binary grid where `0` means free and `1` means blocked, find the shortest path from the top-left cell `(0,0)` to the bottom-right cell `(n-1, m-1)`.

Movement is **8-directional** (including diagonals). Return the length of the shortest path (counting both endpoints), or `-1` if no path exists.

```cpp
int shortestPath(vector<vector<int>>& grid); // 0=free, 1=blocked
```

## Examples

```
grid = [[0,0,0],
        [1,1,0],
        [1,1,0]]
→ 4  (path: (0,0)→(0,1)→(0,2)→(1,2)→(2,2))

grid = [[1,0,0],
        [1,1,0],
        [1,1,0]]
→ -1  (start is blocked)
```

## Constraints

- `1 <= n, m <= 100`
- `grid[i][j]` is `0` or `1`
- Start at `(0,0)`, goal at `(n-1, m-1)`

**Optimal complexity:** O(n*m) — BFS explores each cell at most once.
