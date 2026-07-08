#include <cmath>

struct Vec3 { double x, y, z; };

double pointToLineDistance(Vec3 P, Vec3 A, Vec3 B) {
    // TODO: dist = |cross(P-A, B-A)| / |B-A|
    return 0.0;
}

double pointToSegmentDistance(Vec3 P, Vec3 A, Vec3 B) {
    // TODO: project P onto AB; clamp t to [0,1]; return dist to closest point
    return 0.0;
}
