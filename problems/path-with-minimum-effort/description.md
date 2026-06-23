# Path With Minimum Effort

You are a hiker on a grid `heights` of size `rows x columns`, where `heights[r][c]` is the height of cell `(r, c)`. You start at the top-left cell `(0, 0)` and want to reach the bottom-right cell `(rows-1, columns-1)`. You can move **up, down, left, or right**.

A route's **effort** is the **maximum absolute difference** in heights between two consecutive cells along the route.

Return the **minimum effort** required to travel from top-left to bottom-right.

### Example 1
```
Input:  heights = [[1,2,2],[3,8,2],[5,3,5]]
Output: 2        # the route 1→2→2→2→5 has max diff 2 (better than going through 8)
```

### Example 2
```
Input:  heights = [[1,2,3],[3,8,4],[5,3,5]]
Output: 1
```

### Example 3
```
Input:  heights = [[1,2,1,1,1],[1,2,1,2,1],[1,2,1,2,1],[1,2,1,2,1],[1,1,1,2,1]]
Output: 0
```

### Constraints
- `rows == len(heights)`, `columns == len(heights[0])`
- `1 <= rows, columns <= 100`
- `1 <= heights[r][c] <= 10^6`

Dijkstra where the path "cost" is the running maximum edge difference, not a sum.
