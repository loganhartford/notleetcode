#include <stdio.h>
#include <stdbool.h>

void balanceCurrent(int n, const bool *car_present, const bool *online,
                    int site_limit, int max_charger, int *limits) {
    uint8_t num_present = 0;
    uint8_t num_offline = 0;
    for (int i = 0; i < n; i++) {
        if (car_present[i]) num_present++;
        if (!online[i]) num_offline++;
    }
    
    int safe_limit = site_limit / n;
    int charge_limit = (site_limit - (num_offline * safe_limit)) / num_present;
    if (charge_limit > max_charger) charge_limit = max_charger;
    
    for (int i = 0; i < n; i++) {
        if (car_present[i] && online[i]) {
            limits[i] = charge_limit;
        } else if (!online[i]) {
            limits[i] = safe_limit;
        } else {
            limits[i] = 0;
        }
    }
}
