# RRT on a 2D Map with Obstacles (Stretch)

Implement a basic **Rapidly-exploring Random Tree (RRT)** planner on a 2D
occupancy grid.

```python
def rrt(grid, start, goal, max_iters=4000, step=1.0, goal_tol=1.5, seed=0):
    # returns a path [(x, y), ...] from start to near goal, or None if not found
```

- `grid[r][c]` is `1` for an obstacle, `0` for free. The continuous workspace is
  `[0, cols] x [0, rows]`; a point `(x, y)` is **free** iff it is in bounds and
  `grid[int(y)][int(x)] == 0`.
- `start` and `goal` are `(x, y)` float tuples in free space.
- Grow a tree from `start`: sample a random free-space point (bias toward `goal`
  occasionally), find the nearest tree node, and **extend** from it toward the
  sample by `step`. Reject the new node if it (or the segment to it) hits an
  obstacle.
- When a node lands within `goal_tol` of `goal`, stop and return the path from
  `start` to that node by walking parent pointers. If `max_iters` is exhausted,
  return `None`.
- Seed your RNG with `seed` for reproducibility.

### What the checker verifies
Because RRT is randomized, the checker does **not** expect a specific path. For a
solvable map it requires a **valid** path: it starts at `start`, ends within
`goal_tol` of `goal`, every vertex is in free space, and consecutive vertices are
no more than `step` apart. For an unsolvable map it requires `None`.

### Notes
- `[author]`-defined robotics practice; this is the stretch problem of the set.
