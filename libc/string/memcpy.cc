#include <stddef.h>
#include <stdint.h>
#include <string.h>

void* memcpy(void* __restrict__ dst, void* __restrict__ src, size_t n) {
    char* p1 = (char*)dst;
    char* p2 = (char*)src;

    for (size_t i = 0; i < n; ++i) {
        p1[i] = p2[i];
    }

    return dst;
}
