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

    // Test 1: point on line → 0
    chk("point on line dist=0",
        pointToLineDistance({1,1,1},{0,0,0},{2,2,2}), 0.0, "P=(1,1,1) on line (0,0)-(2,2,2)");

    // Test 2: simple perpendicular (2D in xy plane)
    // Line along x-axis from (0,0,0) to (1,0,0). P=(0,3,0). dist=3
    chk("line distance perpendicular 3",
        pointToLineDistance({0,3,0},{0,0,0},{1,0,0}), 3.0, "P=(0,3,0) line x-axis");

    // Test 3: point on segment endpoint → 0
    chk("segment endpoint dist=0",
        pointToSegmentDistance({0,0,0},{0,0,0},{1,0,0}), 0.0, "P at segment start");

    // Test 4: point perpendicular to middle of segment
    // Segment (0,0,0)-(4,0,0), P=(2,3,0). t=0.5, closest=(2,0,0), dist=3
    chk("segment mid perpendicular",
        pointToSegmentDistance({2,3,0},{0,0,0},{4,0,0}), 3.0, "P=(2,3,0) perp to segment");

    // Test 5: degenerate A==B → dist(P,A)
    chk("degenerate A==B",
        pointToSegmentDistance({3,4,0},{1,1,0},{1,1,0}),
        std::sqrt(4.0+9.0), "A=B=(1,1,0) P=(3,4,0)");

    // Test 6: point beyond end of segment → dist to endpoint
    // Segment (0,0,0)-(1,0,0), P=(3,0,0). t=3 clamped to 1, closest=(1,0,0), dist=2
    chk("beyond segment end",
        pointToSegmentDistance({3,0,0},{0,0,0},{1,0,0}), 2.0, "P=(3,0,0) beyond (0,0)-(1,0)");

    // Test 7: 3D case with all non-zero components
    // Line A=(0,0,0) B=(1,1,1), P=(1,0,0)
    // d=(1,1,1), w=(1,0,0), cross=w×d=(0*1-0*1, 0*1-1*1, 1*1-0*1)=(0,-1,1), |cross|=sqrt(2)
    // |d|=sqrt(3), dist=sqrt(2)/sqrt(3)=sqrt(2/3)
    chk("3D non-zero components",
        pointToLineDistance({1,0,0},{0,0,0},{1,1,1}),
        std::sqrt(2.0/3.0), "P=(1,0,0) line (0,0,0)-(1,1,1)");

    // Test 8: point on segment (not endpoint) → 0
    chk("point on segment interior",
        pointToSegmentDistance({2,0,0},{0,0,0},{4,0,0}), 0.0, "P on segment interior");

    nlc_end();
    return 0;
}
