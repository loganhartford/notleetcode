# Edit Distance

Given two strings `word1` and `word2`, return the minimum number of single-character edits (insertions, deletions, or substitutions) required to transform `word1` into `word2`.

```cpp
int editDistance(string word1, string word2);
```

## Examples

```
editDistance("horse", "ros")       → 3
editDistance("intention", "execution") → 5
editDistance("", "abc")            → 3
editDistance("abc", "abc")         → 0
```

## Operations

- **Insert**: add one character
- **Delete**: remove one character
- **Replace**: change one character to another

## Constraints

- `0 <= word1.length, word2.length <= 500`

**Optimal complexity:** O(m*n) time, O(min(m,n)) space — 2D DP table where `dp[i][j]` is the edit distance between the first `i` chars of `word1` and first `j` chars of `word2`.
