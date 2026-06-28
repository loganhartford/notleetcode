from typing import List


def threeSum(nums: List[int]) -> List[List[int]]:
    # bad version
    # triplets = []
    # for i in range(len(nums)-2):
    #     for j in range(i+1, len(nums)-1):
    #         for k in range(j+1, len(nums)):
    #             sum = nums[i]+nums[j]+nums[k]
                
    #             if sum == 0:
    #                 triplet = [nums[i],nums[j],nums[k]]
    #                 triplet.sort()
    #                 if triplet not in triplets:
    #                     triplets.append(triplet)
    # return triplets
                    
    """
    Why is this a bad solution?
    - O(n^3)
    """
    # O(n^2)
    triplets = []
    comp = {}
    for i in range(len(nums)-1):
        for j in range(i+1, len(nums)):
            sum = nums[i]+nums[j]
            if -sum in comp and comp[-sum] !=i and comp[-sum] != j:
                triplet = [nums[i],nums[j],nums[comp[-sum]]]
                triplet.sort()
                if triplet not in triplets:
                    triplets.append(triplet)
            
            comp[nums[i]] = i
            comp[nums[j]] = j
                    
    return triplets