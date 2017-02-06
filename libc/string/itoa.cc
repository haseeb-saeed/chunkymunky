#include <stddef.h>
#include <stdint.h>
#include <string.h>

size_t itoa(int n, char* str, int base) {
    size_t len = 0;

    // Only binary to hex is supported
    if (base < 2 || base > 16) {
        *str = '\0';
    } else {
        if (n < 0 && base == 10) {
            *str++ = '-';
            ++len;
            n = -n;
        }

        len += utoa((unsigned)n, str, base);
    }

    return len;
}
