# Walls and Gates

You are given an `m x n` grid `rooms` initialized with these three possible values:
- `-1` — a wall or obstacle.
- `0` — a gate.
- `2147483647` (i.e. `2^31 - 1`, "INF") — an empty room.

Fill each empty room with the distance to its **nearest gate**. If it is impossible to reach a gate, the value stays `INF`.

Modify `rooms` **in place** and return `None`. Movement is 4-directional.

### Example
```
Input:  rooms = [
  [INF, -1,  0, INF],
  [INF, INF, INF, -1],
  [INF, -1, INF, -1],
  [  0, -1, INF, INF]
]
After:  [
  [3, -1, 0, 1],
  [2, 2, 1, -1],
  [1, -1, 2, -1],
  [0, -1, 3, 4]
]
```

### Constraints
- `m == len(rooms)`, `n == len(rooms[0])`
- `1 <= m, n <= 250`
- `rooms[i][j]` is `-1`, `0`, or `2147483647`.

**Multi-source BFS** from all gates at once gives O(m·n).

**Optimal complexity:** O(m·n)  — multi-source BFS from all gates
