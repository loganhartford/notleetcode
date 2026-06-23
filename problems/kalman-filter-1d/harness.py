from solution import KalmanFilter1D
from nlc import run_cases


def drive(init, ops):
    k = KalmanFilter1D(**init)
    out = []
    for kind, val in ops:
        out.append(round(k.predict(val) if kind == "p" else k.update(val), 6))
    return out


def close(got, case, args):
    exp = case["expected"]
    if got is None or len(got) != len(exp):
        return False
    return all(abs(g - e) < 1e-6 for g, e in zip(got, exp))


CASES = [
    {"name": "repeated measurements", "args": [dict(x=0, P=1, Q=0.0, R=1.0), [("u", 2), ("u", 2), ("u", 2)]],
     "expected": [1.0, 1.333333, 1.5]},
    {"name": "predict + update", "args": [dict(x=0, P=1, Q=1.0, R=1.0), [("p", 1), ("u", 2), ("p", 0), ("u", 2)]],
     "expected": [1.0, 1.666667, 1.666667, 1.875]},
    {"name": "perfect estimate ignores measurement", "args": [dict(x=10, P=0.0, Q=0.0, R=1.0), [("u", 5)]],
     "expected": [10.0]},
]

run_cases(drive, CASES, compare=close, fmt_input=lambda c: "init={}, ops={}".format(*c["args"]))
