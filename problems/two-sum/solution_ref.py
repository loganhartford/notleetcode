# Time: O(n)  — hash map single pass
from typing import List


def twoSum(nums: List[int], target: int) -> List[int]:
    seen = {}  # value -> index
    for i, x in enumerate(nums):
        if target - x in seen:
            return [seen[target - x], i]
        seen[x] = i
    return []
