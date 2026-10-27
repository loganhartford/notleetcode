import signal

from solution import Median, median_init, median_update
from nlc import run_cases

H_MAX = 15
H_LIMIT_S = 2.0
H_SWEEP_LEN = 2000
H_INT16_MIN = -32768
H_INT16_MAX = 32767


class H_Hang(BaseException):
    pass


def h_on_alarm(signum, frame):
    raise H_Hang()


signal.signal(signal.SIGALRM, h_on_alarm)


class H_State:
    step = ""
    got = ""
    filters = []
    rng = 0


R = H_State()


def h_fmt(vals):
    return "[" + ", ".join(repr(v) for v in vals) + "]"


def h_same(got, want):
    return type(got) is int and got == want


def h_fresh(name, n):
    """A filter whose fields hold garbage that looks like a full window of large values, then init."""
    m = Median()
    m.ring = [-31000 - 100 * i if i % 2 else 31000 + 100 * i for i in range(H_MAX)]
    m.n = H_MAX
    m.head = 6
    m.count = H_MAX
    R.filters.append((m, name))
    R.step = "median_init(%s, %d) on a garbage-filled filter" % (name, n)
    median_init(m, n)
    return m


def h_reinit(m, name, n):
    R.step = "median_init(%s, %d) on a filter already in use" % (name, n)
    median_init(m, n)


def h_update(m, name, sample, idx):
    R.step = "median_update(%s, %d), sample %d of %s" % (name, sample, idx, name)
    return median_update(m, sample)


def h_report(name, ctx, ins, got, want):
    """The outputs, plus which call was wrong first. Returns (ok, text)."""
    text = h_fmt(got)
    for i, (x, g, w) in enumerate(zip(ins, got, want)):
        if not h_same(g, w):
            text += "  <- first wrong: sample %d%s, median_update(%s, %d) returned %r, expected %d" % (
                i + 1, ctx, name, x, g, w)
            return False, text
    return True, text


def h_shape_ok():
    for m, name in R.filters:
        if not isinstance(m.ring, list) or len(m.ring) != H_MAX:
            R.got = "%s.ring is now %s; it must stay a list of length MEDIAN_MAX" % (
                name, "length %d" % len(m.ring) if isinstance(m.ring, list) else type(m.ring).__name__)
            return False
    return True


def h_run(body, expected):
    """Every test runs under a timer: a call that never returns is cut off and
    reported as a failure naming the call, and the remaining tests still run."""
    R.step = "(setup)"
    R.got = "(no result)"
    R.filters = []
    signal.setitimer(signal.ITIMER_REAL, H_LIMIT_S)
    try:
        ok = body()
    except H_Hang:
        return "%s did not return within %d ms (infinite loop?)" % (R.step, int(H_LIMIT_S * 1000))
    finally:
        signal.setitimer(signal.ITIMER_REAL, 0)
    if ok:
        h_shape_ok()
    return R.got


def h_seq_body(n, ins, want):
    def body():
        m = h_fresh("m", n)
        got = [h_update(m, "m", x, i + 1) for i, x in enumerate(ins)]
        ok, R.got = h_report("m", "", ins, got, want)
        return ok
    return body


def h_seq_case(name, n, ins, want, note=None):
    assert len(ins) == len(want)
    inp = "median_init(m, %d); median_update(m, x) for x in %s" % (n, h_fmt(ins))
    if note:
        inp += "  (%s)" % note
    expected = h_fmt(want)
    return {"name": name, "args": [h_seq_body(n, ins, want), expected], "input": inp, "expected": expected}


# Two filters fed alternately: a, b, a, b, ...
H_A_IN = [1, 100, 2, 99, 3, 98, 4]
H_A_WANT = [1, 1, 2, 99, 3, 98, 4]
H_B_IN = [-50, 50, -40, 40, -30, 30, -20]
H_B_WANT = [-50, -50, -40, -40, -30, 30, -20]


