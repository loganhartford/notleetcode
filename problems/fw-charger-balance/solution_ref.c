#include <stdio.h>
#include <stdbool.h>

void balanceCurrent(int n, const bool *car_present, const bool *online,
                    int site_limit, int max_charger, int *limits) {
    for (int i = 0; i < n; i++) limits[i] = 0;
    if (n <= 0) return;

    /* Fallback a charger self-applies when it loses the leader: the fair share
     * assuming every charger were online, never above its own ceiling. */
    int safe_min = site_limit / n;
    if (safe_min > max_charger) safe_min = max_charger;

    /* We can't see or command an offline charger, and someone may have plugged
     * in, so reserve its fallback out of the budget and record it as its draw. */
    int reserved = 0;
    for (int i = 0; i < n; i++) {
        if (!online[i]) {
            limits[i] = safe_min;
            reserved += safe_min;
        }
    }

    int budget = site_limit - reserved;   /* >= 0 when site_limit <= n*max_charger */

    int k = 0;
    for (int i = 0; i < n; i++)
        if (online[i] && car_present[i]) k++;
    if (k == 0) return;

    int total = budget < k * max_charger ? budget : k * max_charger;
    int base = total / k;
    int rem  = total % k;

    for (int i = 0; i < n; i++) {
        if (online[i] && car_present[i]) {
            limits[i] = base;
            if (rem > 0) { limits[i]++; rem--; }
        }
    }
}
