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

static std::string fmtVS(const std::vector<std::string>& v) {
    std::string s = "[";
    for (int i=0;i<(int)v.size();i++){if(i)s+=","; s+="\""+v[i]+"\"";}
    return s+"]";
}

static void check(const char* name, std::vector<std::string> got,
                  std::vector<std::string> expected, const char* input) {
    std::string gs=fmtVS(got), es=fmtVS(expected);
    nlc_case(name, got==expected, input, es.c_str(), gs.c_str(), "");
}

int main() {
    nlc_begin();

    // Test 1: basic case, no ties
    check("basic no ties",
          topKFrequent({"apple","banana","apple","cherry","banana","apple"},2),
          {"apple","banana"}, "apple x3 banana x2 cherry x1, k=2");

    // Test 2: ties broken lexicographically
    check("tie broken lex",
          topKFrequent({"b","a","c","b","a","c"},2),
          {"a","b"}, "a,b,c all freq=2, k=2 → a,b");

    // Test 3: k=1
    check("k=1",
          topKFrequent({"dog","cat","dog","dog"},1),
          {"dog"}, "dog x3, k=1");

    // Test 4: k equals unique count (all words)
    check("k=unique count",
          topKFrequent({"x","y","x","y","z"},3),
          {"x","y","z"}, "x,y freq=2 z freq=1, k=3");

    // Test 5: duplicate words counted correctly
    check("duplicates",
          topKFrequent({"hello","hello","hello"},1),
          {"hello"}, "hello x3, k=1");

    // Test 6: single word list
    check("single word",
          topKFrequent({"go"},1),
          {"go"}, "single word 'go', k=1");

    // Test 7: classic example
    check("classic i love leetcode",
          topKFrequent({"i","love","leetcode","i","love","coding"},2),
          {"i","love"}, "i,love x2 rest x1, k=2");

    nlc_end();
    return 0;
}
