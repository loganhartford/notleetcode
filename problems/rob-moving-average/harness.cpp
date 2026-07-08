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

static void check(const char* name, double got, double expected, const char* input) {
    char got_s[64], exp_s[64];
    snprintf(got_s, sizeof(got_s), "%.6f", got);
    snprintf(exp_s, sizeof(exp_s), "%.6f", expected);
    int passed = fabs(got - expected) < 1e-5;
    nlc_case(name, passed, input, exp_s, got_s, "");
}

int main() {
    nlc_begin();

    // Test 1: fewer than k values
    {
        MovingAverage ma(3);
        double r = ma.next(1);
        check("fewer than k: first value", r, 1.0, "next(1), k=3");
    }

    // Test 2: exactly k values
    {
        MovingAverage ma(3);
        ma.next(1); ma.next(10);
        double r = ma.next(3);
        check("exactly k values", r, 14.0/3.0, "next(1),next(10),next(3), k=3");
    }

    // Test 3: window slides past k
    {
        MovingAverage ma(3);
        ma.next(1); ma.next(10); ma.next(3);
        double r = ma.next(5);
        check("window slides: 4th value", r, 6.0, "next(1,10,3,5) k=3 → [10,3,5]");
    }

    // Test 4: negative values
    {
        MovingAverage ma(2);
        ma.next(-5);
        double r = ma.next(-3);
        check("negative values", r, -4.0, "next(-5),next(-3) k=2");
    }

    // Test 5: large values (int64_t accumulation)
    {
        MovingAverage ma(2);
        ma.next(2000000000);
        double r = ma.next(2000000000);
        check("large values int64_t", r, 2000000000.0, "next(2e9),next(2e9) k=2");
    }

    // Test 6: window size 1
    {
        MovingAverage ma(1);
        ma.next(42);
        double r = ma.next(99);
        check("window size 1", r, 99.0, "next(42),next(99) k=1");
    }

    // Test 7: window size equals all samples
    {
        MovingAverage ma(5);
        ma.next(1); ma.next(2); ma.next(3); ma.next(4);
        double r = ma.next(5);
        check("window equals all samples", r, 3.0, "next(1..5) k=5");
    }

    // Test 8: running average sequence
    {
        MovingAverage ma(3);
        double r1 = ma.next(4);
        double r2 = ma.next(0);
        double r3 = ma.next(2);
        // window: [4,0,2] avg=2.0
        int ok = fabs(r1 - 4.0) < 1e-5 && fabs(r2 - 2.0) < 1e-5 && fabs(r3 - 2.0) < 1e-5;
        nlc_case("running average sequence", ok,
                 "next(4)=4.0, next(0)=2.0, next(2)=2.0",
                 "4.000000, 2.000000, 2.000000",
                 ok ? "4.000000, 2.000000, 2.000000" : "mismatch", "");
    }

    nlc_end();
    return 0;
}
