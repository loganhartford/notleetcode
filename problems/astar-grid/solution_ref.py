from typing import List
import heapq


def astar(grid: List[List[int]], start: List[int], goal: List[int]) -> int:
    rows, cols = len(grid), len(grid[0])
    sr, sc = start
    gr, gc = goal

    def h(r: int, c: int) -> int:
        return abs(r - gr) + abs(c - gc)  # Manhattan

    g_cost = [[float("inf")] * cols for _ in range(rows)]
    g_cost[sr][sc] = 0
    heap = [(h(sr, sc), 0, sr, sc)]  # (f = g + h, g, r, c)
    while heap:
        f, g, r, c = heapq.heappop(heap)
        if (r, c) == (gr, gc):
            return g
        if g > g_cost[r][c]:
            continue
        for dr, dc in ((1, 0), (-1, 0), (0, 1), (0, -1)):
            nr, nc = r + dr, c + dc
            if 0 <= nr < rows and 0 <= nc < cols and grid[nr][nc] == 0:
                ng = g + 1
                if ng < g_cost[nr][nc]:
                    g_cost[nr][nc] = ng
                    heapq.heappush(heap, (ng + h(nr, nc), ng, nr, nc))
    return -1
