# Time: O(max(|dx|, |dy|))  — one step per output pixel
def bresenham(x0, y0, x1, y1):
    points = []
    dx = abs(x1 - x0)
    dy = abs(y1 - y0)
    sx = 1 if x0 < x1 else -1
    sy = 1 if y0 < y1 else -1
    err = dx - dy
    x, y = x0, y0
    while True:
        points.append((x, y))
        if x == x1 and y == y1:
            break
        e2 = 2 * err
        if e2 > -dy:
            err -= dy
            x += sx
        if e2 < dx:
            err += dx
            y += sy
    return points


def line_of_sight(grid, start, end):
    for (x, y) in bresenham(start[0], start[1], end[0], end[1]):
        if grid[y][x] == 1:
            return False
    return True
