from solution import findCheapestPrice
from nlc import run_cases

CASES = [
    {"name": "k=1 with cycle", "args": [4, [[0, 1, 100], [1, 2, 100], [2, 0, 100], [1, 3, 600], [2, 3, 200]], 0, 3, 1], "expected": 700},
    {"name": "cheaper via stop", "args": [3, [[0, 1, 100], [1, 2, 100], [0, 2, 500]], 0, 2, 1], "expected": 200},
    {"name": "direct only (k=0)", "args": [3, [[0, 1, 100], [1, 2, 100], [0, 2, 500]], 0, 2, 0], "expected": 500},
    {"name": "unreachable", "args": [3, [[0, 1, 100]], 0, 2, 1], "expected": -1},
    {"name": "no flights", "args": [2, [], 0, 1, 5], "expected": -1},
]

run_cases(findCheapestPrice, CASES, fmt_input=lambda c: "n={}, src={}, dst={}, k={}".format(c["args"][0], c["args"][2], c["args"][3], c["args"][4]))
