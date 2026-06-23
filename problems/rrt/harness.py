import math
from solution import rrt
from nlc import run_cases


def valid_path(grid, start, goal, path, step, goal_tol):
    if not path:
        return False
    rows, cols = len(grid), len(grid[0])

    def free(x, y):
        return 0 <= x < cols and 0 <= y < rows and grid[int(y)][int(x)] == 0

    if math.hypot(path[0][0] - start[0], path[0][1] - start[1]) > 1e-6:
        return False
    if math.hypot(path[-1][0] - goal[0], path[-1][1] - goal[1]) > goal_tol + 1e-6:
        return False
    if any(not free(p[0], p[1]) for p in path):
        return False
    if any(math.hypot(b[0] - a[0], b[1] - a[1]) > step + 1e-6 for a, b in zip(path, path[1:])):
        return False
    return True


def drive(grid, start, goal, solvable, seed):
    path = rrt(grid, start, goal, max_iters=4000, step=1.0, goal_tol=1.5, seed=seed)
    if solvable:
        return valid_path(grid, start, goal, path, 1.0, 1.5)
    return path is None


OPEN = [[0] * 10 for _ in range(10)]
WALL = [[0] * 10 for _ in range(10)]
for _r in range(0, 8):
    WALL[_r][5] = 1  # vertical wall with a gap at the bottom
ENCLOSED = [[0] * 7 for _ in range(7)]
for _cell in [(2, 2), (2, 3), (2, 4), (3, 2), (3, 4), (4, 2), (4, 3), (4, 4)]:
    ENCLOSED[_cell[0]][_cell[1]] = 1

CASES = [
    {"name": "open map (valid path)", "args": [OPEN, (0.5, 0.5), (9.5, 9.5), True, 1], "expected": True},
    {"name": "wall with gap (valid path)", "args": [WALL, (0.5, 0.5), (9.5, 0.5), True, 2], "expected": True},
    {"name": "enclosed goal (no path)", "args": [ENCLOSED, (0.5, 0.5), (3.5, 3.5), False, 3], "expected": True},
]

run_cases(drive, CASES, fmt_input=lambda c: "start={}, goal={}, solvable={}".format(c["args"][1], c["args"][2], c["args"][3]))
