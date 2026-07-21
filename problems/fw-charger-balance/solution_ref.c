#include <stdio.h>
#include <stdbool.h>

void balanceCurrent(int n, const bool *car_present, const bool *online,
                    int site_limit, int max_charger, int *limits) {
    int k = 0;
    for (int i = 0; i < n; i++) {
        limits[i] = 0;
        if (online[i] && car_present[i]) k++;
    }
    if (k == 0) return;

    int total = site_limit < k * max_charger ? site_limit : k * max_charger;
    int base = total / k;
    int rem  = total % k;

    for (int i = 0; i < n; i++) {
        if (online[i] && car_present[i]) {
            limits[i] = base;
            if (rem > 0) { limits[i]++; rem--; }
        }
    }
}
