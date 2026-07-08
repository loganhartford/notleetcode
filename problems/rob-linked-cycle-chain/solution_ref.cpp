#include <unordered_set>

struct Node {
    int val;
    Node* next;
    Node(int v) : val(v), next(nullptr) {}
};

int longestNonRepeatingChain(Node* head) {
    std::unordered_set<Node*> visited;
    int count = 0;
    Node* cur = head;
    while (cur && !visited.count(cur)) {
        visited.insert(cur);
        count++;
        cur = cur->next;
    }
    return count;
}
