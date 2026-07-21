#include <stdio.h>
#include "nlc.h"
#include "solution.c"

static void chk(const char *name, const char *input, float got, float exp) {
    float d = got - exp;
    if (d < 0) d = -d;
    char g[40], e[40];
    snprintf(g, sizeof g, "%.4f", got);
    snprintf(e, sizeof e, "%.4f", exp);
    nlc_case(name, d < 1e-3f, input, e, g, "");
}

int main(void) {
    nlc_begin();

    /* First update: P = V*I, window has one value. */
    {
        PowerMonitor m;
        power_monitor_init(&m);
        float i[] = {2.0f};
        chk("first update, P=4", "v=2, i=[2]", power_monitor_update(&m, 2.0f, i, 1), 4.0f);
    }

    /* Current samples are averaged before multiplying. */
    {
        PowerMonitor m;
        power_monitor_init(&m);
        float i[] = {0.5f, 1.5f};
        chk("mean current then multiply", "v=10, i=[0.5,1.5]",
            power_monitor_update(&m, 10.0f, i, 2), 10.0f);
    }
    {
        PowerMonitor m;
        power_monitor_init(&m);
        float i[] = {1.0f, 2.0f, 3.0f};
        chk("mean of 3 currents", "v=4, i=[1,2,3]",
            power_monitor_update(&m, 4.0f, i, 3), 8.0f);
    }

    /* No current samples this second -> contributes 0 power. */
    {
        PowerMonitor m;
        power_monitor_init(&m);
        chk("empty current batch -> 0", "v=5, i=[]",
            power_monitor_update(&m, 5.0f, NULL, 0), 0.0f);
    }

    /* Growing window then sliding: the documented example, checked step by step. */
    {
        PowerMonitor m;
        power_monitor_init(&m);
        float i[] = {2.0f};
        chk("seq 1: avg 4.0",  "v=2, i=[2]",  power_monitor_update(&m, 2.0f,  i, 1), 4.0f);
        chk("seq 2: avg 5.0",  "v=3, i=[2]",  power_monitor_update(&m, 3.0f,  i, 1), 5.0f);
        chk("seq 3: avg 4.0",  "v=1, i=[2]",  power_monitor_update(&m, 1.0f,  i, 1), 4.0f);
        chk("seq 4: avg 5.5",  "v=5, i=[2]",  power_monitor_update(&m, 5.0f,  i, 1), 5.5f);
        chk("seq 5: avg 6.0",  "v=4, i=[2]",  power_monitor_update(&m, 4.0f,  i, 1), 6.0f);
        chk("seq 6: window slides, avg 9.2", "v=10, i=[2]",
            power_monitor_update(&m, 10.0f, i, 1), 9.2f);
    }

    /* Window never exceeds 5: after many identical samples the average is that value. */
    {
        PowerMonitor m;
        power_monitor_init(&m);
        float i[] = {2.0f};
        float last = 0.0f;
        for (int k = 0; k < 20; k++) last = power_monitor_update(&m, 3.0f, i, 1);
        chk("steady state holds at P=6", "20x (v=3, i=[2])", last, 6.0f);
    }

    nlc_end();
    return 0;
}
