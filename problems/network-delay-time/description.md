# Network Delay Time

You are given a network of `n` nodes, labeled `1` to `n`. You are also given `times`, a list of travel times as directed edges `times[i] = [u, v, w]`, where `u` is the source node, `v` is the target node, and `w` is the time it takes for a signal to travel from `u` to `v`.

You send a signal from a given node `k`. Return the **minimum time** for **all** `n` nodes to receive the signal. If it is impossible for all nodes to receive the signal, return `-1`.

### Example 1
```
Input:  times = [[2,1,1],[2,3,1],[3,4,1]], n = 4, k = 2
Output: 2
```

### Example 2
```
Input:  times = [[1,2,1]], n = 2, k = 1
Output: 1
```

### Example 3
```
Input:  times = [[1,2,1]], n = 2, k = 2
Output: -1      # node 1 never receives the signal
```

### Constraints
- `1 <= k <= n <= 100`
- `1 <= len(times) <= 6000`
- `0 <= w <= 100`

Classic single-source shortest path → **Dijkstra** with a min-heap.

**Optimal complexity:** O((V + E) log V)  — Dijkstra with binary heap
