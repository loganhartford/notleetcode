# Time: O(n)  — rolling DP
from typing import List


def rob(nums: List[int]) -> int:
    prev, curr = 0, 0  # best up to i-2, best up to i-1
    for n in nums:
        prev, curr = curr, max(curr, prev + n)
    return curr
