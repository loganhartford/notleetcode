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

static void check(const char* name, bool got, bool expected, const char* input) {
    nlc_case(name, (int)(got == expected), input,
             expected ? "true" : "false",
             got ? "true" : "false", "");
}

int main() {
    nlc_begin();

    // Test 1: no cars, 3 lanes → true
    check("no cars 3 lanes", canCross(3, {}, 5.0, 20), true, "3 lanes, no cars");

    // Test 2: immediate collision at t=1 in lane 0 forces wait
    // Car in lane 0 at t=1 blocks crossing_x=5.0, at t=2 it's moved away
    // Car: pos=3.0, v=0.0, len=4.0 → occupies [3,7] always. crossing_x=5 inside → blocked forever
    check("single lane blocked always", canCross(1, {{0, 3.0, 0.0, 4.0}}, 5.0, 20), false,
          "1 lane, car stationary blocking");

    // Test 3: must wait 1 tick then cross
    // Car in lane 0: pos=3.0, v=5.0, len=2.0
    // t=1: pos=8, span=[8,10], crossing_x=5 → NOT inside (5 < 8) → safe at t=1
    // Agent can advance at t=1, reach lane 0 at t=1 → no collision → then advance to safety
    check("can cross immediately", canCross(1, {{0, 3.0, 5.0, 2.0}}, 5.0, 10), true,
          "1 lane, car moves away quickly");

    // Test 4: car starts far but approaches, must time crossing
    // Lane 0 car: pos=-100, v=10.0, len=2.0
    // At t=10: pos=-100+100=0, span=[0,2], x=5 → not inside
    // At t=11: pos=10, span=[10,12], x=5 → not inside
    // Basically x=5 is inside when pos+0 <= 5 < pos+2, i.e., 3 <= pos < 5
    // pos+10t in [3,5): 10t in [3-pos, 5-pos) = [103, 105) → t in [10.3, 10.5)
    // At integer t=10: pos+10*10=-100+100=0, span=[0,2], x=5 not inside → safe at t=10
    // Actually let's just check result
    {
        std::vector<Car> cars = {{0, -100.0, 10.0, 2.0}};
        check("car approaches but window available", canCross(1, cars, 5.0, 50), true,
              "1 lane, approaching car");
    }

    // Test 5: negative velocity car moving away → safe to cross
    // Car at pos=10, v=-2, len=2. At t=1: pos=8, span=[8,10]. x=5 not inside → safe
    {
        std::vector<Car> cars = {{0, 10.0, -2.0, 2.0}};
        check("negative velocity car moves away", canCross(1, cars, 5.0, 10), true,
              "1 lane, car v=-2 moves away from x=5");
    }

    // Test 6: multiple cars per lane, one lane possible
    // Lane 0: car1 blocks t=1 (pos=3,v=0,len=4→x=5 inside), car2 blocks t=2, etc.
    // Car1: pos=3, v=0, len=4 → always blocks x=5 → impossible
    {
        std::vector<Car> cars = {{0, 3.0, 0.0, 4.0}, {0, 3.0, 0.0, 4.0}};
        check("multiple cars both blocking", canCross(1, cars, 5.0, 20), false,
              "2 cars in lane 0 both blocking x=5");
    }

    // Test 7: impossible continuous blockage, no path
    // 2 lanes, both always blocked
    {
        std::vector<Car> cars = {{0, 3.0, 0.0, 4.0}, {1, 3.0, 0.0, 4.0}};
        check("all lanes continuously blocked", canCross(2, cars, 5.0, 30), false,
              "2 lanes both stationary blocking x=5");
    }

    // Test 8: max_time cutoff prevents success
    // 1 lane: car blocks at all times t<=5, but can cross at t=6; max_time=3 → false
    // Car: pos=3, v=0, len=4 → always blocks. max_time=3 → false regardless
    {
        std::vector<Car> cars = {{0, 3.0, 0.0, 4.0}};
        check("max_time cutoff", canCross(1, cars, 5.0, 3), false,
              "blocked, max_time=3");
    }

    // Test 9: 0 lane_count → agent already at safety (no lanes to cross)
    // lane -1 is "before lane 0", advancing gets to lane 0...
    // Actually with lane_count=0, advancing from -1 to 0 = lane_count → safety immediately
    // At tick 1, agent advances to lane 0 = lane_count → true
    check("zero lanes to cross", canCross(0, {}, 5.0, 10), true, "0 lanes, no cars");

    nlc_end();
    return 0;
}
