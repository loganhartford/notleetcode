import math


def transform_point(pose, point):
    x, y, theta = pose
    px, py = point
    c, s = math.cos(theta), math.sin(theta)
    return (x + c * px - s * py, y + s * px + c * py)


def compose(pose_a, pose_b):
    x1, y1, t1 = pose_a
    x2, y2, t2 = pose_b
    c, s = math.cos(t1), math.sin(t1)
    return (x1 + c * x2 - s * y2, y1 + s * x2 + c * y2, t1 + t2)


def invert(pose):
    x, y, theta = pose
    c, s = math.cos(theta), math.sin(theta)
    return (-c * x - s * y, s * x - c * y, -theta)
