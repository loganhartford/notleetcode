from typing import List
import heapq


def gridDijkstra(grid: List[List[int]], start: List[int], goal: List[int]) -> int:
    rows, cols = len(grid), len(grid[0])
    sr, sc = start
    gr, gc = goal
    if grid[sr][sc] == -1 or grid[gr][gc] == -1:
        return -1

    dist = [[float("inf")] * cols for _ in range(rows)]
    dist[sr][sc] = 0
    heap = [(0, sr, sc)]
    while heap:
        d, r, c = heapq.heappop(heap)
        if (r, c) == (gr, gc):
            return d
        if d > dist[r][c]:
            continue
        for dr, dc in ((1, 0), (-1, 0), (0, 1), (0, -1)):
            nr, nc = r + dr, c + dc
            if 0 <= nr < rows and 0 <= nc < cols and grid[nr][nc] != -1:
                nd = d + grid[nr][nc]  # pay to enter (nr, nc)
                if nd < dist[nr][nc]:
                    dist[nr][nc] = nd
                    heapq.heappush(heap, (nd, nr, nc))
    return -1
