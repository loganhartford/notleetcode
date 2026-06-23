#include <stddef.h>

void reorderList(struct ListNode* head) {
    if (!head || !head->next) return;

    /* 1. Find the middle (slow ends at the middle). */
    struct ListNode* slow = head;
    struct ListNode* fast = head;
    while (fast->next && fast->next->next) {
        slow = slow->next;
        fast = fast->next->next;
    }

    /* 2. Reverse the second half. */
    struct ListNode* second = slow->next;
    slow->next = NULL;
    struct ListNode* prev = NULL;
    while (second) {
        struct ListNode* nxt = second->next;
        second->next = prev;
        prev = second;
        second = nxt;
    }
    second = prev;

    /* 3. Merge the two halves, alternating. */
    struct ListNode* first = head;
    while (second) {
        struct ListNode* f_next = first->next;
        struct ListNode* s_next = second->next;
        first->next = second;
        second->next = f_next;
        first = f_next;
        second = s_next;
    }
}
