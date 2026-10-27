import signal

from solution import Heap, heap_init, heap_alloc, heap_free
from nlc import run_cases

H_LIMIT_S = 0.4
H_LIMIT_RANDOM_S = 2.0
H_ROUNDS = 4
H_ROUND_OPS = 5000
H_MAX_ARENA = 65536
H_JUNK_MIXED, H_JUNK_ZERO, H_JUNK_ONES = range(3)
H_FORWARD, H_REVERSE, H_ALTERNATE, H_SHUFFLE = range(4)

H_FRESH = "an empty heap fits any sequence of requests whose r(n) + 16 sum to at most size"
H_HOLE = "a freed block's space holds any request up to that block's size"


class H_Hang(BaseException):
    pass


def h_on_alarm(signum, frame):
    raise H_Hang()


signal.signal(signal.SIGALRM, h_on_alarm)


class H_State:
    op = 0
    step = ""
    got = ""
    rng = 0
    used = []


R = H_State()


def h_rand():
    x = R.rng
    x ^= (x << 13) & 0xFFFFFFFF
    x ^= x >> 17
    x ^= (x << 5) & 0xFFFFFFFF
    R.rng = x
    return x


def h_r8(n):
    return (n + 7) & ~7


R.rng = 0x2545F491
H_PAT = bytes((h_rand() >> 11) & 0xFF for _ in range(H_MAX_ARENA + 256))


class H_Heap:
    """One heap under test: the user's Heap object, its arena, and a shadow
    list of live blocks [offset, n, pattern seed, op that created it]."""

    def __init__(self, name, size):
        self.name = name
        self.size = size
        self.arena = bytearray(size)
        self.h = Heap()
        self.live = []
        self.ready = False


class H_Fail(Exception):
    pass


def h_fail(msg):
    R.got = R.step + " " + msg
    raise H_Fail()


