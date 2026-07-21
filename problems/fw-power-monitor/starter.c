#include <stdio.h>

#define POWER_WINDOW 5

typedef struct {
    float window[POWER_WINDOW];   /* last 5 one-second power values */
    int   count;                  /* how many are valid so far (0..5) */
    int   head;                   /* next write position             */
} PowerMonitor;

void power_monitor_init(PowerMonitor *m) {

}

float power_monitor_update(PowerMonitor *m, float voltage,
                           const float *current_samples, int n) {
    return 0.0f;
}
