#include <stdio.h>
#include <stdbool.h>

void balanceCurrent(int n, const bool *car_present, const bool *online,
                    int site_limit, int max_charger, int *limits) {
    for (int i = 0; i < n; i++) limits[i] = 0;
    if (n <= 0) return;

    int num_present = 0;   /* chargers reporting a car (offline -> car_present is false) */
    int num_offline = 0;
    for (int i = 0; i < n; i++) {
        if (car_present[i]) num_present++;
        if (!online[i])     num_offline++;
    }

    /* Fallback each offline charger self-applies; we reserve it out of the budget. */
    int safe_limit = site_limit / n;
    int budget = site_limit - num_offline * safe_limit;

    /* Split the remaining budget equally among the reachable chargers with a car. */
    int charge_limit = num_present > 0 ? budget / num_present : 0;
    if (charge_limit > max_charger) charge_limit = max_charger;

    for (int i = 0; i < n; i++) {
        if (!online[i])          limits[i] = safe_limit;    /* assumed charging at fallback */
        else if (car_present[i]) limits[i] = charge_limit;
        else                     limits[i] = 0;             /* reachable, no car */
    }
}
