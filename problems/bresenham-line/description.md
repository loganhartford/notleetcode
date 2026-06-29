# Bresenham Line / Line-of-Sight on a Grid

Implement integer **Bresenham line rasterization** and use it for a grid
line-of-sight check.

```python
def bresenham(x0, y0, x1, y1):       # list of (x, y) integer cells, start..end inclusive
def line_of_sight(grid, start, end): # True if the straight line hits no obstacle
```

- **bresenham(x0, y0, x1, y1)** returns the list of grid cells the line passes
  through, **in order from start to end, inclusive of both endpoints**. It must
  work in all directions (all 8 octants) using only integer arithmetic.
- **line_of_sight(grid, start, end)**: `grid[y][x]` is `1` for an obstacle, `0`
  for free. `start` and `end` are `(x, y)` tuples. Return `True` iff **no cell**
  along the Bresenham line between them is an obstacle (endpoints included).

### Example
```
bresenham(0, 0, 3, 1) -> [(0,0), (1,0), (2,1), (3,1)]
bresenham(2, 2, 2, 0) -> [(2,2), (2,1), (2,0)]

grid = [[0,0,0],
        [0,1,0],
        [0,0,0]]
line_of_sight(grid, (0,0), (2,2)) -> False   # passes through the obstacle at (1,1)
line_of_sight(grid, (0,0), (2,0)) -> True
```

### Constraints
- Coordinates are non-negative integers within the grid.
- `[author]`-defined robotics practice.

**Optimal complexity:** O(max(|dx|, |dy|))  — one step per output pixel
