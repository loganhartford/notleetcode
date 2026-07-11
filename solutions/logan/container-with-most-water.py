from typing import List


def maxArea(height: List[int]) -> int:
    # O(n^2) version
    # bestArea = 0
    # for i in range(len(height)-1):
    #     for j in range(i+1, len(height)):
    #         area = min(height[i], height[j]) * (j - i)
    #         bestArea = max(area, bestArea)
    # return bestArea


    """
    What we know:
    - we strictly gain a benefit from i and j getting further a part
    - i think this is a sliding window questionlike the best time to buy
    - we want the lowest possible first index, unless the height advantage at the next index
      is greater than the loss from incrementing.
    """
    bestArea = 0
    i, j = 0, (len(height) -1)
    while (i<j):
        h = min(height[i],height[j])
        w = j-i
        bestArea = max(bestArea, (h*w))
        
        print(h, w, bestArea)
        
        """
        - Moving either pointer will strictly make w worse, so h has to get better
        - Move the pointer with the lower h, the larger one can only contribute if we find
          another h that's even taller.
        """
        
        if height[i] < height[j]:
            i += 1
        else:
            j -= 1
            
    return bestArea
        
    
