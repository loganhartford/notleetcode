from typing import List


def twoSum(numbers: List[int], target: int) -> List[int]:
    # Return 1-indexed positions [index1, index2] with index1 < index2.
    
    """
    The previous non-sorted version of this question is solved using a hashtable
    in O(n). How can we do it differently if it's sorted?
    - The only thing that comes to mind is that with a sorted list, we can early exit
      but that doesn't significanly improve how fast we can go through the list
    """
    
    # O(n)
    # seen = {}
    # for i, num in enumerate(numbers):
    #     comp = target - num
    #     print(target, num, comp)
    #     if comp in seen:
    #         return [seen[comp], i+1]
    #     seen[num] = i+1
        
    # Sorted...
    i = 0
    j = len(numbers) - 1
    while i < j:
        comp = numbers[i] + numbers[j]
        if comp == target:
            return [i+1, j+1]
        if comp < target:
            i += 1
        else:
            j -= 1
    """
    I looked at the ref solution after only implementing the non sorted version...
    
    Why does this work with the sorted list and not the unsorted like?
    - In the unsorted case, we have to check the array becuase we don't know whether or 
      not the next index will be better or worse
    - In the sorted case, we can start from the ends and use the known sorting direction of the
      the list to make more educated index changes
    """
            
    
    