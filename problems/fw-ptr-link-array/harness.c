#include <stdint.h>
#include <stdio.h>
#include "nlc.h"
#include "solution.c"

static void chk(const char *nm, const char *in, void *got, void *exp) {
    nlc_case(nm, got == exp, in, "correct addr", got == exp ? "correct addr" : "wrong addr", "");
}

int main(void) {
    nlc_begin();

    /* 4 blocks of 16 bytes */
    static uint8_t mem64[64] __attribute__((aligned(8)));
    link_block_array(mem64, 4, 16);
    chk("b0->b1",    "*(void**)&mem[0]==&mem[16]",  *(void **)&mem64[0],  &mem64[16]);
    chk("b1->b2",    "*(void**)&mem[16]==&mem[32]", *(void **)&mem64[16], &mem64[32]);
    chk("b2->b3",    "*(void**)&mem[32]==&mem[48]", *(void **)&mem64[32], &mem64[48]);
    chk("b3->NULL",  "*(void**)&mem[48]==NULL",      *(void **)&mem64[48], NULL);

    /* 3 blocks of 8 bytes */
    static uint8_t mem24[24] __attribute__((aligned(8)));
    link_block_array(mem24, 3, 8);
    chk("3b: b0->b1", "*(void**)&m[0]==&m[8]",  *(void **)&mem24[0], &mem24[8]);
    chk("3b: b1->b2", "*(void**)&m[8]==&m[16]", *(void **)&mem24[8], &mem24[16]);
    chk("3b: b2->NULL","*(void**)&m[16]==NULL",  *(void **)&mem24[16], NULL);

    /* 1 block: just NULL */
    static uint8_t mem8[8] __attribute__((aligned(8)));
    link_block_array(mem8, 1, 8);
    chk("1b: b0->NULL", "*(void**)mem8==NULL", *(void **)mem8, NULL);

    nlc_end();
    return 0;
}
