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

static void chk(const char* name, bool got, bool expected, const char* input) {
    nlc_case(name, (int)(got==expected), input,
             expected?"true":"false", got?"true":"false", "");
}

int main() {
    nlc_begin();

    const double PI = M_PI;
    Vec2 origin{0,0};

    // Test 1: target at observer → inside (distance=0)
    chk("target at observer", pointInFOV(origin,{1,0},PI/4.0,10.0,{0,0}), true,
        "target=observer");

    // Test 2: target directly ahead → inside
    // heading (1,0), half_angle=30°, target (5,0)
    chk("directly ahead", pointInFOV(origin,{1,0},PI/4.0,10.0,{5,0}), true,
        "target ahead on heading axis");

    // Test 3: target directly behind → outside
    chk("directly behind", pointInFOV(origin,{1,0},PI/4.0,10.0,{-3,0}), false,
        "target behind observer");

    // Test 4: target at 30° inside half_angle=45° → true
    // heading (1,0), target at (cos30°, sin30°) = (0.866, 0.5), angle from heading = 30° < 45°
    chk("30deg inside 45deg fov", pointInFOV(origin,{1,0},PI/4.0,10.0,{0.866,0.5}), true,
        "target at 30deg, half_angle=45deg");

    // Test 5: target exactly at max_range → inside (<= comparison)
    chk("exactly at max_range", pointInFOV(origin,{1,0},PI/4.0,5.0,{5,0}), true,
        "target at dist=max_range=5");

    // Test 6: target beyond max_range → outside
    chk("beyond max_range", pointInFOV(origin,{1,0},PI/4.0,5.0,{6,0}), false,
        "target at dist=6 max_range=5");

    // Test 7: non-normalized heading still works
    // heading (3,0) normalized is (1,0), same as test 2
    chk("non-normalized heading", pointInFOV(origin,{3,0},PI/4.0,10.0,{5,0}), true,
        "heading=(3,0) non-normalized");

    // Test 8: target just outside angle
    // heading (1,0), half=30° ≈ 0.5236. cos(30°)≈0.866. Target at 45°: cos=1/√2≈0.707 < 0.866
    chk("outside angle slightly", pointInFOV(origin,{1,0},PI/6.0,10.0,{1,1}), false,
        "target at 45deg, half_angle=30deg");

    nlc_end();
    return 0;
}