def h_two_body():
    a = h_fresh("a", 3)
    b = h_fresh("b", 5)
    ga, gb = [], []
    for i, (x, y) in enumerate(zip(H_A_IN, H_B_IN)):
        ga.append(h_update(a, "a", x, i + 1))
        gb.append(h_update(b, "b", y, i + 1))
    ok_a, ta = h_report("a", " of a", H_A_IN, ga, H_A_WANT)
    ok_b, tb = h_report("b", " of b", H_B_IN, gb, H_B_WANT)
    R.got = "a: %s; b: %s" % (ta, tb)
    return ok_a and ok_b


# One filter re-initialised twice, with a different n each time.
H_REINIT = [
    (5, [1000, 1000, 1000, 1000, 1000, 1000], [1000, 1000, 1000, 1000, 1000, 1000]),
    (3, [1, 2, 3, 4], [1, 1, 2, 3]),
    (7, [5, -5, 6], [5, -5, 5]),
]


def h_reinit_body():
    m = None
    ok, parts = True, []
    for n, ins, want in H_REINIT:
        if m is None:
            m = h_fresh("m", n)
        else:
            h_reinit(m, "m", n)
        got = [h_update(m, "m", x, i + 1) for i, x in enumerate(ins)]
        ok_p, text = h_report("m", " after median_init(m, %d)" % n, ins, got, want)
        ok = ok and ok_p
        parts.append("n=%d: %s" % (n, text))
    R.got = "; ".join(parts)
    return ok


# Long pseudo-random run checked against a slow model that keeps the whole
# history: copy the last min(k, n) samples, insertion sort, take the lower
# middle. The PRNG and sample mix match the C harness.
def h_next():
    x = R.rng
    x ^= (x << 13) & 0xFFFFFFFF
    x ^= x >> 17
    x ^= (x << 5) & 0xFFFFFFFF
    R.rng = x
    return x


def h_rand_sample():
    """1/16 INT16_MIN, 1/16 INT16_MAX, 6/16 full range, 8/16 small values -8..8 (many repeats)."""
    r = h_next()
    sel = r & 15
    if sel == 0:
        return H_INT16_MIN
    if sel == 1:
        return H_INT16_MAX
    if sel < 8:
        return ((r >> 16) & 0xFFFF) - 32768
    return ((r >> 8) % 17) - 8


def h_window(hist, k, n):
    return hist[max(0, k - n):k]


