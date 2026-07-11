from typing import List


def threeSum(nums: List[int]) -> List[List[int]]:
    triplets = []
    nums.sort() #O(nlogn)
    
    """
    What I failed to see:
    - You make sure the triplets are unique before the sum, duplicates only show up right next to each other
    - Had the right idea about moving the pointers based on the sum, but wrong imp
    - Missed the lowest number being > 0 as a stop condition
    """
    
    for i in range(len(nums) - 2):
        if i > 0 and nums[i] == nums[i-1]:
            # skip duplicates
            continue
        if nums[i] > 0:
            # can't make a 0 sum
            break
        low, high = i + 1, len(nums) - 1
        while low < high:
            sum = nums[i] + nums[low] + nums[high]
            if sum > 0:
                # make the bigger number smaller
                high -= 1
            elif sum < 0:
                # make the smaller buger
                low +=1
            else:
                # good triplet
                triplets.append([nums[i], nums[low], nums[high]])
                low += 1
                high -= 1
                while low < high and nums[low] == nums[low - 1]:
                    low += 1
                while low < high and nums[high] == nums[high + 1]:
                    high -= 1
    
    
    # O(n^3) version
    # for i in range(len(nums)-2):
    #     for j in range(i+1, len(nums)-1):
    #         for k in range(j+1, len(nums)):
    #             sum = nums[i]+nums[j]+nums[k]
                
    #             if sum == 0:
    #                 triplet = [nums[i],nums[j],nums[k]]
    #                 triplet.sort()
    #                 if triplet not in triplets:
    #                     triplets.append(triplet)
    
    return triplets
        