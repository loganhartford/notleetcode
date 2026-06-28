# Time: O(n² · log n)  — Dijkstra minimising maximum elevation seen
from typing import List
import heapq


def swimInWater(grid: List[List[int]]) -> int:
    n = len(grid)
    visited = [[False] * n for _ in range(n)]
    heap = [(grid[0][0], 0, 0)]  # (max elevation so far, r, c)
    visited[0][0] = True
    while heap:
        t, r, c = heapq.heappop(heap)
        if r == n - 1 and c == n - 1:
            return t
        for dr, dc in ((1, 0), (-1, 0), (0, 1), (0, -1)):
            nr, nc = r + dr, c + dc
            if 0 <= nr < n and 0 <= nc < n and not visited[nr][nc]:
                visited[nr][nc] = True
                heapq.heappush(heap, (max(t, grid[nr][nc]), nr, nc))
    return -1
