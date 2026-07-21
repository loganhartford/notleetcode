# AC Charger Load Balancing

A group of AC chargers are connected to one A/C grid in parallel. Each charger can
report whether a car is plugged in, and its current usage. We can set each charger's
current limit in real time. The grid has a **breaker limit** that must never be
exceeded — but we also want to deliver **as much current as the site can supply**.
Chargers are wireless and can drop offline at any moment.

```
                         AC GRID
                            │
                   ┌────────┴────────┐
                   │  400 A BREAKER  │
                   └────────┬────────┘
                            │
         ┌──────────────────┼──────────────────┐
    ┌────┼────┬────┬────┬────┬────┬────┬────┬──┴─┐
   [C1] [C2] [C3] [C4] [C5] [C6] [C7] [C8] [C9] [C10]
```

### The system (design context)

A **leader** task periodically polls every charger's status and pushes a fresh current
limit to each one. Each charger runs a **follower** task that enforces the limit it was
last told to. Because a charger can lose contact with the leader at any time, a robust
follower drops to a **safe fallback limit** when it hasn't heard from the leader — so an
offline charger is never left unlimited if someone plugs in.

### What you implement

Implement the pure allocation the leader computes each round:

```c
void balanceCurrent(int n, const bool *car_present, const bool *online,
                    int site_limit, int max_charger, int *limits);
```

- `n` — number of chargers
- `online[i]` — true if the leader could reach charger `i` this round
- `car_present[i]` — true if a car is plugged in at charger `i`. This comes from the
  charger's status reply, so it is only meaningful when `online[i]` is true. **When a
  charger is offline the status read failed and `car_present[i]` is left at its default
  `false` — it tells you nothing about whether a car is actually plugged in.**
- `site_limit` — total current the breaker allows, in whole amps
- `max_charger` — max current any single charger may draw, in whole amps
- `limits` — **output**: the current attributed to charger `i`. For a charger the leader
  can reach, this is the limit it commands; for an offline charger it is the fallback
  current the leader must assume that charger may draw.

### The safe fallback (offline chargers)

When a charger loses the leader it falls back to a fixed **safe minimum** — the current
it would get if *every* charger were online and sharing the site equally:

```
safe_min = site_limit / n          (whole amps, and never above max_charger)
```

The leader can't talk to an offline charger, and because the status read failed it can't
tell whether a car is plugged into it — `car_present` is just the default `false`, which
proves nothing. So the leader is **forced to assume the charger is charging**, but it also
knows the offline charger's follower has fallen back to `safe_min`, so it will draw *at
most* `safe_min`. For each offline charger, reserve `safe_min` from the site budget (and
report `safe_min` as that charger's draw). Whatever budget is left over is shared among
the chargers the leader *can* reach that have a car.

### Rules

- `safe_min = site_limit / n`, capped at `max_charger`.
- Every **offline** charger is assigned `safe_min` — regardless of `car_present`, since
  that field is unreliable when offline. We assume it is charging at the safe limit.
- An **online** charger with **no car** is assigned `0`.
- The remaining budget, `site_limit − (number offline) × safe_min`, is split among the
  **online chargers that have a car**, as evenly as possible, no charger above
  `max_charger`. When it doesn't divide evenly, the leftover amps go one at a time to the
  **lowest-indexed** eligible chargers.
- The sum of all assigned currents must never exceed `site_limit`.

### Example 1 — all reachable
```
n=10, site_limit=400, max_charger=50
car_present = all true,  online = all true
safe_min = 40, nothing offline -> full 400 A split 10 ways
Output: [40,40,40,40,40,40,40,40,40,40]
```

### Example 2 — one offline charger reserved
```
n=4, site_limit=120, max_charger=50
online      = [true, true, true, false]
car_present = [true, true, false, false]   (C3 offline -> car_present forced to false)
safe_min = 120/4 = 30
C3 is offline -> assume it's charging at safe_min, reserve 30 for it.
C2 is online with no car -> 0.
Remaining budget 120-30 = 90 split between C0 and C1 -> 45 each.
Output: [45, 45, 0, 30]     (sum 120, breaker exactly met, never exceeded)
```

### Example 3 — uneven split
```
n=4, site_limit=10, max_charger=50
car_present = all true,  online = all true
safe_min = 2, budget 10 split 4 ways -> the 2 leftover amps go to C0 and C1
Output: [3, 3, 2, 2]
```

### Constraints
- `0 <= n <= 64`; a sanely-provisioned site has `site_limit <= n * max_charger`.
- All currents are non-negative whole amps.
- If `online[i]` is false then `car_present[i]` is false (a failed status read leaves the
  default) — so you can never observe an offline charger reporting a car.
- If `n == 0`, there is nothing to do.

**Optimal complexity:** O(n) — compute `safe_min`, reserve for offline chargers, then
hand out the remaining budget to the eligible chargers with the remainder spread one amp
at a time.
