# Lane Crossing Simulation

Simulate an autonomous agent crossing a road with `N` lanes. Each tick, the agent may **wait** (stay in its current position) or **advance** one lane forward.

The agent starts **before lane 0** (position -1) and must reach **safety** (position N, past lane N-1). A collision occurs if the agent occupies a lane while a car covers the crossing point at the same tick.

```cpp
struct Car {
    int lane;
    double position; // position at t=0
    double velocity; // units per tick (negative = moving backward)
    double length;   // car length in position units
};

// Returns true if the agent can cross all N lanes without collision
// max_time: abort search if tick exceeds this value
bool canCross(int lane_count, vector<Car> cars, double crossing_x, int max_time);
```

At tick `t`, a car occupies the interval `[lo, hi]` where:
- `lo = min(pos + v*t, pos + v*t + length)`
- `hi = max(pos + v*t, pos + v*t + length)`

The agent at lane `L` at time `t` collides if `crossing_x` falls **strictly inside** `(lo, hi)` for any car in lane `L` (touching endpoints is safe).

Use BFS/DP over states `(tick, current_lane)`. The agent starts at `current_lane = -1` (before lane 0) at `tick = 0`.

## Constraints

- `1 <= lane_count <= 10`
- `0 <= cars.size() <= 20`
- `1 <= max_time <= 200`

**Optimal complexity:** O(max_time * lane_count) — BFS over time-lane states.
