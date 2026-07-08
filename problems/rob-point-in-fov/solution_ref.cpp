#include <cmath>

struct Vec2 { double x, y; };

bool pointInFOV(Vec2 observer, Vec2 heading, double half_angle_rad,
                double max_range, Vec2 target) {
    double dx = target.x - observer.x;
    double dy = target.y - observer.y;
    double dist = std::sqrt(dx*dx + dy*dy);

    if (dist > max_range) return false;
    if (dist < 1e-12) return true; // at observer position

    // Normalize heading
    double hlen = std::sqrt(heading.x*heading.x + heading.y*heading.y);
    double hx = heading.x / hlen, hy = heading.y / hlen;

    // Normalize direction to target
    double tx = dx / dist, ty = dy / dist;

    double cosAngle = hx*tx + hy*ty;
    double cosHalf = std::cos(half_angle_rad);

    return cosAngle >= cosHalf;
}
