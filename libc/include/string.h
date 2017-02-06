#ifndef __LIBC_STRING_H__
#define __LIBC_STRING_H__

#include <stdint.h>
#include <stddef.h>

extern "C" {
    void* memcpy(void* dst, const void* src, size_t n);
    void* memmove(void* dst, const void* src, size_t n);
    void* memset(void* s, int c, size_t n);
    void* memchr(const void* s, int c, size_t n);
    int memcmp(const void* s1, const void* s2, size_t n);

    size_t strlen(const char* str);
    size_t itoa(int n, char* str, int base = 10);
    size_t utoa(unsigned n, char* str, int base = 10);
}

#endif // __LIBC_STRING_H__
