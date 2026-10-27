import signal

from solution import Bitmap, bm_init, bm_alloc, bm_alloc_run, bm_free
from nlc import run_cases

H_IDS = 128
H_WORDS = 4
H_LIMIT_S = 2.0
H_CHURN_OPS = 20000
H_MAX_RANGES = 16
H_JUNK = [0xDEADBEEF, 0xFFFFFFFF, 0x0000FFFE, 0x80000001]
H_BAD_IDS = [-1, 128, 129, 159]
H_ODD_NS = [0, 128, 129, 0xFFFFFFFF]


class H_Hang(BaseException):
    pass


def h_on_alarm(signum, frame):
    raise H_Hang()


signal.signal(signal.SIGALRM, h_on_alarm)


class H_State:
    op = 0
    step = ""
    got = ""
    when = ""
    bms = []
    rng = 0


R = H_State()


class H_Bm:
    """A bitmap under test plus the harness's own model of which IDs are in use."""

    def __init__(self, name):
        self.name = name
        self.b = Bitmap()
        self.used = [False] * H_IDS
        self.words = [0] * H_WORDS
        self.ok = True

    def mark(self, i, in_use):
        self.used[i] = in_use
        if in_use:
            self.words[i // 32] |= 1 << (i % 32)
        else:
            self.words[i // 32] &= ~(1 << (i % 32))

    def clear(self):
        self.used = [False] * H_IDS
        self.words = [0] * H_WORDS


def h_fail(msg):
    R.got = R.step + " " + msg
    return False


def h_check_one(m):
    bits = getattr(m.b, "bits", None)
    if type(bits) is not list or len(bits) != H_WORDS:
        return h_fail("left %s.bits = %r; it must stay a list of %d ints" % (m.name, bits, H_WORDS))
    extra = sorted(k for k in getattr(m.b, "__dict__", {}) if k != "bits")
    if extra:
        return h_fail("left %s with attribute(s) %s besides bits; all state must live in bits"
                      % (m.name, ", ".join(extra)))
    for w in range(H_WORDS):
        got = bits[w]
        if type(got) is not int or not 0 <= got < 1 << 32:
            return h_fail("left %s.bits[%d] = %r; each word must be an int in [0, 2**32)" % (m.name, w, got))
        want = m.words[w]
        if got != want:
            diff = got ^ want
            i = w * 32 + (diff & -diff).bit_length() - 1
            return h_fail("left %s.bits[%d] = 0x%08X; expected 0x%08X (ID %d should be %s)"
                          % (m.name, w, got, want, i, "in use" if m.used[i] else "free"))
    return True


def h_check():
    """Every call is followed by a check of the raw words of every live bitmap."""
    R.when = "expected after this op"
    return all(h_check_one(m) for m in R.bms)


def h_fresh(name):
    """A bitmap whose words hold garbage, then init. Its .ok says whether init passed."""
    m = H_Bm(name)
    m.b.bits = list(H_JUNK)
    R.bms.append(m)
    R.step = "bm_init(%s) on a garbage-filled bitmap" % name
    bm_init(m.b)
    m.ok = h_check()
    return m


def h_reinit(m, what):
    R.op += 1
    R.step = "op %d: bm_init(%s) %s" % (R.op, m.name, what)
    bm_init(m.b)
    m.clear()
    return h_check()


def h_alloc(m, want):
    R.op += 1
    R.step = "op %d: bm_alloc(%s)" % (R.op, m.name)
    R.when = "before this op"
    got = bm_alloc(m.b)
    if type(got) is not int or got != want:
        return h_fail("returned %r; expected %d" % (got, want))
    if want >= 0:
        m.mark(want, True)
    return h_check()


def h_arun(m, n, want):
    R.op += 1
    R.step = "op %d: bm_alloc_run(%s, %d)" % (R.op, m.name, n)
    R.when = "before this op"
    got = bm_alloc_run(m.b, n)
    if type(got) is not int or got != want:
        return h_fail("returned %r; expected %d" % (got, want))
    if want >= 0:
        for i in range(want, want + n):
            m.mark(i, True)
    return h_check()


def h_free(m, id, want):
    R.op += 1
    R.step = "op %d: bm_free(%s, %d)" % (R.op, m.name, id)
    R.when = "before this op"
    got = bm_free(m.b, id)
    if got is not want:
        return h_fail("returned %r; expected %r" % (got, want))
    if want:
        m.mark(id, False)
    return h_check()


def h_alloc_seq(m, first, count):
    """Allocs that must return first, first+1, ..., first+count-1."""
    return all(h_alloc(m, first + i) for i in range(count))


def h_free_seq(m, first, last):
    return all(h_free(m, i, True) for i in range(first, last + 1))


def h_run(body, expected):
    """Every test runs under a timer: a call that never returns is cut off and
    reported as a failure naming the call, and the remaining tests still run."""
    R.op = 0
    R.step = "(setup)"
    R.got = "(no result)"
    R.bms = []
    signal.setitimer(signal.ITIMER_REAL, H_LIMIT_S)
    try:
        ok = body()
    except H_Hang:
        return "%s did not return within %d ms (infinite loop?)" % (R.step, int(H_LIMIT_S * 1000))
    finally:
        signal.setitimer(signal.ITIMER_REAL, 0)
    return expected if ok else R.got


def h_case_first():
    t = h_fresh("t")
    return t.ok and h_alloc(t, 0) and h_alloc(t, 1) and h_alloc(t, 2)


def h_case_hole():
    t = h_fresh("t")
    return (t.ok and h_alloc_seq(t, 0, 10)
            and h_free(t, 7, True) and h_free(t, 3, True)
            and h_alloc(t, 3) and h_alloc(t, 7) and h_alloc(t, 10))


def h_case_fill():
    t = h_fresh("t")
    return (t.ok and h_alloc_seq(t, 0, H_IDS)
            and h_alloc(t, -1) and h_alloc(t, -1)
            and h_free(t, 100, True) and h_alloc(t, 100) and h_alloc(t, -1))


def h_case_boundaries():
    ids = [31, 32, 63, 64, 127]
    t = h_fresh("t")
    return (t.ok and h_alloc_seq(t, 0, H_IDS)
            and all(h_free(t, i, True) and h_alloc(t, i) for i in ids)
            and all(h_free(t, i, True) for i in reversed(ids))
            and all(h_alloc(t, i) for i in ids)
            and h_alloc(t, -1))


def h_case_bad_free():
    t = h_fresh("t")
    return (t.ok
            and h_free(t, 0, False) and h_free(t, 127, False)
            and h_free(t, -1, False) and h_free(t, 128, False)
            and h_alloc_seq(t, 0, H_IDS)
            and h_free(t, -1, False) and h_free(t, 128, False) and h_free(t, 129, False)
            and h_free(t, 5, True) and h_free(t, 5, False)
            and h_free(t, 127, True) and h_free(t, 127, False)
            and h_free(t, 0, True) and h_free(t, 0, False)
            and h_alloc(t, 0) and h_alloc(t, 5) and h_alloc(t, 127) and h_alloc(t, -1))


def h_case_run1():
    t = h_fresh("t")
    return (t.ok
            and h_arun(t, 1, 0) and h_arun(t, 1, 1) and h_alloc(t, 2) and h_arun(t, 1, 3)
            and h_free(t, 1, True) and h_arun(t, 1, 1) and h_alloc(t, 4)
            and all(h_arun(t, 1, i) for i in range(5, H_IDS))
            and h_arun(t, 1, -1) and h_alloc(t, -1))


def h_case_cross():
    t = h_fresh("t")
    return (t.ok and h_alloc_seq(t, 0, 30)
            and h_arun(t, 4, 30) and h_alloc(t, 34)
            and h_arun(t, 40, 35) and h_alloc(t, 75)
            and h_arun(t, 53, -1) and h_arun(t, 52, 76) and h_alloc(t, -1))


def h_case_run32():
    t = h_fresh("t")
    return (t.ok
            and h_arun(t, 32, 0) and h_arun(t, 32, 32) and h_alloc(t, 64)
            and h_arun(t, 32, 65) and h_arun(t, 32, -1) and h_arun(t, 31, 97) and h_alloc(t, -1)
            and h_free_seq(t, 64, 95)
            and h_arun(t, 33, -1) and h_arun(t, 32, 64) and h_alloc(t, -1))


def h_case_run128():
    t = h_fresh("t")
    return (t.ok
            and h_arun(t, 128, 0) and h_alloc(t, -1) and h_arun(t, 1, -1)
            and h_free(t, 70, True) and h_arun(t, 128, -1) and h_arun(t, 2, -1) and h_arun(t, 1, 70)
            and h_free_seq(t, 0, H_IDS - 1) and h_arun(t, 128, 0)
            and h_reinit(t, "on a full bitmap")
            and h_alloc(t, 0) and h_arun(t, 128, -1) and h_arun(t, 127, 1)
            and h_reinit(t, "on a full bitmap")
            and h_arun(t, 127, 0) and h_arun(t, 2, -1) and h_alloc(t, 127))


def h_case_gaps():
    t = h_fresh("t")
    return (t.ok and h_arun(t, 128, 0)
            and h_free(t, 2, True) and h_free_seq(t, 5, 6) and h_free_seq(t, 10, 12)
            and h_free_seq(t, 20, 25) and h_free_seq(t, 28, 35) and h_free_seq(t, 40, 43)
            and h_free_seq(t, 60, 67)
            and h_arun(t, 4, 20) and h_arun(t, 6, 28) and h_arun(t, 4, 40) and h_arun(t, 4, 60)
            and h_arun(t, 3, 10) and h_arun(t, 4, 64) and h_arun(t, 2, 5) and h_arun(t, 2, 24)
            and h_arun(t, 2, 34) and h_arun(t, 2, -1) and h_alloc(t, 2) and h_alloc(t, -1))


def h_case_bad_n():
    t = h_fresh("t")
    return (t.ok
            and h_arun(t, 0, -1) and h_arun(t, 129, -1) and h_arun(t, 1000, -1)
            and h_arun(t, 0xFFFFFFFF, -1)
            and h_alloc(t, 0)
            and h_arun(t, 0, -1) and h_arun(t, 128, -1) and h_arun(t, 127, 1)
            and h_arun(t, 1, -1) and h_arun(t, 0, -1))


def h_case_init():
    t = h_fresh("t")
    if not (t.ok and h_alloc(t, 0) and h_arun(t, 127, 1)
            and h_reinit(t, "on a full bitmap") and h_arun(t, 128, 0)):
        return False
    t.b.bits = [0x55555555, 0xAAAAAAAA, 0x0F0F0F0F, 0xF0F0F0F0]
    return (h_reinit(t, "on bits = [0x55555555, 0xAAAAAAAA, 0x0F0F0F0F, 0xF0F0F0F0]")
            and h_alloc(t, 0) and h_alloc(t, 1) and h_arun(t, 126, 2) and h_alloc(t, -1))


def h_case_two():
    a = h_fresh("a")
    if not a.ok:
        return False
    b = h_fresh("b")
    return (b.ok
            and h_alloc_seq(a, 0, 3) and h_alloc(b, 0)
            and h_arun(a, 40, 3) and h_arun(b, 40, 1)
            and h_free(a, 1, True) and h_free(b, 1, True) and h_free(b, 50, False) and h_free(a, 42, True)
            and h_alloc(a, 1) and h_alloc(b, 1)
            and h_arun(a, 128, -1) and h_arun(b, 87, 41) and h_alloc(b, -1)
            and h_alloc(a, 42) and h_alloc(a, 43))


# Long random workload checked against the model after every call. The PRNG
# and the op mix match the C harness, so both languages see the same operations.
def h_rand():
    x = R.rng
    x ^= (x << 13) & 0xFFFFFFFF
    x ^= x >> 17
    x ^= (x << 5) & 0xFFFFFFFF
    R.rng = x
    return x


def h_model_alloc(m):
    for i in range(H_IDS):
        if not m.used[i]:
            return i
    return -1


def h_model_run(m, n):
    if n == 0 or n > H_IDS:
        return -1
    length = 0
    for i in range(H_IDS):
        length = 0 if m.used[i] else length + 1
        if length == n:
            return i + 1 - n
    return -1


def h_pick_id(m):
    """Mostly in-use IDs, some random (possibly already free) IDs, a few out of range."""
    x = h_rand() % 100
    if x < 4:
        return H_BAD_IDS[h_rand() % 4]
    if x < 50:
        return h_rand() % H_IDS
    start = h_rand() % H_IDS
    for k in range(H_IDS):
        if m.used[(start + k) % H_IDS]:
            return (start + k) % H_IDS
    return start


def h_pick_n():
    y = h_rand() % 100
    if y < 3:
        return H_ODD_NS[h_rand() % 4]
    if y < 60:
        return 1 + h_rand() % 4
    if y < 90:
        return 1 + h_rand() % 16
    return 1 + h_rand() % 64


def h_churn_fail(m):
    ranges = []
    i = 0
    while i < H_IDS:
        if m.used[i]:
            i += 1
            continue
        j = i
        while j + 1 < H_IDS and not m.used[j + 1]:
            j += 1
        ranges.append(str(i) if i == j else "%d-%d" % (i, j))
        i = j + 1
    if len(ranges) > H_MAX_RANGES:
        ranges = ranges[:H_MAX_RANGES] + ["..."]
    R.got += " [model %s: %d of %d IDs in use; free IDs: %s]" % (
        R.when, sum(m.used), H_IDS, ", ".join(ranges) if ranges else "none")
    return False


def h_case_churn():
    t = h_fresh("t")
    if not t.ok:
        return False
    R.rng = 0x9E3779B9
    for k in range(H_CHURN_OPS):
        free_pct = 85 if (k // 1000) % 2 else 40
        r = h_rand() % 100
        if r < free_pct:
            i = h_pick_id(t)
            ok = h_free(t, i, 0 <= i < H_IDS and t.used[i])
        elif r < free_pct + (100 - free_pct) // 2:
            ok = h_alloc(t, h_model_alloc(t))
        else:
            n = h_pick_n()
            ok = h_arun(t, n, h_model_run(t, n))
        if not ok:
            return h_churn_fail(t)
    return True


def h_case(name, body, inp, expected):
    return {"name": name, "args": [body, expected], "input": inp, "expected": expected}


CASES = [
    h_case("first allocs return 0, 1, 2", h_case_first,
           "bm_init on a garbage-filled bitmap; alloc x3",
           "init leaves every word 0; the allocs return 0, 1, 2, leaving bits[0] = 0x00000007"),
    h_case("alloc reuses the lowest hole", h_case_hole,
           "alloc x10 (IDs 0..9); free 7; free 3; alloc x3",
           "both frees return True; the allocs return 3, 7, 10"),
    h_case("fill all 128", h_case_fill,
           "alloc x128; alloc x2; free 100; alloc x2",
           "allocs return 0..127, then -1 twice with bits unchanged; free 100 returns True; "
           "then alloc returns 100, then -1"),
    h_case("word boundary IDs 31, 32, 63, 64, 127", h_case_boundaries,
           "alloc x128; for each of 31, 32, 63, 64, 127: free it, then alloc; "
           "then free 127, 64, 63, 32, 31; alloc x6",
           "each alloc after a single free returns the ID just freed; "
           "the last six allocs return 31, 32, 63, 64, 127, -1"),
    h_case("free out of range and double free", h_case_bad_free,
           "empty: free 0, 127, -1, 128; alloc x128; free -1, 128, 129; "
           "free 5 twice, free 127 twice, free 0 twice; alloc x4",
           "frees of free or out-of-range IDs return False and leave bits unchanged; "
           "the first free of 5, 127 and 0 returns True; the allocs return 0, 5, 127, -1"),
    h_case("run of 1 behaves like bm_alloc", h_case_run1,
           "run(1), run(1), alloc, run(1); free 1; run(1), alloc; run(1) x123; run(1), alloc",
           "0, 1, 2, 3; free returns True; 1, 4; the 123 runs return 5..127; then -1, -1"),
    h_case("run crossing word boundaries", h_case_cross,
           "alloc x30 (IDs 0..29); run(4); alloc; run(40); alloc; run(53); run(52); alloc",
           "run(4) returns 30 (IDs 30..33 span bits[0] and bits[1]); alloc 34; run(40) returns 35; "
           "alloc 75; run(53) returns -1 with bits unchanged; run(52) returns 76; alloc -1"),
    h_case("runs of exactly 32", h_case_run32,
           "run(32), run(32), alloc, run(32), run(32), run(31), alloc; free 64..95; "
           "run(33), run(32), alloc",
           "0, 32, 64, 65, -1 (only 31 IDs free), 97, -1; the frees return True; "
           "run(33) -1, run(32) 64, alloc -1"),
    h_case("run of all 128", h_case_run128,
           "run(128), alloc, run(1); free 70; run(128), run(2), run(1); free 0..127; run(128); "
           "bm_init; alloc, run(128), run(127); bm_init; run(127), run(2), alloc",
           "0, -1, -1; free True; -1, -1 (bits unchanged), 70; frees True; 0; "
           "after init: 0, -1, 1; after init: 0, -1, 127"),
    h_case("lowest start that fits", h_case_gaps,
           "run(128); free 2, 5..6, 10..12, 20..25, 28..35, 40..43, 60..67; "
           "run(4), run(6), run(4), run(4), run(3), run(4), run(2), run(2), run(2), run(2); alloc x2",
           "20, 28, 40, 60, 10, 64, 5, 24, 34, -1; alloc 2, -1. Each run starts at the lowest ID "
           "where it fits, not in the smallest gap and not only within one word"),
    h_case("n of 0 or above BM_IDS", h_case_bad_n,
           "empty: run(0), run(129), run(1000), run(4294967295); alloc; run(0), run(128), run(127); "
           "run(1), run(0)",
           "-1 four times with bits unchanged; alloc 0; -1, -1, 1; -1, -1"),
    h_case("init clears any previous state", h_case_init,
           "bm_init on a garbage-filled bitmap; alloc, run(127); bm_init; run(128); "
           "set bits to [0x55555555, 0xAAAAAAAA, 0x0F0F0F0F, 0xF0F0F0F0]; bm_init; "
           "alloc x2, run(126), alloc",
           "every bm_init leaves all four words 0; alloc 0, run(127) 1; run(128) 0; "
           "alloc 0, 1, run(126) 2, alloc -1"),
    h_case("two bitmaps are independent", h_case_two,
           "bitmaps a and b: a alloc x3; b alloc; a run(40); b run(40); free a 1, b 1, b 50, a 42; "
           "a alloc; b alloc; a run(128); b run(87); b alloc; a alloc x2",
           "each call changes only its own bitmap: a 0, 1, 2; b 0; a 3; b 1; "
           "True, True, False, True; a 1; b 1; a -1; b 41; b -1; a 42, 43"),
    h_case("20000 random operations", h_case_churn,
           "seeded random bm_alloc, bm_alloc_run (n mostly 1..16, some up to 64, a few of "
           "0, 128, 129, 4294967295) and bm_free (in-use IDs, random IDs, and -1, 128, 129, 159), "
           "switching every 1000 ops between mostly allocating and mostly freeing",
           "every return value and the raw bits after every call match a reference model"),
]

run_cases(h_run, CASES, show=str)
