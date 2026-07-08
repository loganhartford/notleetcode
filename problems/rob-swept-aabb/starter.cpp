#include <algorithm>
#include <limits>

struct AABB {
    double min_x, min_y;
    double max_x, max_y;
    double vx, vy;
};

double sweptAABBCollision(AABB a, AABB b, double T) {
    // TODO: compute relative velocity, per-axis entry/exit times
    // Global entry = max(entryX, entryY), exit = min(exitX, exitY)
    // Return earliest collision time in [0,T] or -1.0
    return -1.0;
}
