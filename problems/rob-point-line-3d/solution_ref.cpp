#include <cmath>

struct Vec3 { double x, y, z; };

static Vec3 sub(Vec3 a, Vec3 b) { return {a.x-b.x, a.y-b.y, a.z-b.z}; }
static Vec3 cross(Vec3 a, Vec3 b) {
    return {a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x};
}
static double dot(Vec3 a, Vec3 b) { return a.x*b.x + a.y*b.y + a.z*b.z; }
static double len(Vec3 a) { return std::sqrt(dot(a,a)); }

double pointToLineDistance(Vec3 P, Vec3 A, Vec3 B) {
    Vec3 d = sub(B, A);
    Vec3 w = sub(P, A);
    Vec3 c = cross(w, d);
    double dlen = len(d);
    if (dlen < 1e-12) return len(w);
    return len(c) / dlen;
}

double pointToSegmentDistance(Vec3 P, Vec3 A, Vec3 B) {
    Vec3 d = sub(B, A);
    Vec3 w = sub(P, A);
    double dd = dot(d, d);
    if (dd < 1e-12) return len(w); // degenerate: A==B
    double t = dot(w, d) / dd;
    if (t < 0.0) t = 0.0;
    if (t > 1.0) t = 1.0;
    Vec3 closest = {A.x + t*d.x, A.y + t*d.y, A.z + t*d.z};
    Vec3 diff = sub(P, closest);
    return len(diff);
}
