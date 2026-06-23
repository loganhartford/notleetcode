# Set Matrix Zeroes

Given an `m x n` integer matrix, if an element is `0`, set its **entire row and column** to `0`. Do it **in place** and return `None`.

A naive approach uses O(m·n) extra space; a better one uses O(m+n); the optimal uses O(1) by storing the flags in the first row/column.

### Example 1
```
Input:  matrix = [[1,1,1],
                  [1,0,1],
                  [1,1,1]]
After:  [[1,0,1],
         [0,0,0],
         [1,0,1]]
```

### Example 2
```
Input:  matrix = [[0,1,2,0],
                  [3,4,5,2],
                  [1,3,1,5]]
After:  [[0,0,0,0],
         [0,4,5,0],
         [0,3,1,0]]
```

### Constraints
- `m == len(matrix)`, `n == len(matrix[0])`
- `1 <= m, n <= 200`
- `-2^31 <= matrix[i][j] <= 2^31 - 1`
