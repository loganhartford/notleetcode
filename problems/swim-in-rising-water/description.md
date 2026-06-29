# Swim in Rising Water

You are given an `n x n` integer matrix `grid` where each value `grid[r][c]` represents the elevation at that point.

It starts raining. At time `t`, the water level is `t` everywhere. You can swim from a cell to a **4-directionally adjacent** cell if and only if the elevation of **both** cells is at most `t`. You can swim infinitely fast within a time unit.

Starting at the top-left cell `(0, 0)`, return the **least time** until you can reach the bottom-right cell `(n-1, n-1)`.

### Example 1
```
Input:  grid = [[0,2],[1,3]]
Output: 3
```

### Example 2
```
Input:  grid = [[0,1,2,3,4],[24,23,22,21,5],[12,13,14,15,16],[11,17,18,19,20],[10,9,8,7,6]]
Output: 16
```

### Constraints
- `n == len(grid)`
- `1 <= n <= 50`
- `0 <= grid[r][c] < n^2`
- Each value in `grid` is unique.

Dijkstra/heap where a path's cost is the **maximum elevation** seen along it.

**Optimal complexity:** O(n² · log n)  — Dijkstra minimising maximum elevation seen
