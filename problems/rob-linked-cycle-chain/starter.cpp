#include <unordered_set>

struct Node {
    int val;
    Node* next;
    Node(int v) : val(v), next(nullptr) {}
};

int longestNonRepeatingChain(Node* head) {
    // TODO: traverse from head, count unique nodes (by pointer) before revisit
    // Use an unordered_set<Node*> to track visited pointers
    return 0;
}
