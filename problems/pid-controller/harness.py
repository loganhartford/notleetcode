from solution import PID
from nlc import run_cases


def drive(params, steps):
    pid = PID(**params)
    return [round(pid.update(sp, meas), 6) for sp, meas in steps]


def close(got, case, args):
    exp = case["expected"]
    if got is None or len(got) != len(exp):
        return False
    return all(abs(g - e) < 1e-6 for g, e in zip(got, exp))


CASES = [
    {"name": "basic PID",
     "args": [dict(kp=1.0, ki=0.5, kd=0.1, dt=1.0), [(10, 0), (10, 2), (10, 5), (10, 8)]],
     "expected": [16.0, 16.8, 16.2, 14.2]},
    {"name": "saturation + anti-windup",
     "args": [dict(kp=2.0, ki=1.0, kd=0.0, dt=1.0, out_max=5.0), [(10, 0), (10, 0), (10, 0)]],
     "expected": [5.0, 5.0, 5.0]},
    {"name": "proportional with dt=0.5",
     "args": [dict(kp=1.0, ki=0.0, kd=0.0, dt=0.5), [(5, 0), (5, 5), (5, 10)]],
     "expected": [5.0, 0.0, -5.0]},
]

run_cases(drive, CASES, compare=close, fmt_input=lambda c: "{}, steps={}".format(c["args"][0], c["args"][1]))
