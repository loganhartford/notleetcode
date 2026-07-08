# Module Dependency Order

Given `n` modules labeled `0` to `n-1` and a list of dependencies, return a valid build order. A dependency `[a, b]` means module `a` depends on module `b` (so `b` must be built before `a`).

```cpp
// n: number of modules
// deps[i] = {a, b} means a depends on b (b before a)
// Returns valid build order, or empty vector if a cycle exists
vector<int> buildOrder(int n, vector<vector<int>> deps);
```

Any valid topological ordering is accepted. Return an empty vector if the dependency graph contains a cycle.

## Example

```
n=4, deps=[[1,0],[2,0],[3,1],[3,2]]
One valid order: [0, 1, 2, 3]  (0 first, 3 last)
```

## Constraints

- `1 <= n <= 2000`
- `0 <= deps.length <= 5000`
- `deps[i].length == 2`
- Self-loops `[a, a]` count as cycles

**Optimal complexity:** O(n + e) — Kahn's algorithm (BFS topological sort).
