import math

from solution import Ema, ema_init, ema_update
from nlc import run_cases

# Every check below comes from the contract, not from a reference solution:
#   accuracy:     |output_n - y_n| < 1, where y_n is the ideal real-valued EMA
#                 (tracked here as a float; its error is far below 2^-8);
#   steady state: once the same x has been fed 16 * 2^k times in a row, the
#                 output is exactly x on that update and every later one.

H_SWEEP_LEN = 2400
H_CONV_TRIALS = 24
H_RAILS = [-32768, 32767, -32767, 32766, 0, -1, 1]
H_NOISE, H_WALK, H_STEPS, H_LEVEL, H_RAIL = range(5)
H_SIG_NAMES = ["white noise", "random walk", "random steps", "noisy level", "rail-to-rail"]


class H_State:
    rng = 0x6C8E9CF5
    bad = False
    why = ""


R = H_State()


def h_next():
    s = R.rng
    s = (s ^ (s << 13)) & 0xFFFFFFFF
    s ^= s >> 17
    s = (s ^ (s << 5)) & 0xFFFFFFFF
    R.rng = s
    return s


def h_rand_i16():
    return (h_next() & 0xFFFF) - 32768


def h_rand_in(lo, hi):
    return lo + h_next() % (hi - lo + 1)


def h_rand_rail():
    return H_RAILS[h_next() % len(H_RAILS)]


def h_clamp16(v):
    return max(-32768, min(32767, v))


def h_bound(k):
    return 16 << k


def h_fail(msg):
    if not R.bad:
        R.bad = True
        R.why = msg


def h_begin():
    R.bad = False
    R.why = ""


def h_end(exp):
    return R.why if R.bad else exp


def h_allowed(y):
    lo = math.floor(y)
    if lo == y:
        return "exactly {}".format(lo)
    return "{} or {}".format(lo, lo + 1)


class H_Trk:
    def __init__(self, tag=""):
        self.f = Ema()
        self.tag = tag
        self.k = 0
        self.ideal = 0.0
        self.n = 0
        self.run_x = None
        self.run_len = 0


def h_init(t, k, initial):
    ema_init(t.f, k, initial)
    t.k = k
    t.ideal = float(initial)
    t.n = 0
    t.run_x = None
    t.run_len = 0


def h_feed(t, x):
    got = ema_update(t.f, x)
    t.n += 1
    t.ideal += (x - t.ideal) / (1 << t.k)
    if x == t.run_x:
        t.run_len += 1
    else:
        t.run_x = x
        t.run_len = 1
    if type(got) is not int:
        h_fail("{}update #{} (k = {}, x = {}) returned {!r}, which is not an int".format(
            t.tag, t.n, t.k, x, got))
    elif not abs(got - t.ideal) < 1.0:
        h_fail("{}update #{} (k = {}, x = {}) returned {}, but the ideal y_n is {:.4f}, so it must be {}".format(
            t.tag, t.n, t.k, x, got, t.ideal, h_allowed(t.ideal)))
    elif t.run_len >= h_bound(t.k) and got != x:
        h_fail("{}update #{} (k = {}) returned {}, but x = {} has now been fed {} times in a row "
               "(limit 16 * 2^k = {}), so the output must be exactly {} (ideal y_n = {:.4f})".format(
                   t.tag, t.n, t.k, got, x, t.run_len, h_bound(t.k), x, t.ideal))
    return got


def h_hold(t, x, count):
    for _ in range(count):
        h_feed(t, x)


def h_step_exp(k, initial, x):
    if initial == x:
        return "exactly {} on every update (the ideal y_n is exactly {})".format(x, x)
    return "every output within 1 of the ideal y_n, and exactly {} from update {} on".format(x, h_bound(k))


def h_step_case(k, initial, x):
    h_begin()
    t = H_Trk()
    h_init(t, k, initial)
    h_hold(t, x, 2 * h_bound(k))
    return h_end(h_step_exp(k, initial, x))


H_RAILS_EXP = "each output equals its initial value on every update"


