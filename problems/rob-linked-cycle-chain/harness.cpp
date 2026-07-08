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
    nlc_case(name, got==expected, input,
             std::to_string(expected).c_str(), std::to_string(got).c_str(), "");
}

int main() {
    nlc_begin();

    // Test 1: null → 0
    chk("null head", longestNonRepeatingChain(nullptr), 0, "null");

    // Test 2: 3-node chain, no cycle → 3
    {
        Node* a = new Node(1);
        Node* b = new Node(2);
        Node* c = new Node(3);
        a->next=b; b->next=c;
        chk("3-node chain no cycle", longestNonRepeatingChain(a), 3, "1→2→3→null");
        delete a; delete b; delete c;
    }

    // Test 3: self-cycle: 1 node pointing to itself → 1
    {
        Node* a = new Node(1);
        a->next = a;
        chk("self-cycle", longestNonRepeatingChain(a), 1, "1→self");
        a->next = nullptr; delete a; // break cycle before delete
    }

    // Test 4: 3-node pure cycle → 3
    {
        Node* a = new Node(1);
        Node* b = new Node(2);
        Node* c = new Node(3);
        a->next=b; b->next=c; c->next=a;
        chk("3-node pure cycle", longestNonRepeatingChain(a), 3, "1→2→3→1");
        c->next=nullptr; // break cycle
        delete a; delete b; delete c;
    }

    // Test 5: 2-node tail + 3-node cycle → 5
    // 1→2→3→4→5→back to 3
    {
        Node* n1 = new Node(1);
        Node* n2 = new Node(2);
        Node* n3 = new Node(3);
        Node* n4 = new Node(4);
        Node* n5 = new Node(5);
        n1->next=n2; n2->next=n3; n3->next=n4; n4->next=n5; n5->next=n3;
        chk("tail+cycle 5 unique", longestNonRepeatingChain(n1), 5, "1→2→3→4→5→3");
        n5->next=nullptr;
        delete n1; delete n2; delete n3; delete n4; delete n5;
    }

    // Test 6: single node, no next → 1
    {
        Node* a = new Node(42);
        chk("single node no cycle", longestNonRepeatingChain(a), 1, "42→null");
        delete a;
    }

    // Test 7: longer chain no cycle
    {
        Node* nodes[6];
        for (int i=0;i<6;i++) nodes[i]=new Node(i);
        for (int i=0;i<5;i++) nodes[i]->next=nodes[i+1];
        chk("6-node chain no cycle", longestNonRepeatingChain(nodes[0]), 6, "0→1→2→3→4→5→null");
        for (int i=0;i<6;i++) delete nodes[i];
    }

    nlc_end();
    return 0;
}
