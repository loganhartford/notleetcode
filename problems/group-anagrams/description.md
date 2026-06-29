# Group Anagrams

Given an array of strings `strs`, group the **anagrams** together. You can return the answer in any order, and the groups in any order.

An **anagram** is a word formed by rearranging the letters of another, using all the original letters exactly once.

### Example 1
```
Input:  strs = ["eat", "tea", "tan", "ate", "nat", "bat"]
Output: [["bat"], ["nat", "tan"], ["ate", "eat", "tea"]]
```

### Example 2
```
Input:  strs = [""]
Output: [[""]]
```

### Example 3
```
Input:  strs = ["a"]
Output: [["a"]]
```

### Constraints
- `1 <= len(strs) <= 10^4`
- `0 <= len(strs[i]) <= 100`
- `strs[i]` consists of lowercase English letters.

The checker is order-independent: groups and the strings within them may be in any order.

**Optimal complexity:** O(n · k log k)  — sort each string of average length k as hash key
