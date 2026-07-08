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

static void chk(const char* name, int got, int expected, const char* input) {
    nlc_case(name, got == expected, input,
             std::to_string(expected).c_str(), std::to_string(got).c_str(), "");
}

int main() {
    nlc_begin();

    // Test 1: basic get/put
    {
        LRUCache c(2);
        c.put(1, 10);
        chk("basic get", c.get(1), 10, "put(1,10); get(1)");
    }

    // Test 2: update existing key value
    {
        LRUCache c(2);
        c.put(1, 10);
        c.put(1, 20);
        chk("update existing key", c.get(1), 20, "put(1,10); put(1,20); get(1)");
    }

    // Test 3: capacity 1 eviction
    {
        LRUCache c(1);
        c.put(1, 1);
        c.put(2, 2);
        chk("cap1 evict: key1 gone", c.get(1), -1, "cap=1; put(1,1);put(2,2);get(1)");
    }

    // Test 4: eviction order (LRU is evicted)
    {
        LRUCache c(2);
        c.put(1, 1); c.put(2, 2);
        c.put(3, 3); // evicts 1 (LRU)
        chk("eviction order: key1 evicted", c.get(1), -1, "put(1,2,3); get(1)=-1");
        chk("eviction order: key2 remains", c.get(2), 2, "put(1,2,3); get(2)=2");
    }

    // Test 5: get updates recency
    {
        LRUCache c(2);
        c.put(1, 1); c.put(2, 2);
        c.get(1);    // 1 becomes MRU
        c.put(3, 3); // evicts 2 (now LRU)
        chk("get updates recency: key2 evicted", c.get(2), -1, "get(1) then put(3) evicts 2");
        chk("get updates recency: key1 survives", c.get(1), 1, "key1 still present");
    }

    // Test 6: put updates recency
    {
        LRUCache c(2);
        c.put(1, 1); c.put(2, 2);
        c.put(1, 10); // update 1, makes it MRU
        c.put(3, 3);  // evicts 2
        chk("put updates recency: key2 evicted", c.get(2), -1, "put(1,10) makes 1 MRU; put(3) evicts 2");
        chk("put updates recency: key1 has new val", c.get(1), 10, "key1=10");
    }

    // Test 7: capacity 2 scenario
    {
        LRUCache c(2);
        c.put(2, 1); c.put(1, 1);
        c.put(2, 3); // update 2 → MRU
        c.put(4, 1); // evicts 1 (LRU)
        chk("cap2: key1 evicted after put(2,3)", c.get(1), -1, "cap2 scenario");
        chk("cap2: key2 survives", c.get(2), 3, "key2=3");
    }

    // Test 8: get missing key returns -1
    {
        LRUCache c(3);
        chk("get missing key", c.get(42), -1, "empty cache get(42)");
    }

    nlc_end();
    return 0;
}
