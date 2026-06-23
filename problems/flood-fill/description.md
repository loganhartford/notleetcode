# Flood Fill

You are given an image represented by an `m x n` grid of integers `image`, where `image[i][j]` is the pixel value. You are also given three integers `sr`, `sc`, and `color`.

Perform a **flood fill** starting from the pixel `(sr, sc)`: change that pixel's color, and the color of any pixel connected **4-directionally** to it that has the **same original color**, and so on, to `color`.

Return the modified image.

### Example 1
```
Input:  image = [[1,1,1],[1,1,0],[1,0,1]], sr = 1, sc = 1, color = 2
Output: [[2,2,2],[2,2,0],[2,0,1]]
```

### Example 2
```
Input:  image = [[0,0,0],[0,0,0]], sr = 0, sc = 0, color = 0
Output: [[0,0,0],[0,0,0]]      # already that color — no change, no infinite loop
```

### Constraints
- `m == len(image)`, `n == len(image[0])`
- `1 <= m, n <= 50`
- `0 <= image[i][j], color < 2^16`
- `0 <= sr < m`, `0 <= sc < n`

Watch the case where the start pixel already equals `color` — don't loop forever.
