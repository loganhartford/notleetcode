from typing import List


def pacificAtlantic(heights: List[List[int]]) -> List[List[int]]:
    if not heights or not heights[0]:
        return []
    rows, cols = len(heights), len(heights[0])
    pacific = set()
    atlantic = set()

    def dfs(r: int, c: int, seen: set, prev: int) -> None:
        if (r < 0 or r >= rows or c < 0 or c >= cols or
                (r, c) in seen or heights[r][c] < prev):
            return
        seen.add((r, c))
        for dr, dc in ((1, 0), (-1, 0), (0, 1), (0, -1)):
            dfs(r + dr, c + dc, seen, heights[r][c])

    for c in range(cols):
        dfs(0, c, pacific, heights[0][c])
        dfs(rows - 1, c, atlantic, heights[rows - 1][c])
    for r in range(rows):
        dfs(r, 0, pacific, heights[r][0])
        dfs(r, cols - 1, atlantic, heights[r][cols - 1])

    return [[r, c] for (r, c) in (pacific & atlantic)]
