# Time: O(k · n)  — k iterations, each with O(n) nearest-neighbour scan
import math
import random


def rrt(grid, start, goal, max_iters=4000, step=1.0, goal_tol=1.5, seed=0):
    rows, cols = len(grid), len(grid[0])
    rng = random.Random(seed)

    def free(x, y):
        if x < 0 or y < 0 or x >= cols or y >= rows:
            return False
        return grid[int(y)][int(x)] == 0

    def segment_free(a, b):
        steps = 5
        for i in range(steps + 1):
            t = i / steps
            if not free(a[0] + (b[0] - a[0]) * t, a[1] + (b[1] - a[1]) * t):
                return False
        return True

    nodes = [start]
    parent = {0: None}
    for _ in range(max_iters):
        sample = goal if rng.random() < 0.1 else (rng.uniform(0, cols), rng.uniform(0, rows))
        ni = min(range(len(nodes)),
                 key=lambda i: (nodes[i][0] - sample[0]) ** 2 + (nodes[i][1] - sample[1]) ** 2)
        nx, ny = nodes[ni]
        d = math.hypot(sample[0] - nx, sample[1] - ny)
        if d == 0:
            continue
        newp = (nx + (sample[0] - nx) / d * step, ny + (sample[1] - ny) / d * step)
        if not free(*newp) or not segment_free((nx, ny), newp):
            continue
        nodes.append(newp)
        parent[len(nodes) - 1] = ni
        if math.hypot(newp[0] - goal[0], newp[1] - goal[1]) <= goal_tol:
            path = []
            i = len(nodes) - 1
            while i is not None:
                path.append(nodes[i])
                i = parent[i]
            path.reverse()
            return path
    return None
