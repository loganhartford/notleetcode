#include <cmath>

struct Vec2 { double x, y; };

bool pointInFOV(Vec2 observer, Vec2 heading, double half_angle_rad,
                double max_range, Vec2 target) {
    // TODO:
    // 1. Check if dist(observer, target) <= max_range
    // 2. Normalize heading and direction to target
    // 3. Check if dot product >= cos(half_angle_rad)
    return false;
}
