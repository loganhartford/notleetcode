# Implement A* on an Occupancy Grid

Implement **A\*** shortest-path search on a binary occupancy grid.

```python
def astar(grid: List[List[int]], start: List[int], goal: List[int]) -> int
```

- `grid[r][c]` is `0` for a **free** cell and `1` for an **obstacle**.
- You move **4-directionally**; each step costs `1`.
- `start` and `goal` are `[row, col]` pairs, both guaranteed to be free.
- Use the **Manhattan distance** as the heuristic (admissible for 4-connected,
  unit-cost grids).

Return the **length of the shortest path** (number of steps) from `start` to
`goal`, or `-1` if the goal is unreachable.

> With unit costs and an admissible heuristic, A\* returns the same optimal path
> length as Dijkstra/BFS — but expands fewer nodes. That's the point of the
> heuristic; the returned value is what we check here.

### Example 1
```
grid = [[0, 0, 0],
        [1, 1, 0],
        [0, 0, 0]]
start = [0, 0], goal = [2, 0]
-> 6
```

### Example 2
```
grid = [[0, 0],
        [0, 0]]
start = [0, 0], goal = [1, 1]
-> 2
```

### Example 3
```
grid = [[0, 1],
        [1, 0]]
start = [0, 0], goal = [1, 1]
-> -1     # diagonal moves are not allowed; the corner is sealed off
```

### Constraints
- `1 <= rows, cols <= 200`
- `start` and `goal` are free cells.
- This is `[author]`-defined: there is no LeetCode equivalent.
