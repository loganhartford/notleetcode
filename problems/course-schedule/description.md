# Course Schedule

There are `numCourses` courses labeled `0` to `numCourses - 1`. You are given an array `prerequisites` where `prerequisites[i] = [a, b]` means you **must take course `b` before course `a`**.

Return `True` if you can finish all courses, or `False` otherwise.

This is equivalent to detecting whether the directed "prerequisite" graph has a **cycle** (a cycle makes it impossible).

### Example 1
```
Input:  numCourses = 2, prerequisites = [[1, 0]]
Output: True       # take 0, then 1
```

### Example 2
```
Input:  numCourses = 2, prerequisites = [[1, 0], [0, 1]]
Output: False      # 0 and 1 depend on each other
```

### Constraints
- `1 <= numCourses <= 2000`
- `0 <= len(prerequisites) <= 5000`
- `prerequisites[i].length == 2`, all pairs distinct.
