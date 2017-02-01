#include <stddef.h>
#include <stdint.h>
#include <string.h>

void* memmove(void* dst, const void* src, size_t n) {
    char* p1 = (char*)dst;
    char* p2 = (char*)src;

    if (p1 < p2) {
        for (size_t i = 0; i < n; ++i) {
            p1[i] = p2[i];
        }
    } else {
        for (size_t i = n; i > 0; --i) {
            p1[i - 1] = p2[i - 1];
        }
    }

    return dst;
}
