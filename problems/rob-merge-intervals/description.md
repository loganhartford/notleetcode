# Merge Intervals

Given a list of closed intervals `[start, end]`, merge all overlapping intervals and return them sorted.

```cpp
vector<vector<int>> mergeIntervals(vector<vector<int>> intervals);
```

Two intervals overlap if one starts before or at the end of the other. Touching intervals like `[1,2]` and `[2,3]` should be merged into `[1,3]`.

## Examples

```
mergeIntervals([[1,3],[2,6],[8,10],[15,18]]) → [[1,6],[8,10],[15,18]]
mergeIntervals([[1,4],[4,5]])                → [[1,5]]
mergeIntervals([])                           → []
```

## Constraints

- `0 <= intervals.length <= 10^4`
- `intervals[i].length == 2`
- `0 <= start <= end <= 10^4`
- Input may be unsorted

**Optimal complexity:** O(n log n) — sort by start, then single linear merge pass.
