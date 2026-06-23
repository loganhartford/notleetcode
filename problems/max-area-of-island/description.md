# Max Area of Island

You are given an `m x n` binary matrix `grid`. An **island** is a group of `1`s (land) connected **4-directionally** (horizontal or vertical). You may assume all four edges of the grid are surrounded by water.

The **area** of an island is the number of cells with value `1` in it.

Return the **maximum area** of an island in `grid`. If there is no island, return `0`.

### Example 1
```
Input: grid = [
  [0,0,1,0,0,0,0,1,0,0,0,0,0],
  [0,0,0,0,0,0,0,1,1,1,0,0,0],
  [0,1,1,0,1,0,0,0,0,0,0,0,0],
  [0,1,0,0,1,1,0,0,1,0,1,0,0],
  [0,1,0,0,1,1,0,0,1,1,1,0,0],
  [0,0,0,0,0,0,0,0,0,0,1,0,0],
  [0,0,0,0,0,0,0,1,1,1,0,0,0],
  [0,0,0,0,0,0,0,1,1,0,0,0,0]
]
Output: 6
```

### Example 2
```
Input:  grid = [[0,0,0,0,0,0,0,0]]
Output: 0
```

### Constraints
- `m == len(grid)`, `n == len(grid[0])`
- `1 <= m, n <= 50`
- `grid[i][j]` is `0` or `1`.