def h_model(hist, k, n):
    win = h_window(hist, k, n)
    for i in range(1, len(win)):
        v = win[i]
        j = i - 1
        while j >= 0 and win[j] > v:
            win[j + 1] = win[j]
            j -= 1
        win[j + 1] = v
    return win[(len(win) - 1) // 2]


def h_sweep_body(n):
    def body():
        R.rng = (0x2545F491 + n) & 0xFFFFFFFF
        m = h_fresh("m", n)
        hist = []
        wrong, first = 0, None
        for i in range(H_SWEEP_LEN):
            x = h_rand_sample()
            hist.append(x)
            got = h_update(m, "m", x, i + 1)
            exp = h_model(hist, i + 1, n)
            if not h_same(got, exp):
                if first is None:
                    first = (i + 1, x, got, exp)
                wrong += 1
        if not wrong:
            R.got = "all %d correct" % H_SWEEP_LEN
            return True
        k, x, got, exp = first
        win = h_window(hist, k, n)
        R.got = ("sample %d: median_update(m, %d) returned %r, expected %d, the median of the last %d "
                 "samples %s (oldest first); %d of %d wrong") % (
                     k, x, got, exp, len(win), h_fmt(win), wrong, H_SWEEP_LEN)
        return False
    return body


def h_sweep_case(n):
    expected = "all %d correct" % H_SWEEP_LEN
    return {
        "name": "%d random samples, n = %d" % (H_SWEEP_LEN, n),
        "args": [h_sweep_body(n), expected],
        "input": ("median_init(m, %d); %d seeded pseudo-random samples: full-range int16 values, "
                  "INT16_MIN, INT16_MAX, and small values -8..8 with many repeats") % (n, H_SWEEP_LEN),
        "expected": expected,
    }


def h_case(name, body, inp, expected):
    return {"name": name, "args": [body, expected], "input": inp, "expected": expected}


CASES = [
    h_seq_case("example 1: spike rejected", 5,
               [10, 12, 11, 500, 13, 12, 11],
               [10, 10, 11, 11, 12, 12, 12]),
    h_seq_case("example 2: warm-up takes the lower middle", 5,
               [40, 20, 30, 10],
               [40, 20, 30, 20]),
    h_seq_case("example 3: n = 3", 3,
               [5, 7, 900, 6, 8, 4],
               [5, 5, 7, 7, 8, 6]),
    h_seq_case("spikes in a flat signal", 5,
               [10, 10, 500, 10, 10, 10, 10, -500, 10, 10, 10, 500, 501, 10, 10, 10],
               [10] * 16,
               note="single and double spikes, both signs"),
    h_seq_case("n = 1 returns each sample", 1,
               [5, -3, 32767, -32768, 0, 0, 7, 7, -1],
               [5, -3, 32767, -32768, 0, 0, 7, 7, -1]),
    h_seq_case("n = 3 on a ramp up and down", 3,
               [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0],
               [0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 8, 8, 7, 6, 5, 4, 3, 2, 1]),
    h_seq_case("negatives, INT16_MIN and INT16_MAX", 5,
               [-32768, 32767, -1, 0, -32768, 32767, 32767, -5, -32768, -32768, 1, -32768, 32767],
               [-32768, -32768, -1, -1, -1, 0, 0, 0, -5, -5, -5, -32768, -32768]),
    h_seq_case("all duplicates, then a step", 7,
               [-7] * 10 + [3] * 10,
               [-7] * 13 + [3] * 7),
    h_seq_case("old values leave the window", 5,
               [1000, 1, 2, 3, 4, 5, 6, 7, 8, -1000, 9, 10, 11, 12, 13, 14],
               [1000, 1, 2, 2, 3, 3, 4, 5, 6, 6, 7, 8, 9, 10, 11, 12],
               note="1000 and -1000 must stop counting 5 samples later"),
    h_seq_case("n = 15 (MEDIAN_MAX)", 15,
               [-99, 47, -14, 132, 71, 10, 156, 95, 241, 180, 119, 265, 204, 350, 289, 228, 374, 313,
                459, 398, -30000, 30000, 422, 361, 507, 446, 592, 32767, 470, 616, 555, 701, 640, 579,
                725, 664, 810, 749, 688, 834],
               [-99, -99, -14, -14, 47, 10, 47, 47, 71, 71, 95, 95, 119, 119, 132, 156, 180, 204, 228,
                241, 241, 265, 289, 313, 350, 361, 374, 398, 422, 446, 459, 470, 507, 555, 579, 592,
                592, 616, 640, 664],
               note="a noisy rising signal with spikes at samples 21, 22 and 28"),
    h_case("two filters interleaved", h_two_body,
           "median_init(a, 3); median_init(b, 5); then alternate median_update(a, x), "
           "median_update(b, y): a gets %s, b gets %s" % (h_fmt(H_A_IN), h_fmt(H_B_IN)),
           "a: %s; b: %s" % (h_fmt(H_A_WANT), h_fmt(H_B_WANT))),
    h_case("re-init a used filter", h_reinit_body,
           "; ".join("median_init(m, %d); feed %s" % (n, h_fmt(ins)) for n, ins, _ in H_REINIT),
           "; ".join("n=%d: %s" % (n, h_fmt(want)) for n, _, want in H_REINIT)),
] + [h_sweep_case(n) for n in (1, 3, 5, 7, 15)]

run_cases(h_run, CASES, show=str)
