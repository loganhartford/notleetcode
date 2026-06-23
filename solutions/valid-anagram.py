def isAnagram(s: str, t: str) -> bool:
    print("---")
    if len(s) != len(t):
        return False
    seen = {}
    for char in s:
        if char not in seen:
            seen[char] = 1
        else:
            seen[char] = seen[char] + 1
        print(char, seen)
    print()
    for char in t:
        print(char, seen)
        if char not in seen:
            return False
        seen[char] = seen[char] - 1
        if seen[char] < 0:
            return False
    
    
    return True
