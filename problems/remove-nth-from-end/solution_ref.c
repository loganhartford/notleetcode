#include <stdlib.h>

struct ListNode* removeNthFromEnd(struct ListNode* head, int n) {
    struct ListNode dummy;
    dummy.next = head;
    struct ListNode* fast = &dummy;
    struct ListNode* slow = &dummy;
    for (int i = 0; i < n; i++) fast = fast->next;
    while (fast->next) {
        fast = fast->next;
        slow = slow->next;
    }
    struct ListNode* victim = slow->next;
    slow->next = victim->next;
    free(victim);
    return dummy.next;
}
