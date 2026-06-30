// Time: O(n) linear scan through points
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

bool interp_i32(const point_t *pts, size_t n, int32_t x, int32_t *y_out) {
    if (n == 0) return false;
    if (x < pts[0].x) return false;
    if (x > pts[n-1].x) return false;

    for (size_t i = 0; i < n - 1; i++) {
        if (x == pts[i].x) {
            *y_out = pts[i].y;
            return true;
        }
        if (x < pts[i+1].x) {
            int32_t x0 = pts[i].x, y0 = pts[i].y;
            int32_t x1 = pts[i+1].x, y1 = pts[i+1].y;
            *y_out = (int32_t)(y0 + (int64_t)(y1 - y0) * (x - x0) / (x1 - x0));
            return true;
        }
    }
    /* x == pts[n-1].x */
    *y_out = pts[n-1].y;
    return true;
}
