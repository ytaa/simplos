#include <stdint.h>
#include <string.h>

void* memcpy(void* __restrict dst, const void* __restrict src, size_t nbytes) {
    uint8_t* bdst = dst;
    const uint8_t* bsrc = src;

    for (uint32_t byte_index = 0; byte_index < nbytes; byte_index++, bdst++, bsrc++) {
        *bdst = *bsrc;
    }

    return (void*)bdst;
}