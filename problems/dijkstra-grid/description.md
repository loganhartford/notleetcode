# Implement Dijkstra on a Grid

Implement weighted shortest-path search on a 2D grid using a **min-heap (Dijkstra)**.

```python
def gridDijkstra(grid: List[List[int]], start: List[int], goal: List[int]) -> int
```

- `grid[r][c]` is the **cost to enter** that cell. A value of `-1` marks an
  **impassable obstacle**.
- You move **4-directionally** (up/down/left/right).
- The cost of a path is the **sum of the entry costs of every cell you step onto**,
  **excluding** the start cell (you don't pay to stand on start; you pay to enter
  the goal).
- `start` and `goal` are `[row, col]` pairs.

Return the **minimum total cost** from `start` to `goal`, or `-1` if the goal is
unreachable (or if `start`/`goal` is itself an obstacle).

### Example 1
```
grid = [[1, 1, 1],
        [1, 9, 1],
        [1, 1, 1]]
start = [0, 0], goal = [2, 2]
-> 4      # skirt around the costly center: enter four cells of cost 1
```

### Example 2
```
grid = [[1, -1, 1],
        [1, -1, 1],
        [1,  1, 1]]
start = [0, 0], goal = [0, 2]
-> 6      # forced down and around the wall
```

### Example 3
```
grid = [[0, -1],
        [-1, 0]]
start = [0, 0], goal = [1, 1]
-> -1     # blocked
```

### Constraints
- `1 <= rows, cols <= 200`
- Entry costs are `0..1000`, or `-1` for an obstacle.
- This is `[author]`-defined: there is no LeetCode equivalent.
