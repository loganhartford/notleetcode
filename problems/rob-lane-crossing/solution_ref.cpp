#include <vector>
#include <queue>
#include <set>

struct Car {
    int lane;
    double position;
    double velocity;
    double length;
};

// Returns true if crossing_x is strictly inside the car's span at tick t
static bool carBlocks(const Car& car, double crossing_x, int t) {
    double front = car.position + car.velocity * t;
    double back = front + car.length;
    double lo = std::min(front, back);
    double hi = std::max(front, back);
    return crossing_x > lo && crossing_x < hi;
}

bool canCross(int lane_count, std::vector<Car> cars, double crossing_x, int max_time) {
    // State: (tick, lane) where lane=-1 means before lane 0
    // BFS
    std::set<std::pair<int,int>> visited;
    std::queue<std::pair<int,int>> q;  // {tick, lane}
    q.push({0, -1});
    visited.insert({0, -1});

    while (!q.empty()) {
        auto [t, lane] = q.front(); q.pop();

        if (t >= max_time) continue;

        int nt = t + 1;
        // Option 1: wait (stay in lane)
        // Option 2: advance to lane+1
        for (int nl : {lane, lane + 1}) {
            if (nl > lane_count) continue; // already past, shouldn't happen
            if (nl == lane_count) {
                // Reached safety
                return true;
            }
            if (visited.count({nt, nl})) continue;

            // Check if agent would be in a real lane (not -1)
            bool collision = false;
            if (nl >= 0) {
                for (const auto& car : cars) {
                    if (car.lane == nl && carBlocks(car, crossing_x, nt)) {
                        collision = true;
                        break;
                    }
                }
            }
            if (!collision) {
                visited.insert({nt, nl});
                q.push({nt, nl});
            }
        }
    }
    return false;
}
