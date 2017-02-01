#include <stddef.h>
#include <stdint.h>
#include <string.h>

void* memchr(const void* s, int c, size_t n) {
    unsigned char* ptr = (unsigned char*)s;
    unsigned char ch = (unsigned char)c;

    for (size_t i = 0; i < n; ++i) {
        if (ptr[i] == ch) {
            return ptr + i;
        }
    }

    return nullptr;
}
