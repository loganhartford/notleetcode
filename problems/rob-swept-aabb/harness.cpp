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
    bool ok;
    if (expected < 0) {
        ok = got < 0;
        snprintf(es, sizeof(es), "-1 (no collision)");
        snprintf(gs, sizeof(gs), "%.6f", got);
    } else {
        ok = fabs(got - expected) < 1e-6;
        snprintf(es, sizeof(es), "%.6f", expected);
        snprintf(gs, sizeof(gs), "%.6f", got);
    }
    nlc_case(name, ok, input, es, gs, "");
}

int main() {
    nlc_begin();

    // Test 1: already overlapping at t=0 → returns 0.0
    // a=[0,0,2,2] b=[1,1,3,3] overlap at t=0
    {
        AABB a{0,0,2,2,0,0}, b{1,1,3,3,0,0};
        chk("already overlapping t=0", sweptAABBCollision(a,b,10.0), 0.0, "overlapping at t=0");
    }

    // Test 2: never collide (moving apart)
    // a=[0,0,1,1] vx=0; b=[3,0,4,1] vx=1 (moving right away)
    {
        AABB a{0,0,1,1,0,0}, b{3,0,4,1,1,0};
        chk("moving apart never collide", sweptAABBCollision(a,b,10.0), -1.0, "b moving away from a");
    }

    // Test 3: head-on collision at t=1
    // a=[0,0,1,1] vx=1; b=[3,0,4,1] vx=-1. Relative v of b w.r.t. a = -2.
    // Gap on x: a.max=1, b.min=3. Entry: (1-4)/(-2)=1.5, exit: (1-3)/(-2)...
    // Actually: axisCollision(a.min_x=0,a.max_x=1, b.min_x=3,b.max_x=4, rv=-2)
    // t1=(0-4)/(-2)=2, t2=(1-3)/(-2)=1. Swap: entry=1, exit=2
    // axisCollision y: same vel, already overlapping → entry=-inf, exit=inf
    // global entry=1, exit=2. 1<=2, 1<=10, 2>=0 → collision at t=1
    {
        AABB a{0,0,1,1,1,0}, b{3,0,4,1,-1,0};
        chk("head-on collision t=1", sweptAABBCollision(a,b,10.0), 1.0, "head-on a vx=1 b vx=-1");
    }

    // Test 4: collision exactly at T
    // Same as test 3 but T=1
    {
        AABB a{0,0,1,1,1,0}, b{3,0,4,1,-1,0};
        chk("collision exactly at T=1", sweptAABBCollision(a,b,1.0), 1.0, "T=1, collision at T");
    }

    // Test 5: same velocity, boxes not overlapping → no collision
    {
        AABB a{0,0,1,1,2,0}, b{5,0,6,1,2,0};
        chk("same velocity no collision", sweptAABBCollision(a,b,10.0), -1.0, "same vx=2, separated");
    }

    // Test 6: one stationary, one moving toward it
    // a=[0,0,2,2] stationary; b=[4,0,5,2] moving left at vx=-1
    // rv = b.vx - a.vx = -1.
    // axisX: a=(0,2), b=(4,5), rv=-1. t1=(0-5)/(-1)=5, t2=(2-4)/(-1)=2. entry=2, exit=5
    // axisY: already overlap (a=[0,2] b=[0,2]) → entry=-inf, exit=inf
    // global entry=2, exit=5 → collision at t=2
    {
        AABB a{0,0,2,2,0,0}, b{4,0,5,2,-1,0};
        chk("stationary vs approaching", sweptAABBCollision(a,b,10.0), 2.0, "b moves left toward a");
    }

    // Test 7: zero relative velocity on y, collision on x only
    // Both boxes share y range but separated on x
    {
        AABB a{0,0,1,1,1,0}, b{5,0,6,1,-1,0};
        // rv_x = -2, axisX: t1=(0-6)/(-2)=3, t2=(1-5)/(-2)=2, entry=2, exit=3
        // rv_y = 0, y overlap [0,1] vs [0,1] → entry=-inf, exit=inf
        // global: entry=2, exit=3 → collision at t=2
        chk("zero rv on y collision on x", sweptAABBCollision(a,b,10.0), 2.0, "zero rel v on y");
    }

    // Test 8: collision would happen at t=5 but T=3 → -1
    {
        AABB a{0,0,1,1,0,0}, b{7,0,8,1,-1,0};
        // axisX: rv=-1. t1=(0-8)/(-1)=8, t2=(1-7)/(-1)=6. entry=6, exit=8. 6>T=3 → -1
        chk("collision after T cutoff", sweptAABBCollision(a,b,3.0), -1.0, "collision at t=6 T=3");
    }

    nlc_end();
    return 0;
}
