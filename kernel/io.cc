#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <arch/frame_buffer.h>
#include <kernel/io.h>

namespace kernel {
namespace io {
    static int kprintf_internal(const char* fmt, va_list args) {
        int result = 0;
        char buffer[65];

        while (*fmt != '\0') {
            char c = *fmt++;

            if (c != '%') {
                frame_buffer::print(c);
                ++result;
                continue;
            }

            c = *fmt++;
            switch(c) {
                case '%':
                    frame_buffer::print('%');
                    ++result;
                    break;
                case 's':
                    frame_buffer::print(va_arg(args, char*));
                    break;
                case 'c':
                    frame_buffer::print((char)va_arg(args, int));
                    ++result;
                    break;
                case 'i':
                case 'd':
                    result += itoa(va_arg(args, int), buffer);
                    frame_buffer::print(buffer);
                    break;
                case 'b':
                    result += utoa(va_arg(args, unsigned), buffer, 2);
                    frame_buffer::print(buffer);
                    break;
                case 'o':
                    result += utoa(va_arg(args, unsigned), buffer, 8);
                    frame_buffer::print(buffer);
                    break;
                case 'x':
                    result += utoa(va_arg(args, unsigned), buffer, 16);
                    frame_buffer::print(buffer);
                    break;
                case 'u':
                    result += utoa(va_arg(args, unsigned), buffer);
                    frame_buffer::print(buffer);
                    break;
                default:
                    // TODO: Handle invalid format specifier
                    break;
            }
        }

        return result;
    }

    int kprintf(const char* fmt, ...) {
        int result;
        va_list args;

        va_start(args, fmt);
        result = kprintf_internal(fmt, args);
        va_end(args);

        return result;
    }

    // TODO: Consolidate all printing into a klog function with flags?
    void kpanic(const char* fmt, ...) {
        va_list args;

        kprintf("Panic: ");
        va_start(args, fmt);
        kprintf_internal(fmt, args);
        va_end(args);

        // TODO: Aboort/halt
    }

    void kerror(const char* fmt, ...) {
        // TODO: Redirect to file or stderr
        va_list args;

        kprintf("Error: ");
        va_start(args, fmt);
        kprintf_internal(fmt, args);
        va_end(args);
    }
}
}
