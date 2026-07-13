#include <stdint.h>
#include <stdio.h>
#include "nlc.h"
#include "solution.c"

static void chk(const char *nm, const char *in, void *got, void *exp) {
    nlc_case(nm, got == exp, in, "correct addr", got == exp ? "correct addr" : "wrong addr", "");
}

int main(void) {
    nlc_begin();

    uint8_t data[32];

    chk("offset 0",  "byte_offset(data, 0)",  byte_offset(data, 0),  &data[0]);
    chk("offset 5",  "byte_offset(data, 5)",  byte_offset(data, 5),  &data[5]);
    chk("offset 15", "byte_offset(data, 15)", byte_offset(data, 15), &data[15]);
    chk("offset 31", "byte_offset(data, 31)", byte_offset(data, 31), &data[31]);

    /* Works on any pointer type */
    int nums[8];
    chk("int offset 0", "byte_offset(nums, 0)",              byte_offset(nums, 0),              &nums[0]);
    chk("int offset 4", "byte_offset(nums, sizeof(int))",    byte_offset(nums, sizeof(int)),    &nums[1]);
    chk("int offset 8", "byte_offset(nums, 2*sizeof(int))",  byte_offset(nums, 2*sizeof(int)),  &nums[2]);

    nlc_end();
    return 0;
}
