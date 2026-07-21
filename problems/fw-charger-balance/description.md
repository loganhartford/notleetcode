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
follower drops to a safe fail-safe limit if it hasn't heard from the leader recently —
which is what lets the leader safely divide the **whole** site budget among only the
chargers it can currently reach.

### What you implement

Implement the pure allocation the leader computes each round:

```c
void balanceCurrent(int n, const bool *car_present, const bool *online,
                    int site_limit, int max_charger, int *limits);
```

- `n` — number of chargers
- `car_present[i]` — true if a car is plugged in at charger `i`
- `online[i]` — true if the leader can currently reach charger `i`
- `site_limit` — total current the breaker allows, in whole amps
- `max_charger` — max current any single charger may draw, in whole amps
- `limits` — **output**: `limits[i]` is the current to command charger `i`

### Rules

- A charger is **eligible** only if it is `online` **and** has a car. Every other
  charger must be assigned `0`.
- No charger may be assigned more than `max_charger`.
- The sum of all assigned limits must never exceed `site_limit`.
- Deliver as much of `site_limit` as possible to the eligible chargers, shared **as
  evenly as possible**. Work in whole amps; when the share doesn't divide evenly, hand
  out the leftover amps one at a time to the **lowest-indexed** eligible chargers.

### Example 1
```
n=10, site_limit=400, max_charger=50
car_present = all true,  online = all true
Output: [40,40,40,40,40,40,40,40,40,40]   (400 split 10 ways)
```

### Example 2
```
n=4, site_limit=200, max_charger=50
car_present = [true,true,true,true],  online = [true,false,true,false]
Output: [50,0,50,0]   (only C0 and C2 are eligible; 100 A used, breaker never hit)
```

### Example 3
```
n=4, site_limit=10, max_charger=50
car_present = all true,  online = all true
Output: [3,3,2,2]   (10 A split 4 ways; 2 leftover amps go to C0 and C1)
```

### Constraints
- `0 <= n <= 64`, all currents are non-negative whole amps.
- If no charger is eligible, every limit is `0`.

**Optimal complexity:** O(n) — count eligible chargers, then hand out `site_limit / k`
to each with the remainder distributed one amp at a time.
