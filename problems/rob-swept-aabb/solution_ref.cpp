#include <algorithm>
#include <limits>

struct AABB {
    double min_x, min_y;
    double max_x, max_y;
    double vx, vy;
};

static void axisCollision(double aMin, double aMax, double bMin, double bMax,
                          double rv, double& entry, double& exit) {
    double INF = std::numeric_limits<double>::infinity();
    if (std::fabs(rv) < 1e-12) {
        // No relative motion on this axis
        if (bMax < aMin || aMax < bMin) {
            entry = INF; exit = -INF; // no collision possible
        } else {
            entry = -INF; exit = INF; // always overlapping
        }
    } else {
        double t1 = (aMin - bMax) / rv;
        double t2 = (aMax - bMin) / rv;
        if (t1 > t2) std::swap(t1, t2);
        entry = t1;
        exit = t2;
    }
}

double sweptAABBCollision(AABB a, AABB b, double T) {
    double rvx = b.vx - a.vx;
    double rvy = b.vy - a.vy;

    double entryX, exitX, entryY, exitY;
    axisCollision(a.min_x, a.max_x, b.min_x, b.max_x, rvx, entryX, exitX);
    axisCollision(a.min_y, a.max_y, b.min_y, b.max_y, rvy, entryY, exitY);

    double entry = std::max(entryX, entryY);
    double exitT = std::min(exitX, exitY);

    if (entry > exitT) return -1.0;  // no overlap
    if (entry > T) return -1.0;       // collision after window
    if (exitT < 0.0) return -1.0;    // collision before window

    double t = std::max(entry, 0.0);
    return t;
}
