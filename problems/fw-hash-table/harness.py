import signal

from solution import HashTable, ht_init, ht_put, ht_get, ht_del
from nlc import run_cases

H_CAP = 64
H_LIMIT_S = 0.5
H_POOL = 96
H_CHURN_OPS = 20000


class H_Hang(BaseException):
    pass


def h_on_alarm(signum, frame):
    raise H_Hang()


signal.signal(signal.SIGALRM, h_on_alarm)


class H_State:
    op = 0
    step = ""
    got = ""
    tables = []
    rng = 0
    live = 0


R = H_State()


def h_key(k):
    return str(k) if k < 65536 else "0x%X" % k


def h_name(t):
    for tab, name in R.tables:
        if tab is t:
            return name
    return "t"


def h_fail(msg):
    R.got = R.step + " " + msg
    return False


def h_fresh(name):
    """A table whose lists hold garbage that looks like live data, then init."""
    t = HashTable()
    t.keys = [i * 64 for i in range(H_CAP)]
    t.vals = [0xBAD00000 + i for i in range(H_CAP)]
    t.state = [i % 3 for i in range(H_CAP)]
    t.count = 0xA5A5A5A5
    R.tables.append((t, name))
    R.step = "ht_init(%s) on a garbage-filled table" % name
    ht_init(t)
    return t


def h_put(t, key, val, want):
    R.op += 1
    R.step = "op %d: ht_put(%s, %s, %d)" % (R.op, h_name(t), h_key(key), val)
    got = ht_put(t, key, val)
    if got is not want:
        return h_fail("returned %r; expected %r" % (got, want))
    return True


def h_del(t, key, want):
    R.op += 1
    R.step = "op %d: ht_del(%s, %s)" % (R.op, h_name(t), h_key(key))
    got = ht_del(t, key)
    if got is not want:
        return h_fail("returned %r; expected %r" % (got, want))
    return True


def h_get_hit(t, key, want):
    R.op += 1
    R.step = "op %d: ht_get(%s, %s)" % (R.op, h_name(t), h_key(key))
    got = ht_get(t, key)
    if got is None or isinstance(got, bool) or got != want:
        return h_fail("returned %r; expected %d" % (got, want))
    return True


def h_get_miss(t, key):
    R.op += 1
    R.step = "op %d: ht_get(%s, %s)" % (R.op, h_name(t), h_key(key))
    got = ht_get(t, key)
    if got is not None:
        return h_fail("returned %r; expected None" % (got,))
    return True


def h_shape_ok():
    for t, name in R.tables:
        for field in ("keys", "vals", "state"):
            n = len(getattr(t, field))
            if n != H_CAP:
                R.got = "%s.%s now has length %d; the lists must keep length HT_CAP" % (name, field, n)
                return False
    return True


def h_run(body, expected):
    """Every test runs under a timer: a call that never returns is cut off and
    reported as a failure naming the call, and the remaining tests still run."""
    R.op = 0
    R.step = "(setup)"
    R.got = "(no result)"
    R.tables = []
    signal.setitimer(signal.ITIMER_REAL, H_LIMIT_S)
    try:
        ok = body()
    except H_Hang:
        return "%s did not return within %d ms (infinite loop?)" % (R.step, int(H_LIMIT_S * 1000))
    finally:
        signal.setitimer(signal.ITIMER_REAL, 0)
    if ok:
        ok = h_shape_ok()
    return expected if ok else R.got


