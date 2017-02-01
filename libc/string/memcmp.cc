#include <stddef.h>
#include <stdint.h>
#include <string.h>

int memcmp(const void* s1, const void* s2, size_t n) {
    unsigned char* p1 = (unsigned char*)s1;
    unsigned char* p2 = (unsigned char*)s2;

    for (size_t i = 0; i < n; ++i) {
        if (p1[i] != p2[i]) {
            return p1[i] - p2[i];
        }
    }

    return 0;
}
