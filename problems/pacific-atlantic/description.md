# Pacific Atlantic Water Flow

There is an `m x n` rectangular island that borders both the **Pacific** Ocean (top and left edges) and the **Atlantic** Ocean (bottom and right edges). The island is partitioned into a grid of square cells; `heights[r][c]` is the height above sea level of cell `(r, c)`.

Water can flow from a cell to a **4-directionally adjacent** cell whose height is **less than or equal to** the current cell's height. Water can also flow into either ocean from any cell on its bordering edges.

Return a list of grid coordinates `[r, c]` from which rain water can flow to **both** the Pacific and Atlantic oceans.

### Example
```
Input: heights = [
  [1,2,2,3,5],
  [3,2,3,4,4],
  [2,4,5,3,1],
  [6,7,1,4,5],
  [5,1,1,2,4]
]
Output (in any order): [[0,4],[1,3],[1,4],[2,2],[3,0],[3,1],[4,0]]
```

### Constraints
- `m == len(heights)`, `n == len(heights[0])`
- `1 <= m, n <= 200`
- `0 <= heights[r][c] <= 10^5`

The checker is order-independent.