def h_mixed(i):
    """0, 1, 64, 2, 128, 3, ...: multiples of 64 interleaved with small keys."""
    return (i // 2) * 64 if i % 2 == 0 else i // 2 + 1


def h_case_basic():
    t = h_fresh("t")
    pairs = [(1, 100), (2, 200), (3, 300), (1000000, 4), (64, 5)]
    return (all(h_put(t, k, v, True) for k, v in pairs)
            and all(h_get_hit(t, k, v) for k, v in pairs))


def h_case_get_missing():
    t = h_fresh("t")
    return (h_get_miss(t, 42) and h_get_miss(t, 0)
            and h_put(t, 1, 10, True) and h_put(t, 65, 20, True)
            and h_get_miss(t, 129) and h_get_miss(t, 2) and h_get_hit(t, 65, 20))


def h_case_update():
    t = h_fresh("t")
    return (h_put(t, 7, 1, True) and h_put(t, 7, 2, True) and h_get_hit(t, 7, 2)
            and h_put(t, 7, 3, True) and h_get_hit(t, 7, 3)
            and all(h_put(t, 100 + j, j, True) for j in range(H_CAP - 1))
            and h_put(t, 999, 9, False) and h_get_hit(t, 7, 3) and h_get_miss(t, 999))


def h_case_extreme_keys():
    mx = 0xFFFFFFFF
    t = h_fresh("t")
    return (h_get_miss(t, 0) and h_get_miss(t, mx)
            and h_put(t, 0, 0, True) and h_get_hit(t, 0, 0) and h_get_miss(t, mx)
            and h_put(t, mx, mx, True) and h_get_hit(t, mx, mx) and h_get_hit(t, 0, 0)
            and h_del(t, 0, True) and h_get_miss(t, 0) and h_get_hit(t, mx, mx)
            and h_del(t, mx, True) and h_get_miss(t, mx))


def h_case_fill():
    t = h_fresh("t")
    return (all(h_put(t, j * 64, j, True) for j in range(H_CAP))
            and h_put(t, 4096, 1, False) and h_get_miss(t, 4096)
            and h_put(t, 64, 777, True) and h_get_hit(t, 64, 777)
            and h_put(t, 4096, 1, False)
            and all(h_get_hit(t, j * 64, 777 if j == 1 else j) for j in range(H_CAP)))


def h_case_del_get():
    t = h_fresh("t")
    return (h_put(t, 10, 1, True) and h_put(t, 74, 2, True) and h_put(t, 138, 3, True)
            and h_del(t, 74, True) and h_get_miss(t, 74)
            and h_get_hit(t, 10, 1) and h_get_hit(t, 138, 3))


def h_case_del_missing():
    t = h_fresh("t")
    return (h_del(t, 5, False) and h_put(t, 5, 50, True)
            and h_del(t, 69, False) and h_del(t, 6, False) and h_get_hit(t, 5, 50))


def h_case_del_twice():
    t = h_fresh("t")
    return (h_put(t, 42, 1, True) and h_del(t, 42, True) and h_del(t, 42, False)
            and h_get_miss(t, 42) and h_put(t, 42, 2, True) and h_get_hit(t, 42, 2))


def h_case_del_every_other():
    t = h_fresh("t")
    return (all(h_put(t, h_mixed(i), 1000 + i, True) for i in range(60))
            and all(h_del(t, h_mixed(i), True) for i in range(0, 60, 2))
            and all(h_get_hit(t, h_mixed(i), 1000 + i) if i % 2 else h_get_miss(t, h_mixed(i))
                    for i in range(60)))


def h_case_reput():
    t = h_fresh("t")
    later = range(30, 60)
    return (all(h_put(t, h_mixed(i), 1000 + i, True) for i in range(60))
            and all(h_del(t, h_mixed(i), True) for i in range(30))
            and all(h_put(t, h_mixed(i), 2000 + i, True) for i in later)
            and all(h_get_hit(t, h_mixed(i), 2000 + i) for i in later)
            and all(h_del(t, h_mixed(i), True) for i in later)
            and all(h_get_miss(t, h_mixed(i)) for i in later)
            and all(h_del(t, h_mixed(i), False) for i in later)
            and all(h_put(t, 0x10000 + j * 64, j, True) for j in range(H_CAP))
            and h_put(t, 0x10000 + 64 * 64, 0, False))


def h_case_full_del_insert():
    t = h_fresh("t")
    return (all(h_put(t, h_mixed(j), j, True) for j in range(H_CAP))
            and h_del(t, h_mixed(30), True) and h_put(t, 6400, 1, True) and h_get_hit(t, 6400, 1)
            and all(h_get_hit(t, h_mixed(j), j) for j in range(H_CAP) if j != 30)
            and h_put(t, 6464, 2, False) and h_get_miss(t, h_mixed(30)) and h_get_miss(t, 6464))


def h_case_rounds():
    t = h_fresh("t")
    for r in range(20):
        keys = [(r + 1) * 0x100000 + j * 64 for j in range(H_CAP)]
        ok = (all(h_put(t, k, r * 100 + j, True) for j, k in enumerate(keys))
              and h_put(t, (r + 1) * 0x100000 + 64 * 64, 0, False)
              and all(h_get_hit(t, k, r * 100 + j) for j, k in enumerate(keys))
              and all(h_del(t, k, True) for k in keys)
              and all(h_get_miss(t, k) for k in keys))
        if not ok:
            return False
    return True


def h_case_reinit():
    t = h_fresh("t")
    if not all(h_put(t, j * 64 + 3, j, True) for j in range(10)):
        return False
    R.op += 1
    R.step = "op %d: ht_init(t) on a table holding 10 entries" % R.op
    ht_init(t)
    return (all(h_get_miss(t, j * 64 + 3) for j in range(10))
            and all(h_put(t, 5000 + j, j, True) for j in range(H_CAP))
            and h_put(t, 5000 + H_CAP, 0, False))


def h_case_two_tables():
    a = h_fresh("a")
    b = h_fresh("b")
    return (h_put(a, 5, 1, True) and h_put(b, 5, 2, True) and h_put(b, 69, 3, True)
            and h_get_hit(a, 5, 1) and h_get_hit(b, 5, 2) and h_get_miss(a, 69)
            and h_del(a, 5, True) and h_get_miss(a, 5) and h_get_hit(b, 5, 2)
            and all(h_put(a, 3000 + j, j, True) for j in range(H_CAP))
            and h_put(b, 3000, 9, True) and h_get_hit(b, 3000, 9) and h_get_hit(a, 3000, 0))


# Long random workload checked against a shadow model of the map. The PRNG and
# key pool match the C harness, so both languages see the same operations.
def h_rand():
    x = R.rng
    x ^= (x << 13) & 0xFFFFFFFF
    x ^= x >> 17
    x ^= (x << 5) & 0xFFFFFFFF
    R.rng = x
    return x


def h_pool_key(i):
    if i < 32:
        return i * 64
    if i < 48:
        return i - 31
    if i < 64:
        return 0xFFFFFFFF - (i - 48) * 64
    if i < 80:
        return (i - 63) * 4096
    return (i - 79) << 24


def h_churn_fail(m):
    if not m["changed"]:
        note = "key %s was never stored" % h_key(m["key"])
    elif m["by_put"]:
        note = "key %s last put with value %d at op %d" % (h_key(m["key"]), m["val"], m["changed"])
    else:
        note = "key %s last deleted at op %d" % (h_key(m["key"]), m["changed"])
    R.got += " [model: %d live entries; %s]" % (R.live, note)
    return False


def h_case_churn():
    t = h_fresh("t")
    R.rng = 0x2545F491
    R.live = 0
    model = [{"key": h_pool_key(i), "val": 0, "live": False, "by_put": False, "changed": 0}
             for i in range(H_POOL)]
    for _ in range(H_CHURN_OPS):
        r = h_rand() % 100
        m = model[h_rand() % H_POOL]
        if r < 45:
            val = h_rand()
            want = m["live"] or R.live < H_CAP
            ok = h_put(t, m["key"], val, want)
            if ok and want:
                if not m["live"]:
                    R.live += 1
                m.update(val=val, live=True, by_put=True, changed=R.op)
        elif r < 75:
            ok = h_del(t, m["key"], m["live"])
            if ok and m["live"]:
                R.live -= 1
                m.update(val=0, live=False, by_put=False, changed=R.op)
        else:
            ok = h_get_hit(t, m["key"], m["val"]) if m["live"] else h_get_miss(t, m["key"])
        if not ok:
            return h_churn_fail(m)
    for m in model:
        ok = h_get_hit(t, m["key"], m["val"]) if m["live"] else h_get_miss(t, m["key"])
        if not ok:
            return h_churn_fail(m)
    return True


def h_case(name, body, inp, expected):
    return {"name": name, "args": [body, expected], "input": inp, "expected": expected}


CASES = [
    h_case("put and get a few keys", h_case_basic,
           "put (1,100) (2,200) (3,300) (1000000,4) (64,5); get each key",
           "every put returns True; every get returns that key's value"),
    h_case("get a missing key", h_case_get_missing,
           "empty table: get 42, get 0; put (1,10) (65,20); get 129, get 2, get 65",
           "gets of absent keys return None; get 65 returns 20"),
    h_case("update an existing key", h_case_update,
           "put (7,1) (7,2); get 7; put (7,3); get 7; put 63 new keys 100..162 (value j); "
           "put one more new key (999,9); get 7, get 999",
           "updates return True and take no extra room: the 63 new keys fit, 999 is rejected "
           "and absent, get 7 returns 3"),
    h_case("keys 0 and 0xFFFFFFFF", h_case_extreme_keys,
           "empty table: get 0, get 0xFFFFFFFF; put (0,0), get 0, get 0xFFFFFFFF; "
           "put (0xFFFFFFFF,4294967295), get both; del 0, get both; del 0xFFFFFFFF, get it",
           "0 and 0xFFFFFFFF behave like any other key: absent until put, present after, gone after del"),
    h_case("fill to HT_CAP", h_case_fill,
           "put 64 keys 0, 64, 128, ..., 4032 (key j*64, value j); put (4096,1); get 4096; "
           "put (64,777); get 64; put (4096,1) again; get all 64 keys",
           "all 64 puts succeed; key 4096 is rejected and absent; updating 64 on the full table succeeds"),
    h_case("delete then get", h_case_del_get,
           "put (10,1) (74,2) (138,3); del 74; get 74, get 10, get 138",
           "del 74 returns True; get 74 misses; get 10 and get 138 return 1 and 3"),
    h_case("delete a missing key", h_case_del_missing,
           "empty table: del 5; put (5,50); del 69, del 6; get 5",
           "deleting absent keys returns False and changes nothing; get 5 returns 50"),
    h_case("delete twice", h_case_del_twice,
           "put (42,1); del 42; del 42; get 42; put (42,2); get 42",
           "first del True, second del False, get misses, re-put works and get returns 2"),
    h_case("delete every other of 60 keys", h_case_del_every_other,
           "put 60 keys 0, 1, 64, 2, 128, 3, ... (key i is (i//2)*64 for even i, i//2+1 for odd i; "
           "value 1000+i); del the even-i keys 0, 64, 128, ...; get all 60",
           "the 30 odd-i keys still return 1000+i; the 30 deleted keys are absent"),
    h_case("re-put after earlier deletes", h_case_reput,
           "put the same 60 keys (value 1000+i); del keys i=0..29; put keys i=30..59 again with "
           "value 2000+i; get them; del each once; get each; del each again; "
           "then put 64 new keys 0x10000 + j*64 and one more",
           "re-puts update in place: gets return 2000+i, one del removes each key for good "
           "(no stale 1000+i copy), the table ends empty so 64 new keys fit and the 65th is rejected"),
    h_case("full table, delete one, insert new", h_case_full_del_insert,
           "put 64 keys 0, 1, 64, 2, ... (same pattern, value i); del 960 (i=30); put (6400,1); "
           "get 6400; get the other 63 keys; put (6464,2); get 960, get 6464",
           "the new key fits in the freed room and the rest are intact; the table is full again, "
           "so 6464 is rejected"),
    h_case("fill and empty 20 times", h_case_rounds,
           "20 rounds on one table: put 64 keys base + j*64 (base = 0x100000 * round number); "
           "put one more; get all 64; del all 64; get all 64",
           "every round: 64 keys fit, the 65th is rejected, all are found, all delete, all then miss"),
    h_case("init clears a used table", h_case_reinit,
           "put 10 keys 3, 67, 131, ..., 579; ht_init(t) again; get those 10; "
           "put 64 new keys 5000..5063 and one more (5064)",
           "after re-init the old keys are absent and all 64 slots are available"),
    h_case("two tables are independent", h_case_two_tables,
           "tables a and b: put a(5,1), b(5,2), b(69,3); get a 5, b 5, a 69; del a 5; get a 5, b 5; "
           "fill a with keys 3000..3063 (value j); put b(3000,9); get b 3000, a 3000",
           "each table keeps its own entries and its own capacity"),
    h_case("20000 random operations", h_case_churn,
           "seeded random put 45% / del 30% / get 25% over 96 keys that collide under simple hashes "
           "(multiples of 64 and 4096, 1..16, 0xFFFFFFFF downward in steps of 64, 0x1000000 multiples); "
           "puts of new keys on a full table expect False; then get all 96",
           "every result matches a reference model of the map"),
]

run_cases(h_run, CASES, show=str)
