from typing import List


def twoSum(nums: List[int], target: int) -> List[int]:
    # Return the indices of the two numbers adding up to target.
    seen = {}
    for i, num in enumerate(nums):
        comp = target - num
        if comp in seen:
            return [seen[comp], i]
        seen[num]=i
    pass
