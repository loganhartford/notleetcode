# Time: O(n)  — linear scan of path waypoints for lookahead point
import math


def pure_pursuit(pose, path, lookahead) -> float:
    x, y, theta = pose

    target = path[-1]
    for (px, py) in path:
        if math.hypot(px - x, py - y) >= lookahead:
            target = (px, py)
            break

    dx = target[0] - x
    dy = target[1] - y
    x_r = math.cos(theta) * dx + math.sin(theta) * dy
    y_r = -math.sin(theta) * dx + math.cos(theta) * dy
    return 2.0 * y_r / (lookahead * lookahead)