def h_rails_case():
    h_begin()
    ks = [1, 8]
    ts = []
    for i in range(4):
        t = H_Trk("filter {}: ".format("abcd"[i]))
        h_init(t, ks[i // 2], 32767 if i % 2 else -32768)
        ts.append(t)
    for i in range(4):
        h_hold(ts[i], 32767 if i % 2 else -32768, 2 * h_bound(ks[i // 2]))
    return h_end(H_RAILS_EXP)


H_SQUARE_EXP = "every output within 1 of the ideal y_n, and exactly 0 from the 32nd update of the 0s on"


def h_square_case():
    h_begin()
    t = H_Trk()
    h_init(t, 1, 0)
    for i in range(2000):
        h_feed(t, -32768 if i % 2 else 32767)
    h_hold(t, 0, 2 * h_bound(1))
    return h_end(H_SQUARE_EXP)


H_INTERLEAVE_EXP = ("each filter tracks its own ideal y_n and settles exactly: "
                    "a at 500 then -32768, b at -7000 then 32767")


def h_interleave_case():
    h_begin()
    a = H_Trk("filter a: ")
    b = H_Trk("filter b: ")
    h_init(a, 3, 0)
    h_init(b, 6, -2000)
    n = 2 * h_bound(6)
    for _ in range(n):
        h_feed(a, 500)
        h_feed(b, -7000)
    for _ in range(n):
        h_feed(b, 32767)
        h_feed(a, -32768)
    return h_end(H_INTERLEAVE_EXP)


H_REINIT_EXP = ("after re-init: exactly -100 on every update of -100, "
                "then exactly 50 from update 2048 of the 50s on")


def h_reinit_case():
    h_begin()
    t = H_Trk()
    h_init(t, 2, 0)
    h_hold(t, 30000, 2 * h_bound(2))
    t.tag = "after re-init: "
    h_init(t, 7, -100)
    h_hold(t, -100, 2 * h_bound(7))
    h_hold(t, 50, 2 * h_bound(7))
    return h_end(H_REINIT_EXP)


H_GARBAGE_EXP = ("each output equals its initial value until the input changes, "
                 "then settles exactly on the new input")


def h_garbage_case():
    h_begin()
    a = H_Trk("filter a: ")
    b = H_Trk("filter b: ")
    a.f.state, a.f.k = 0xA5A5A5A5 - (1 << 32), 0xA5
    b.f.state, b.f.k = -1, 0xFF
    h_init(a, 5, 1234)
    h_init(b, 1, -32768)
    h_hold(a, 1234, 2 * h_bound(5))
    h_hold(a, -1234, 2 * h_bound(5))
    h_hold(b, -32768, 2 * h_bound(1))
    h_hold(b, 32767, 2 * h_bound(1))
    return h_end(H_GARBAGE_EXP)


class H_Gen:
    def __init__(self, kind, level):
        self.kind = kind
        self.level = level
        self.left = 0

    def next(self, k):
        if self.kind == H_NOISE:
            return h_rand_i16()
        if self.kind == H_WALK:
            self.level = h_clamp16(self.level + h_rand_in(-64, 64))
            return self.level
        if self.kind == H_STEPS:
            if self.left <= 0:
                self.level = h_rand_i16()
                self.left = h_rand_in(1, 3 << k)
            self.left -= 1
            return self.level
        if self.kind == H_LEVEL:
            if self.left <= 0:
                self.level = h_rand_i16()
                self.left = h_rand_in(100, 600)
            self.left -= 1
            return h_clamp16(self.level + h_rand_in(-3, 3))
        if self.left <= 0:
            self.level = h_rand_rail()
            self.left = h_rand_in(1, 4 << k)
        self.left -= 1
        return self.level


H_TOL_EXP = "all {} outputs within 1 of the ideal y_n".format(8 * len(H_SIG_NAMES) * H_SWEEP_LEN)


def h_tolerance_sweep():
    h_begin()
    for k in range(1, 9):
        for s in range(len(H_SIG_NAMES)):
            t = H_Trk("{} signal: ".format(H_SIG_NAMES[s]))
            initial = h_rand_i16()
            h_init(t, k, initial)
            g = H_Gen(s, initial)
            for _ in range(H_SWEEP_LEN):
                h_feed(t, g.next(k))
    return h_end(H_TOL_EXP)


H_CONV_EXP = "all {} targets reached exactly by update 16 * 2^k of the hold, and held".format(8 * H_CONV_TRIALS)


def h_convergence_sweep():
    h_begin()
    for k in range(1, 9):
        for trial in range(H_CONV_TRIALS):
            t = H_Trk("trial {}: ".format(trial))
            initial = h_rand_rail() if trial % 4 == 0 else h_rand_i16()
            h_init(t, k, initial)
            hist = h_rand_in(0, 3 << k)
            for _ in range(hist):
                h_feed(t, h_rand_rail() if trial % 2 else h_rand_i16())
            if trial % 3 == 0:
                target = 32767 if initial < 0 else -32768
            else:
                target = h_rand_i16()
            h_hold(t, target, h_bound(k) + (2 << k))
    return h_end(H_CONV_EXP)


H_CASE_FNS = {
    "step": h_step_case,
    "rails": h_rails_case,
    "square": h_square_case,
    "interleave": h_interleave_case,
    "reinit": h_reinit_case,
    "garbage": h_garbage_case,
    "tolerance": h_tolerance_sweep,
    "convergence": h_convergence_sweep,
}


def h_run(kind, *args):
    return H_CASE_FNS[kind](*args)


def h_step(name, k, initial, x):
    return {
        "name": name,
        "args": ["step", k, initial, x],
        "input": "ema_init(f, {}, {}), then ema_update(f, {}) x {}".format(k, initial, x, 2 * h_bound(k)),
        "expected": h_step_exp(k, initial, x),
    }


CASES = [
    h_step("example 1: step 0 -> 1000, k = 4",           4,      0,   1000),
    h_step("example 2: falling step 100 -> -100, k = 2", 2,    100,   -100),
    h_step("example 3: initial == input, k = 8",         8,     -5,     -5),
    h_step("rising step of 1 LSB, k = 8",                8,      0,      1),
    h_step("rising step -300 -> 200, k = 6",             6,   -300,    200),
    h_step("falling step 1000 -> -1000, k = 5",          5,   1000,  -1000),
    h_step("negative falling step, k = 3",               3,   -100, -20000),
    h_step("negative rising step, k = 7",                7, -20000,   -100),
    h_step("full-scale rise to INT16_MAX, k = 8",        8, -32768,  32767),
    h_step("full-scale fall to INT16_MIN, k = 8",        8,  32767, -32768),
    h_step("full-scale rise to INT16_MAX, k = 1",        1, -32768,  32767),
    h_step("small falling step 7 -> -3, k = 1",          1,      7,     -3),
    {
        "name": "initial == input at the rails, k = 1 and 8",
        "args": ["rails"],
        "input": "four filters: (k=1, -32768), (k=1, 32767), (k=8, -32768), (k=8, 32767); "
                 "each fed its own initial value 32 * 2^k times",
        "expected": H_RAILS_EXP,
    },
    {
        "name": "full-scale square wave, k = 1",
        "args": ["square"],
        "input": "ema_init(f, 1, 0), then x = 32767, -32768, 32767, ... for 2000 updates, then x = 0 x 64",
        "expected": H_SQUARE_EXP,
    },
    {
        "name": "two filters interleaved",
        "args": ["interleave"],
        "input": "a: ema_init(a, 3, 0), b: ema_init(b, 6, -2000); alternate updates: a gets 500 and b gets "
                 "-7000 (2048 each), then b gets 32767 and a gets -32768 (2048 each)",
        "expected": H_INTERLEAVE_EXP,
    },
    {
        "name": "re-init a used filter",
        "args": ["reinit"],
        "input": "ema_init(f, 2, 0), ema_update(f, 30000) x 128, then ema_init(f, 7, -100), "
                 "ema_update(f, -100) x 4096, ema_update(f, 50) x 4096",
        "expected": H_REINIT_EXP,
    },
    {
        "name": "garbage struct before init",
        "args": ["garbage"],
        "input": "a: state/k hold garbage, ema_init(a, 5, 1234), 1234 x 1024, -1234 x 1024; "
                 "b: state/k hold garbage, ema_init(b, 1, -32768), -32768 x 64, 32767 x 64",
        "expected": H_GARBAGE_EXP,
    },
    {
        "name": "tolerance sweep, k = 1..8",
        "args": ["tolerance"],
        "input": "for every k = 1..8, five {}-sample signals (white noise, random walk, random steps, "
                 "noisy level, rail-to-rail) into a freshly initialized filter".format(H_SWEEP_LEN),
        "expected": H_TOL_EXP,
    },
    {
        "name": "convergence sweep, k = 1..8",
        "args": ["convergence"],
        "input": "for every k = 1..8, {} trials: ema_init at a random value, up to 3 * 2^k random "
                 "samples, then one random target held for 18 * 2^k updates".format(H_CONV_TRIALS),
        "expected": H_CONV_EXP,
    },
]


run_cases(
    h_run, CASES,
    compare=lambda got, c, args: got == c["expected"],
    show=lambda v: v if isinstance(v, str) else repr(v),
)
