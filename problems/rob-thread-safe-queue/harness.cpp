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
#include <thread>
#include <optional>
#include "nlc.h"
#include "solution.cpp"

int main() {
    nlc_begin();

    // Test 1: single-threaded FIFO order
    {
        BoundedQueue<int> q(5);
        q.push(1); q.push(2); q.push(3);
        int a=q.pop(), b=q.pop(), c=q.pop();
        bool ok = a==1 && b==2 && c==3;
        nlc_case("single-thread FIFO", ok, "push(1,2,3); pop x3", "1,2,3",
                 (std::to_string(a)+","+std::to_string(b)+","+std::to_string(c)).c_str(), "");
    }

    // Test 2: try_push returns false when full
    {
        BoundedQueue<int> q(2);
        q.try_push(1); q.try_push(2);
        bool r = q.try_push(3);
        nlc_case("try_push full returns false", !r, "cap=2; push 1,2; try_push 3", "false",
                 r?"true":"false", "");
    }

    // Test 3: try_pop returns nullopt when empty
    {
        BoundedQueue<int> q(3);
        auto r = q.try_pop();
        nlc_case("try_pop empty returns nullopt", !r.has_value(), "empty queue try_pop",
                 "nullopt", r.has_value()?"value":"nullopt", "");
    }

    // Test 4: size() accuracy after push/pop
    {
        BoundedQueue<int> q(10);
        q.push(1); q.push(2); q.push(3);
        size_t s1 = q.size();
        q.pop();
        size_t s2 = q.size();
        bool ok = s1==3 && s2==2;
        nlc_case("size accuracy", ok, "push x3, size=3, pop, size=2",
                 "3 then 2",
                 (std::to_string(s1)+" then "+std::to_string(s2)).c_str(), "");
    }

    // Test 5: capacity 1 behavior
    {
        BoundedQueue<int> q(1);
        q.push(42);
        bool full = !q.try_push(99);
        int v = q.pop();
        bool empty = !q.try_pop().has_value();
        bool ok = full && v==42 && empty;
        nlc_case("capacity 1", ok, "cap=1; push(42); try_push(99)=false; pop=42",
                 "true,42,empty", ok?"correct":"wrong", "");
    }

    // Test 6: multi-threaded producer/consumer
    {
        const int N = 100;
        BoundedQueue<int> q(10);
        std::vector<int> received;
        std::mutex recv_mtx;

        std::thread producer([&](){
            for (int i = 0; i < N; i++) q.push(i);
        });
        std::thread consumer([&](){
            for (int i = 0; i < N; i++) {
                int v = q.pop();
                std::lock_guard<std::mutex> lock(recv_mtx);
                received.push_back(v);
            }
        });
        producer.join();
        consumer.join();

        // All N items received, sum correct
        int sum = 0;
        for (int v : received) sum += v;
        int expected_sum = N*(N-1)/2;
        bool ok = (int)received.size()==N && sum==expected_sum;
        nlc_case("multi-threaded producer-consumer", ok,
                 "1 producer pushes 100 items, 1 consumer pops 100",
                 "100 items, correct sum",
                 ok ? "correct" : ("size="+std::to_string(received.size())+" sum="+std::to_string(sum)).c_str(),
                 "");
    }

    // Test 7: try_push then pop (non-blocking round-trip)
    {
        BoundedQueue<int> q(5);
        bool ok1 = q.try_push(77);
        auto v = q.try_pop();
        bool ok = ok1 && v.has_value() && v.value()==77;
        nlc_case("try_push then try_pop", ok, "try_push(77); try_pop()", "true, 77",
                 ok?"correct":"wrong", "");
    }

    nlc_end();
    return 0;
}
