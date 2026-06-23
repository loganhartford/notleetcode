from solution import wallsAndGates
from nlc import run_cases

INF = 2147483647

CASES = [
    {"name": "example",
     "args": [[[INF, -1, 0, INF],
               [INF, INF, INF, -1],
               [INF, -1, INF, -1],
               [0, -1, INF, INF]]],
     "expected": [[3, -1, 0, 1],
                  [2, 2, 1, -1],
                  [1, -1, 2, -1],
                  [0, -1, 3, 4]]},
    {"name": "single gate", "args": [[[0]]], "expected": [[0]]},
    {"name": "unreachable", "args": [[[0, -1, INF]]], "expected": [[0, -1, INF]]},
    {"name": "no gate", "args": [[[INF, INF], [INF, INF]]], "expected": [[INF, INF], [INF, INF]]},
]

run_cases(
    wallsAndGates, CASES,
    compare=lambda got, c, a: got is None and a[0] == c["expected"],
    fmt_input=lambda c: "rooms (INF=2147483647)",
)
