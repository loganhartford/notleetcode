#include <stdio.h>

#define POWER_WINDOW 5

typedef struct {
    float window[POWER_WINDOW];
    int   count;
    int   head;
} PowerMonitor;

void power_monitor_init(PowerMonitor *m) {
    m->count = 0;
    m->head  = 0;
    for (int i = 0; i < POWER_WINDOW; i++) m->window[i] = 0.0f;
}

float power_monitor_update(PowerMonitor *m, float voltage,
                           const float *current_samples, int n) {
    float isum = 0.0f;
    for (int i = 0; i < n; i++) isum += current_samples[i];
    float iavg = n > 0 ? isum / (float)n : 0.0f;

    float power = voltage * iavg;
    m->window[m->head] = power;
    m->head = (m->head + 1) % POWER_WINDOW;
    if (m->count < POWER_WINDOW) m->count++;

    float psum = 0.0f;
    for (int i = 0; i < m->count; i++) psum += m->window[i];
    return psum / (float)m->count;
}
