// Time: O(n) — Floyd's tortoise-and-hare; O(1) space
#include <stdbool.h>

bool list_has_cycle(const node_t *head) {
    const node_t *slow = head;
    const node_t *fast = head;
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast) return true;
    }
    return false;
}
