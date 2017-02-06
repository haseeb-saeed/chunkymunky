#include <stddef.h>
#include <stdint.h>
#include <string.h>

size_t utoa(unsigned n, char* str, int base) {
    size_t len = 0;

    // Only binary to hex is supported
    if (base < 2 || base > 16) {
        *str = '\0';
    } else {
        size_t i = 0;
        const char* digits = "0123456789abcdef";
        char temp;

        do {
            str[i++] = digits[n % base];
            n /= base;
        } while(n > 0);

        len = i;
        str[i--] = '\0';

        for (size_t j = 0; j < i; ++j, --i) {
            temp = str[i];
            str[i] = str[j];
            str[j] = temp;
        }
    }

    return len;
}
