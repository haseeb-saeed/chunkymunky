#include <stddef.h>
#include <stdint.h>
#include <string.h>

void* memset(void *s, int c, size_t n) {
    char* ptr = (char*)s;
    char fill = (char)c;

    for (size_t i = 0; i < n; ++i) {
        ptr[i] = fill;
    }

    return s;
}
