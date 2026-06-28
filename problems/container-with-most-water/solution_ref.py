# Time: O(n)  — two-pointer single pass
from typing import List


def maxArea(height: List[int]) -> int:
    i, j = 0, len(height) - 1
    best = 0
    while i < j:
        best = max(best, min(height[i], height[j]) * (j - i))
        if height[i] < height[j]:
            i += 1
        else:
            j -= 1
    return best
