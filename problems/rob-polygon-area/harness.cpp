#include <algorithm>
#include <climits>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <deque>
#include <functional>
#include <list>
#include <map>
#include <numeric>
#include <optional>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>
#include "nlc.h"
#include "solution.cpp"

static void chk(const char* name, double got, double expected, const char* input) {
    char gs[64], es[64];
    snprintf(gs, sizeof(gs), "%.6f", got);
    snprintf(es, sizeof(es), "%.6f", expected);
    nlc_case(name, fabs(got-expected) < 1e-6, input, es, gs, "");
}

int main() {
    nlc_begin();

    // Test 1: triangle with known area
    chk("right triangle", polygonArea({{0,0},{4,0},{0,3}}), 6.0, "(0,0),(4,0),(0,3)");

    // Test 2: unit square
    chk("unit square", polygonArea({{0,0},{1,0},{1,1},{0,1}}), 1.0, "unit square");

    // Test 3: rectangle 4x3
    chk("rectangle 4x3", polygonArea({{0,0},{4,0},{4,3},{0,3}}), 12.0, "4x3 rectangle");

    // Test 4: CCW vertices (same area)
    chk("CCW triangle", polygonArea({{0,0},{0,1},{1,0}}), 0.5, "(0,0),(0,1),(1,0) CCW");

    // Test 5: CW vertices
    chk("CW triangle", polygonArea({{0,0},{1,0},{0,1}}), 0.5, "(0,0),(1,0),(0,1) CW");

    // Test 6: concave polygon (L-shape)
    // L-shape: (0,0),(3,0),(3,1),(1,1),(1,2),(0,2) area = 3*1 + 1*1 = 4
    chk("concave L-shape", polygonArea({{0,0},{3,0},{3,1},{1,1},{1,2},{0,2}}), 4.0, "L-shape polygon");

    // Test 7: large integer coordinates
    chk("large coords square", polygonArea({{0,0},{1000,0},{1000,1000},{0,1000}}), 1000000.0, "1000x1000 square");

    nlc_end();
    return 0;
}
