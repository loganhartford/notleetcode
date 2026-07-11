from typing import List


def maxSubArray(nums: List[int]) -> int:
    best = current = nums[0]
    for num in nums[1:]:
        current = max(num, current + num)
        best = max(best, current)
    return best
    
        
    
    
    
    # O(n^3) overall
    # O(n)
    # lowStart, highStart = 0, 1
    # while highStart < len(nums) + 1:
    #     low = lowStart
    #     high = highStart
    #     # O(n)
    #     while low < high and high < len(nums) + 1:
    #         sum = 0
    #         # O(n)
    #         for i in range(low, high):
    #             sum += nums[i]
    #         bestSum = max(bestSum, sum)
    #         low += 1
    #         high += 1
    #     highStart +=1
    
    # return bestSum
        
    
    
    