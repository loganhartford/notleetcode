from typing import List


def reverseString(s: List[str]) -> None:
    # Modify s in place. Do not return anything.
    firstIndex = 0
    lastIndex = len(s) - 1
    for i in range(len(s)):
        print(firstIndex)
        temp = s[lastIndex]
        s[lastIndex] = s[firstIndex]
        s[firstIndex] = temp
        firstIndex = firstIndex + 1
        lastIndex = lastIndex - 1
        if firstIndex >= lastIndex:
            return
    return
