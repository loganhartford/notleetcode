# Image Resize — Nearest Neighbor

Resize a grayscale image using **nearest-neighbor interpolation**.

```cpp
// img: in_h x in_w grayscale image (values 0-255)
// Returns resized image of size out_h x out_w
vector<vector<uint8_t>> resizeNearest(
    const vector<vector<uint8_t>>& img,
    int out_h, int out_w);
```

For each output pixel `(r, c)`, map to a source pixel using:

```
src_r = (int)((r + 0.5) * in_h / out_h)   clamped to [0, in_h - 1]
src_c = (int)((c + 0.5) * in_w / out_w)   clamped to [0, in_w - 1]
```

This centers the sampling point within each output pixel.

## Example

```
Input 2x2:        Output 4x4 (2x upscale):
[10, 20]          [10, 10, 20, 20]
[30, 40]          [10, 10, 20, 20]
                  [30, 30, 40, 40]
                  [30, 30, 40, 40]
```

## Constraints

- `1 <= in_h, in_w <= 1000`
- `1 <= out_h, out_w <= 2000`

**Optimal complexity:** O(out_h * out_w) — compute each output pixel independently.
