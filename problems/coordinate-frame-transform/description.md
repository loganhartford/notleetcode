# 2D Coordinate-Frame Transform

Implement 2D rigid-body transforms (rotation + translation). A **pose** is a tuple
`(x, y, theta)` representing the transform `world_T_frame`: a point expressed in
`frame` is rotated by `theta` and then translated by `(x, y)` to get its
coordinates in `world`. `theta` is in radians.

```python
def transform_point(pose, point):   # point in frame -> point in world; returns (x, y)
def compose(pose_a, pose_b):        # world_T_a ∘ a_T_b  -> world_T_b; returns (x, y, theta)
def invert(pose):                   # inverse transform frame_T_world; returns (x, y, theta)
```

Definitions (with `c = cos(theta)`, `s = sin(theta)`):

- **transform_point((x, y, θ), (px, py))** = `(x + c·px − s·py,  y + s·px + c·py)`
- **compose((x1,y1,θ1), (x2,y2,θ2))** = apply pose1 then pose2:
  `(x1 + c1·x2 − s1·y2,  y1 + s1·x2 + c1·y2,  θ1 + θ2)`
- **invert((x, y, θ))** = `(−c·x − s·y,  s·x − c·y,  −θ)`

### Example
```
transform_point((1, 2, 0), (3, 4))            -> (4.0, 6.0)        # pure translation
transform_point((0, 0, pi/2), (1, 0))         -> (0.0, 1.0)        # pure 90° rotation
compose((1,0,pi/2), (1,0,0))                  -> (1.0, 1.0, pi/2)
```

### Notes
- `theta` need not be wrapped; the checker compares angles numerically with a
  small tolerance.
- `[author]`-defined robotics practice.
