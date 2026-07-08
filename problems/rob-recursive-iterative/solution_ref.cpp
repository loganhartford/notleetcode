#include <vector>

struct ListNode {
    int val;
    ListNode* next;
    ListNode(int v, ListNode* n = nullptr) : val(v), next(n) {}
};

int listLength(ListNode* head) {
    int count = 0;
    while (head) { count++; head = head->next; }
    return count;
}

std::vector<int> reverseValues(ListNode* head) {
    std::vector<int> vals;
    while (head) { vals.push_back(head->val); head = head->next; }
    std::reverse(vals.begin(), vals.end());
    return vals;
}
