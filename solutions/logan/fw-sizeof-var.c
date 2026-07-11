#include <stddef.h>
#include <stdint.h>

#define MY_SIZEOF_VAR(x) ((size_t)(((char *)((&x) + 1)) - ((char *)(&x))))

/*
 * For the normal variable case, we get the pointer to the variable and we increment by 1
 * which becasue of pointer arithmetic will advance the pointer by the size of the variable.
 * Then we cast the incremented and original pointer to char * so the arithmetic will be byte level
 * and then we subtract them to see how many bytes appart the two addresses are.
 * 
 * In the array case we take &(buf) which is &(uint8_t *) which if buf is buf[16], evaluates to
 * (uint8_t (*)[16]), so when we increment the pointer we increment by the whole size of the
 * array.
 */