def h_junk(arena, kind):
    size = len(arena)
    if kind == H_JUNK_ZERO:
        arena[:] = bytes(size)
        return
    if kind == H_JUNK_ONES:
        arena[:] = b"\xff" * size
        return
    for i in range(0, size, 8):
        if (i // 8) % 2:
            w = (h_rand() << 32) | h_rand()
        else:
            w = ((i * 3) % 200 + 8) | 1
        arena[i:i + 8] = w.to_bytes(8, "little")


def h_setup(name, size, junk):
    """Fresh arena full of garbage, and a Heap whose fields hold garbage."""
    H = H_Heap(name, size)
    h_junk(H.arena, junk)
    for key, val in list(vars(H.h).items()):
        if key == "arena":
            setattr(H.h, key, bytearray(b"\xa5" * 64))
        elif val is None or isinstance(val, (int, float)):
            setattr(H.h, key, 0xA5A5A5A5)
    R.used = [x for x in R.used if x.name != name] + [H]
    return H


def h_check_heap(H):
    if not H.ready:
        return
    if getattr(H.h, "arena", None) is not H.arena:
        h_fail("then %s.arena is not the bytearray passed to heap_init" % H.name)
    if len(H.arena) != H.size:
        h_fail("then %s's arena has length %d instead of %d; the bytearray must keep its length"
               % (H.name, len(H.arena), H.size))
    for key, val in vars(H.h).items():
        if key == "arena":
            continue
        if not (val is None or isinstance(val, (int, float))):
            h_fail("then %s.%s holds a %s; bookkeeping must live in the arena "
                   "(besides the arena, h may hold only int fields)" % (H.name, key, type(val).__name__))
    arena = H.arena
    for off, n, seed, born in H.live:
        if arena[off:off + n] != H_PAT[seed:seed + n]:
            i = 0
            while arena[off + i] == H_PAT[seed + i]:
                i += 1
            h_fail("then live block %d (%d bytes, from op %d) had byte %d changed from 0x%02X to 0x%02X; "
                   "the heap must not write inside a live block"
                   % (off, n, born, i, H_PAT[seed + i], arena[off + i]))


def h_check_all():
    for H in R.used:
        h_check_heap(H)


def h_init(H):
    R.op += 1
    R.step = "op %d: heap_init(%s, arena) with a %d-byte garbage-filled arena" % (R.op, H.name, H.size)
    H.live = []
    H.ready = True
    heap_init(H.h, H.arena)
    h_check_all()


def h_alloc(H, n):
    """Call heap_alloc and validate whatever comes back; None is accepted here."""
    R.op += 1
    R.step = "op %d: heap_alloc(%s, %d)" % (R.op, H.name, n)
    off = heap_alloc(H.h, n)
    if off is not None:
        if isinstance(off, bool) or not isinstance(off, int):
            h_fail("returned %r; expected an int offset or None" % (off,))
        if n == 0:
            h_fail("returned %d; expected None for n == 0" % off)
        if off < 0 or off + n > H.size:
            h_fail("returned %d, but %d bytes there do not fit inside the %d-byte arena" % (off, n, H.size))
        if off % 8:
            h_fail("returned %d, which is not a multiple of 8" % off)
        for o, m, _, born in H.live:
            if off < o + m and o < off + n:
                h_fail("returned %d, but its %d bytes overlap live block %d (%d bytes, from op %d)"
                       % (off, n, o, m, born))
        seed = h_rand() % 256
        H.live.append((off, n, seed, R.op))
        H.arena[off:off + n] = H_PAT[seed:seed + n]
    h_check_all()
    return off


def h_alloc_ok(H, n, why):
    off = h_alloc(H, n)
    if off is None:
        h_fail("returned None; expected a block (%s)" % why)
    return off


def h_alloc_at(H, n, want, why):
    off = h_alloc(H, n)
    if off != want:
        h_fail("returned %r; expected %d (%s)" % (off, want, why))
    return off


def h_alloc_null(H, n, why):
    off = h_alloc(H, n)
    if off is not None:
        h_fail("returned %d; expected None (%s)" % (off, why))


def h_free(H, off):
    R.op += 1
    R.step = "op %d: heap_free(%s, %r)" % (R.op, H.name, off)
    if off is not None:
        H.live = [b for b in H.live if b[0] != off]
    heap_free(H.h, off)
    h_check_all()


def h_free_all(H, order):
    ps = sorted(b[0] for b in H.live)
    if order == H_REVERSE:
        seq = ps[::-1]
    elif order == H_ALTERNATE:
        seq = ps[0::2] + ps[1::2]
    else:
        seq = list(ps)
    if order == H_SHUFFLE:
        for k in range(len(seq), 1, -1):
            j = h_rand() % k
            seq[k - 1], seq[j] = seq[j], seq[k - 1]
    for off in seq:
        h_free(H, off)


def h_big(H):
    return h_alloc_ok(H, H.size - 16, "with no live blocks, size - 16 bytes must fit")


def h_run(body, expected):
    """Every test runs under a timer: a call that never returns is cut off and
    reported as a failure naming the call, and the remaining tests still run."""
    R.op = 0
    R.used = []
    R.rng = 0x9E3779B9
    R.step = "(setup)"
    R.got = "(no result)"
    limit = H_LIMIT_RANDOM_S if body is h_case_random else H_LIMIT_S
    signal.setitimer(signal.ITIMER_REAL, limit)
    try:
        body()
    except H_Fail:
        return R.got
    except H_Hang:
        return "%s did not return within %d ms (infinite loop?)" % (R.step, int(limit * 1000))
    except Exception as e:
        raise RuntimeError("%s raised %s" % (R.step, type(e).__name__)) from e
    finally:
        signal.setitimer(signal.ITIMER_REAL, 0)
    return expected


def h_case_basic():
    H = h_setup("h", 1024, H_JUNK_MIXED)
    h_init(H)
    a = h_alloc_ok(H, 10, H_FRESH)
    b = h_alloc_ok(H, 100, H_FRESH)
    c = h_alloc_ok(H, 1, H_FRESH)
    d = h_alloc_ok(H, 250, H_FRESH)
    h_free(H, b)
    e = h_alloc_ok(H, 40, H_HOLE)
    h_free(H, a)
    h_free(H, d)
    d = h_alloc_ok(H, 200, H_HOLE)
    h_free(H, c)
    h_free(H, e)
    h_free(H, d)


def h_case_align():
    H = h_setup("h", 4096, H_JUNK_MIXED)
    h_init(H)
    p = {}
    for n in range(1, 41):
        p[n] = h_alloc_ok(H, n, H_FRESH)
    for n in range(1, 41, 3):
        h_free(H, p[n])
    for n in range(1, 41, 3):
        p[n] = h_alloc_ok(H, 1 if n == 1 else n - 1, H_HOLE)


def h_case_zero():
    H = h_setup("h", 256, H_JUNK_MIXED)
    h_init(H)
    h_alloc_null(H, 0, "n == 0")
    a = h_alloc_ok(H, 8, H_FRESH)
    h_alloc_null(H, 0, "n == 0")
    h_free(H, None)
    h_free(H, a)
    h_free(H, None)
    h_big(H)


def h_case_too_big():
    H = h_setup("h", 512, H_JUNK_MIXED)
    h_init(H)
    for n in (513, 520, 100000, 2 ** 64 - 1, 2 ** 64 - 7, 2 ** 63, 10 ** 30):
        h_alloc_null(H, n, "n is larger than the 512-byte arena")
    h_free(H, h_big(H))
    h_alloc_ok(H, 300, H_FRESH)
    h_alloc_null(H, 513, "n is larger than the 512-byte arena")
    h_alloc_null(H, 2 ** 64 - 7, "n is larger than the 512-byte arena")


def h_case_first_fit():
    H = h_setup("h", 1024, H_JUNK_MIXED)
    h_init(H)
    a = h_alloc_ok(H, 64, H_FRESH)
    b = h_alloc_ok(H, 128, H_FRESH)
    h_alloc_ok(H, 64, H_FRESH)
    d = h_alloc_ok(H, 64, H_FRESH)
    e = h_alloc_ok(H, 64, H_FRESH)
    h_free(H, b)
    h_free(H, d)
    h_alloc_at(H, 64, b, "b's hole is the lowest free space that fits, even though d's is a tighter fit")
    h_alloc_at(H, 64, d, "the rest of b's hole is too small for 64 bytes; d's hole is next")
    h_free(H, a)
    x = h_alloc_ok(H, 200, "the space above e fits 200 bytes")
    if x <= e:
        h_fail("returned %d; expected an offset above e = %d (a's hole is too small for 200 bytes)" % (x, e))
    h_alloc_at(H, 16, a, "a's hole is now the lowest free space that fits")
    x = h_alloc_ok(H, 24, "the rest of a's hole fits 24 bytes")
    if not a < x < b:
        h_fail("returned %d; expected an offset between a = %d and b = %d "
               "(the rest of a's hole is the lowest free space that fits)" % (x, a, b))


def h_fill_count(H, n, want):
    count = 0
    while h_alloc(H, n) is not None:
        count += 1
    if count < want:
        h_fail("returned None after only %d blocks of %d bytes; at least %d = %d / (%d + 16) must fit"
               % (count, n, want, H.size, h_r8(n)))


def h_case_fill16():
    H = h_setup("h", 4096, H_JUNK_MIXED)
    h_init(H)
    h_fill_count(H, 16, 4096 // 32)
    h_free_all(H, H_FORWARD)
    h_big(H)


def h_case_pack():
    H = h_setup("h", 2048, H_JUNK_MIXED)
    h_init(H)
    h_fill_count(H, 1, 2048 // 24)
    H = h_setup("h", 2048, H_JUNK_MIXED)
    h_init(H)
    sizes = [1, 8, 13, 24, 40, 64, 100, 7, 250, 33]
    used = 0
    i = 0
    while used + h_r8(sizes[i % 10]) + 16 <= H.size:
        n = sizes[i % 10]
        used += h_r8(n) + 16
        h_alloc_ok(H, n, H_FRESH)
        i += 1


def h_fill_mixed(H):
    sizes = [24, 1, 100, 48, 8, 200, 16, 64, 33, 120]
    used = 0
    sure = 0
    i = 0
    while True:
        n = sizes[i % 10]
        if used + h_r8(n) + 16 <= H.size:
            used += h_r8(n) + 16
            sure += 1
        else:
            used = H.size
        if h_alloc(H, n) is None:
            break
        i += 1
    if len(H.live) < sure:
        h_fail("returned None after %d blocks; the first %d requests of this sequence must fit (%s)"
               % (len(H.live), sure, H_FRESH))


def h_free_all_case(order):
    H = h_setup("h", 4096, H_JUNK_MIXED)
    h_init(H)
    for _ in range(2):
        h_fill_mixed(H)
        h_free_all(H, order)
        h_free(H, h_big(H))
    h_alloc_ok(H, 2048, H_FRESH)
    h_alloc_ok(H, 2000, H_FRESH)


def h_case_free_forward():
    h_free_all_case(H_FORWARD)


def h_case_free_reverse():
    h_free_all_case(H_REVERSE)


def h_case_free_alternate():
    h_free_all_case(H_ALTERNATE)


def h_case_free_shuffle():
    h_free_all_case(H_SHUFFLE)


def h_case_merge():
    scenarios = [
        ([37, 20], [0, 1]),
        ([37, 20], [1, 0]),
        ([16, 40, 24], [0, 2, 1]),
        ([16, 40, 24], [1, 2, 0]),
    ]
    for sizes, order in scenarios:
        H = h_setup("h", 512, H_JUNK_MIXED)
        h_init(H)
        p = [h_alloc_ok(H, n, H_FRESH) for n in sizes]
        h_alloc_ok(H, 8, H_FRESH)
        for i in order:
            h_free(H, p[i])
        h_alloc_at(H, sum(h_r8(n) for n in sizes), p[0],
                   "the freed neighbours form one space starting at the first of them")


def h_case_contents():
    A = lambda n: n
    F = lambda k: -k - 1
    script = [
        A(24), A(7), A(100), A(64), A(1), A(33), A(200), A(16), A(48), A(9), A(130), A(72),
        F(1), F(3), F(2), A(50), A(60), A(8),
        F(6), F(8), F(7), A(250),
        F(0), F(4), F(5), A(1), A(2), A(3), A(4),
        F(15), F(10), F(11), F(9), A(300),
    ]
    H = h_setup("h", 2048, H_JUNK_MIXED)
    h_init(H)
    blk = []
    for v in script:
        if v > 0:
            blk.append(h_alloc_ok(H, v, H_FRESH) if len(blk) < 12 else h_alloc(H, v))
        else:
            h_free(H, blk[-v - 1])
            blk[-v - 1] = None
    for off in reversed(blk):
        if off is not None:
            h_free(H, off)
    h_big(H)


def h_case_edges():
    H = h_setup("h", 256, H_JUNK_MIXED)
    h_init(H)
    h_free(H, h_alloc_ok(H, 240, H_FRESH))
    for _ in range(8):
        h_alloc_ok(H, 16, H_FRESH)
    h_free_all(H, H_REVERSE)
    h_free(H, h_big(H))
    H = h_setup("h", 256, H_JUNK_MIXED)
    h_init(H)
    for _ in range(10):
        h_alloc_ok(H, 5, H_FRESH)
    h_free_all(H, H_FORWARD)
    h_big(H)


def h_case_min_max():
    H = h_setup("h", 64, H_JUNK_MIXED)
    h_init(H)
    h_free(H, h_alloc_ok(H, 48, H_FRESH))
    p = h_alloc_ok(H, 8, H_FRESH)
    q = h_alloc_ok(H, 8, H_FRESH)
    h_free(H, p)
    h_free(H, q)
    h_big(H)
    H = h_setup("h", 65536, H_JUNK_MIXED)
    h_init(H)
    h_free(H, h_big(H))
    h_alloc_ok(H, 30000, H_FRESH)
    h_alloc_ok(H, 30000, H_FRESH)
    h_alloc_ok(H, 5000, H_FRESH)
    h_free_all(H, H_SHUFFLE)
    h_big(H)


def h_case_garbage():
    H = None
    for kind in (H_JUNK_ZERO, H_JUNK_ONES, H_JUNK_MIXED):
        H = h_setup("h", 512, kind)
        h_init(H)
        h_alloc_ok(H, 100, H_FRESH)
        b = h_alloc_ok(H, 50, H_FRESH)
        h_alloc_ok(H, 8, H_FRESH)
        h_free(H, b)
        h_alloc_ok(H, 40, H_HOLE)
        h_free_all(H, H_SHUFFLE)
        h_big(H)
    h_free(H, H.live[0][0])
    h_alloc_ok(H, 200, H_FRESH)
    h_alloc_ok(H, 100, H_FRESH)
    h_alloc_ok(H, 64, H_FRESH)
    R.op += 1
    R.step = "op %d: heap_init(h, arena) again on a heap holding 3 live blocks" % R.op
    H.live = []
    heap_init(H.h, H.arena)
    h_check_all()
    h_big(H)


def h_case_two_heaps():
    A = h_setup("a", 512, H_JUNK_MIXED)
    B = h_setup("b", 1024, H_JUNK_ONES)
    h_init(A)
    h_init(B)
    a1 = h_alloc_ok(A, 64, H_FRESH)
    b1 = h_alloc_ok(B, 64, H_FRESH)
    h_alloc_ok(A, 100, H_FRESH)
    h_alloc_ok(B, 200, H_FRESH)
    h_free(A, a1)
    h_alloc_ok(B, 32, H_FRESH)
    h_alloc_at(A, 32, a1, "a1's hole is a's lowest free space that fits")
    h_free(B, b1)
    h_alloc_at(B, 64, b1, "b1's hole is b's lowest free space that fits")
    h_free_all(A, H_FORWARD)
    h_big(A)
    h_free_all(B, H_SHUFFLE)
    h_big(B)
    h_free(A, A.live[0][0])
    h_alloc_ok(A, 8, H_FRESH)


def h_case_random():
    H = h_setup("h", 8192, H_JUNK_MIXED)
    h_init(H)
    fails = 0
    for rnd in range(H_ROUNDS):
        for _ in range(H_ROUND_OPS):
            try:
                if not H.live or h_rand() % 100 < 55:
                    if h_alloc(H, 1 + h_rand() % 300) is None:
                        fails += 1
                else:
                    h_free(H, H.live[h_rand() % len(H.live)][0])
            except H_Fail:
                R.got += " [workload round %d: %d live blocks holding %d bytes]" % (
                    rnd + 1, len(H.live), sum(b[1] for b in H.live))
                raise
        h_free_all(H, H_SHUFFLE)
        h_free(H, h_big(H))
    if fails == 0:
        h_fail("(harness error: the workload never filled the heap)")


def h_case(name, body, inp, expected):
    return {"name": name, "args": [body, expected], "input": inp, "expected": expected}


CASES = [
    h_case("alloc, write, read back", h_case_basic,
           "size 1024: alloc 10, 100, 1, 250; free the 100; alloc 40; free the 10 and the 250; alloc 200; free all",
           "every alloc returns a usable block; bytes written to each block read back unchanged"),
    h_case("8-byte alignment", h_case_align,
           "size 4096: alloc n = 1, 2, ..., 40; free n = 1, 4, 7, ..., 40; alloc 1, 3, 6, ..., 39 (one less each)",
           "every returned offset is a multiple of 8"),
    h_case("n == 0 and heap_free(None)", h_case_zero,
           "size 256: alloc 0; alloc 8; alloc 0; free None; free the 8; free None; alloc 240",
           "alloc 0 returns None; free None does nothing; the empty heap still fits 240 bytes"),
    h_case("requests larger than the arena", h_case_too_big,
           "size 512: alloc 513, 520, 100000, 2**64 - 1, 2**64 - 7, 2**63, 10**30; alloc 496; free it; "
           "alloc 300; alloc 513, 2**64 - 7",
           "every request larger than the arena returns None; they use no space, so 496 still fits"),
    h_case("first fit by address", h_case_first_fit,
           "size 1024: a = alloc 64, b = alloc 128, c = alloc 64, d = alloc 64, e = alloc 64; free b, free d; "
           "alloc 64; alloc 64; free a; alloc 200; alloc 16; alloc 24",
           "alloc 64 -> b (lowest hole that fits, though d's is tighter); alloc 64 -> d (rest of b's hole is too "
           "small); alloc 200 -> above e; alloc 16 -> a; alloc 24 -> between a and b"),
    h_case("pack 16-byte blocks", h_case_fill16,
           "size 4096: alloc 16 until None; free all in address order; alloc 4080",
           "at least 4096 / (16 + 16) = 128 blocks fit; once all are freed, 4080 fits"),
    h_case("pack small and mixed blocks", h_case_pack,
           "size 2048: alloc 1 until None. Then a fresh 2048 heap: alloc 1, 8, 13, 24, 40, 64, 100, 7, 250, 33, "
           "1, 8, ... while the running total of r(n) + 16 stays <= 2048",
           "at least 2048 / (8 + 16) = 85 one-byte blocks fit; every request in the mixed sequence succeeds"),
    h_case("free all in address order", h_case_free_forward,
           "size 4096, twice: alloc 24, 1, 100, 48, 8, 200, 16, 64, 33, 120, 24, 1, ... until None; "
           "free every block, lowest offset first; alloc 4080; free it. Then alloc 2048, alloc 2000",
           "the leading requests that fit by the space rule succeed; after freeing everything, 4080 fits"),
    h_case("free all in reverse order", h_case_free_reverse,
           "same as above, but free every block highest offset first",
           "the leading requests that fit by the space rule succeed; after freeing everything, 4080 fits"),
    h_case("free all alternating", h_case_free_alternate,
           "same as above, but free the 1st, 3rd, 5th, ... block by offset, then the 2nd, 4th, ...",
           "the leading requests that fit by the space rule succeed; after freeing everything, 4080 fits"),
    h_case("free all shuffled", h_case_free_shuffle,
           "same as above, but free the blocks in a seeded random order",
           "the leading requests that fit by the space rule succeed; after freeing everything, 4080 fits"),
    h_case("freed neighbours act as one space", h_case_merge,
           "four fresh 512-byte heaps. p = alloc 37, q = alloc 20, guard alloc 8: free p, q, then alloc 64; "
           "again freeing q, p. p = alloc 16, q = alloc 40, r = alloc 24, guard alloc 8: free p, r, q, then "
           "alloc 80; again freeing q, r, p",
           "each final alloc returns p (40 + 24 = 64 and 16 + 40 + 24 = 80 fit where the neighbours were)"),
    h_case("live blocks keep their contents", h_case_contents,
           "size 2048: alloc 24, 7, 100, 64, 1, 33, 200, 16, 48, 9, 130, 72 (blocks 0-11); free 1, 3, 2; "
           "alloc 50, 60, 8; free 6, 8, 7; alloc 250; free 0, 4, 5; alloc 1, 2, 3, 4; free 15, 10, 11, 9; "
           "alloc 300; free the rest; alloc 2032",
           "after every call, every live block still holds exactly the bytes written to it; 2032 fits at the end"),
    h_case("blocks that reach the end of the arena", h_case_edges,
           "size 256: alloc 240, write all 240 bytes, free it; alloc 16 eight times (8 x 32 = 256); free all; "
           "alloc 240. A fresh 256 heap: alloc 5 ten times; free all; alloc 240",
           "every request fits, and the arena bytearray keeps its length"),
    h_case("smallest and largest arenas", h_case_min_max,
           "size 64: alloc 48; free; alloc 8, alloc 8; free both; alloc 48. "
           "Size 65536: alloc 65520; free; alloc 30000, 30000, 5000; free all; alloc 65520",
           "all succeed: sizes up to 65536 must work, and 64 bytes fit a 48-byte block or two 8-byte blocks"),
    h_case("garbage before init, and re-init", h_case_garbage,
           "three 512-byte arenas filled with 0x00, 0xFF, then random bytes and fake size words, Heap fields "
           "set to garbage: init; alloc 100, 50, 8; free the 50; alloc 40; free all; alloc 496. Then free; "
           "alloc 200, 100, 64; heap_init again; alloc 496",
           "init ignores whatever the memory held; re-init discards the live blocks so 496 fits"),
    h_case("two heaps interleaved", h_case_two_heaps,
           "heap a (512 bytes) and heap b (1024 bytes): a1 = alloc(a, 64), b1 = alloc(b, 64), alloc(a, 100), "
           "alloc(b, 200), free a1, alloc(b, 32), alloc(a, 32), free b1, alloc(b, 64); free all of a; "
           "alloc(a, 496); free all of b; alloc(b, 1008)",
           "each heap keeps its own state: alloc(a, 32) -> a1, alloc(b, 64) -> b1, and each heap "
           "fits size - 16 once emptied"),
    h_case("20000 random operations", h_case_random,
           "size 8192, 4 rounds of 5000 seeded random calls: alloc 55% (n = 1..300) / free a random live block "
           "45%; after each round free every block in random order and alloc 8176",
           "every call keeps the rules (aligned, inside the arena, no overlap, live contents untouched, "
           "bookkeeping only in the arena) and 8176 fits after each round"),
]

run_cases(h_run, CASES, show=str)
