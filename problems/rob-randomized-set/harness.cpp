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

int main() {
    nlc_begin();

    // Test 1: insert new → true
    {
        RandomizedSet rs;
        bool r = rs.insert(1);
        nlc_case("insert new returns true", r, "insert(1) into empty set", "true", r?"true":"false", "");
    }

    // Test 2: insert duplicate → false
    {
        RandomizedSet rs;
        rs.insert(1);
        bool r = rs.insert(1);
        nlc_case("insert duplicate returns false", !r, "insert(1) twice", "false", r?"true":"false", "");
    }

    // Test 3: remove missing → false
    {
        RandomizedSet rs;
        bool r = rs.remove(42);
        nlc_case("remove missing returns false", !r, "remove(42) from empty set", "false", r?"true":"false", "");
    }

    // Test 4: remove present → true
    {
        RandomizedSet rs;
        rs.insert(5);
        bool r = rs.remove(5);
        nlc_case("remove present returns true", r, "insert(5); remove(5)", "true", r?"true":"false", "");
    }

    // Test 5: getRandom with 1 element always returns that element
    {
        RandomizedSet rs;
        rs.insert(99);
        bool ok = true;
        for (int i = 0; i < 10; i++) {
            if (rs.getRandom() != 99) { ok = false; break; }
        }
        nlc_case("getRandom single element", ok, "10x getRandom with only {99}", "99 each time",
                 ok?"99 (correct)":"wrong value", "");
    }

    // Test 6: getRandom result always in set
    {
        RandomizedSet rs;
        rs.insert(1); rs.insert(2); rs.insert(3);
        std::set<int> allowed = {1,2,3};
        bool ok = true;
        for (int i = 0; i < 30; i++) {
            int v = rs.getRandom();
            if (!allowed.count(v)) { ok = false; break; }
        }
        nlc_case("getRandom always in set", ok, "30x getRandom from {1,2,3}", "element in {1,2,3}",
                 ok?"all in set":"out of range", "");
    }

    // Test 7: remove middle element (swap-delete correctness)
    {
        RandomizedSet rs;
        rs.insert(1); rs.insert(2); rs.insert(3);
        rs.remove(2); // 2 is "middle", should swap with 3
        bool r1 = rs.remove(1); // should succeed
        bool r3 = rs.remove(3); // should succeed
        bool r2 = rs.remove(2); // already removed → false
        bool ok = r1 && r3 && !r2;
        nlc_case("remove middle swap-delete", ok,
                 "insert(1,2,3); remove(2); remove(1,3,2)",
                 "true,true,false", ok?"correct":"wrong", "");
    }

    // Test 8: operations after many mutations
    {
        RandomizedSet rs;
        for (int i = 0; i < 20; i++) rs.insert(i);
        for (int i = 0; i < 10; i++) rs.remove(i*2); // remove evens
        // Set should have {1,3,5,7,9,11,13,15,17,19}
        std::set<int> expected;
        for (int i = 0; i < 10; i++) expected.insert(2*i+1);
        bool ok = true;
        for (int t = 0; t < 50; t++) {
            int v = rs.getRandom();
            if (!expected.count(v)) { ok = false; break; }
        }
        nlc_case("many mutations getRandom", ok,
                 "insert 0-19, remove evens, getRandom 50x",
                 "always odd in [1,19]", ok?"all valid":"invalid element", "");
    }

    nlc_end();
    return 0;
}
