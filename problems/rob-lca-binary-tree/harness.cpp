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

static void chk(const char* name, TreeNode* got, int expectedVal, const char* input) {
    bool ok = got && got->val == expectedVal;
    std::string gs = got ? std::to_string(got->val) : "null";
    nlc_case(name, ok, input, std::to_string(expectedVal).c_str(), gs.c_str(), "");
}

int main() {
    nlc_begin();

    // Tree:  3
    //       / \
    //      5   1
    //     / \ / \
    //    6  2 0  8
    //      / \
    //     7   4
    TreeNode* root = new TreeNode(3);
    TreeNode* n5 = new TreeNode(5);
    TreeNode* n1 = new TreeNode(1);
    TreeNode* n6 = new TreeNode(6);
    TreeNode* n2 = new TreeNode(2);
    TreeNode* n0 = new TreeNode(0);
    TreeNode* n8 = new TreeNode(8);
    TreeNode* n7 = new TreeNode(7);
    TreeNode* n4 = new TreeNode(4);
    root->left=n5; root->right=n1;
    n5->left=n6; n5->right=n2;
    n1->left=n0; n1->right=n8;
    n2->left=n7; n2->right=n4;

    // Test 1: p is ancestor of q → LCA is p
    chk("ancestor is LCA: LCA(5,4)=5", lowestCommonAncestor(root,n5,n4), 5, "LCA(5,4)");

    // Test 2: different subtrees → root is LCA
    chk("different subtrees: LCA(5,1)=3", lowestCommonAncestor(root,n5,n1), 3, "LCA(5,1)");

    // Test 3: deep different subtrees
    chk("deep LCA(6,4)=5", lowestCommonAncestor(root,n6,n4), 5, "LCA(6,4)");

    // Test 4: both in right subtree
    chk("right subtree LCA(0,8)=1", lowestCommonAncestor(root,n0,n8), 1, "LCA(0,8)");

    // Test 5: single-node tree where root is both (tiny tree)
    {
        TreeNode* solo = new TreeNode(42);
        chk("single node root=p=q", lowestCommonAncestor(solo,solo,solo), 42, "LCA(42,42)");
        delete solo;
    }

    // Test 6: LCA is not root
    chk("LCA not root LCA(7,4)=2", lowestCommonAncestor(root,n7,n4), 2, "LCA(7,4)");

    // Test 7: both targets are leaves in different subtrees
    chk("leaves LCA(6,7)=5", lowestCommonAncestor(root,n6,n7), 5, "LCA(6,7)");

    // Test 8: one target is root
    chk("one target is root LCA(3,7)=3", lowestCommonAncestor(root,root,n7), 3, "LCA(3,7)");

    nlc_end();
    return 0;
}
