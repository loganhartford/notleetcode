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

static std::string fmtVec(const std::vector<int>& v) {
    std::string s = "[";
    for (int i=0;i<(int)v.size();i++){if(i)s+=", "; s+=std::to_string(v[i]);}
    return s+"]";
}

static ListNode* makeList(std::initializer_list<int> vals) {
    ListNode* head = nullptr;
    ListNode** cur = &head;
    for (int v : vals) { *cur = new ListNode(v); cur = &((*cur)->next); }
    return head;
}

int main() {
    nlc_begin();

    // Test 1: null head → length 0
    {
        int got = listLength(nullptr);
        nlc_case("null head length=0", got==0, "null", "0", std::to_string(got).c_str(), "");
    }

    // Test 2: null head reverseValues → empty
    {
        auto got = reverseValues(nullptr);
        nlc_case("null head reverse=[]", got.empty(), "null", "[]",
                 got.empty()?"[]":"non-empty", "");
    }

    // Test 3: single node length
    {
        auto* head = makeList({42});
        int got = listLength(head);
        nlc_case("single node length=1", got==1, "[42]", "1", std::to_string(got).c_str(), "");
    }

    // Test 4: two nodes
    {
        auto* head = makeList({1,2});
        int got = listLength(head);
        nlc_case("two nodes length=2", got==2, "[1,2]", "2", std::to_string(got).c_str(), "");
    }

    // Test 5: length 5
    {
        auto* head = makeList({1,2,3,4,5});
        int got = listLength(head);
        nlc_case("list length 5", got==5, "[1,2,3,4,5]", "5", std::to_string(got).c_str(), "");
    }

    // Test 6: reverse [1,2,3,4,5] → [5,4,3,2,1]
    {
        auto* head = makeList({1,2,3,4,5});
        auto got = reverseValues(head);
        std::vector<int> exp = {5,4,3,2,1};
        std::string gs = fmtVec(got), es = fmtVec(exp);
        nlc_case("reverse [1,2,3,4,5]", got==exp, "[1,2,3,4,5]", es.c_str(), gs.c_str(), "");
    }

    // Test 7: single node reverse → [val]
    {
        auto* head = makeList({99});
        auto got = reverseValues(head);
        std::vector<int> exp = {99};
        std::string gs = fmtVec(got), es = fmtVec(exp);
        nlc_case("single node reverse", got==exp, "[99]", es.c_str(), gs.c_str(), "");
    }

    nlc_end();
    return 0;
}
