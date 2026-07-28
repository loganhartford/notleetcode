#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "nlc.h"
#include "solution.c"

static void chk(const char *name, char *input, const char *expected) {
    char *got = run_length_encode(input);
    const char *g = got ? got : "(null)";
    nlc_case(name, got && strcmp(got, expected) == 0, input, expected, g, "");
    free(got);
}

int main(void) {
    nlc_begin();

    chk("main example", "aaabbc", "a3b2c1");
    chk("runs longer than 9 group correctly", "aaabbbcccc", "a3b3c4");
    chk("single character -> count 1", "a", "a1");
    chk("all distinct, every run counted", "abcde", "a1b1c1d1e1");
    chk("empty string -> empty", "", "");

    /* Multi-digit run length. */
    chk("twelve a's -> a12", "aaaaaaaaaaaa", "a12");

    /* Same char in non-adjacent runs stays separate. */
    chk("non-adjacent repeats", "aabbaa", "a2b2a2");

    /* Longer mixed input. */
    chk("mixed runs and singles", "wwwwaaadexxxxxx", "w4a3d1e1x6");

    nlc_end();
    return 0;
}
