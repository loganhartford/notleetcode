# Rotting Oranges

You are given an `m x n` grid where each cell can be:
- `0` — an empty cell,
- `1` — a fresh orange,
- `2` — a rotten orange.

Every minute, any fresh orange that is **4-directionally adjacent** to a rotten orange becomes rotten.

Return the **minimum number of minutes** that must elapse until no cell has a fresh orange. If this is impossible, return `-1`.

### Example 1
```
Input:  grid = [[2,1,1],[1,1,0],[0,1,1]]
Output: 4
```

### Example 2
```
Input:  grid = [[2,1,1],[0,1,1],[1,0,1]]
Output: -1      # the bottom-left orange can never rot
```

### Example 3
```
Input:  grid = [[0,2]]
Output: 0       # no fresh oranges at the start
```

### Constraints
- `m == len(grid)`, `n == len(grid[0])`
- `1 <= m, n <= 10`
- `grid[i][j]` is `0`, `1`, or `2`.

This is a **multi-source BFS** — start from every rotten orange at minute 0.

**Optimal complexity:** O(m·n)  — multi-source BFS from all initially rotten cells
