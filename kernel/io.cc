#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <arch/tty.h>
#include <kernel/io.h>

using namespace Arch;

namespace Kernel {
namespace Io {
    namespace {
        int kprintf_internal(const char* fmt, va_list args) {
            int result = 0;
            char buffer[65];
            char* str;

            while (*fmt != '\0') {
                char c = *fmt++;

                if (c != '%') {
                    Tty::print(c);
                    ++result;
                    continue;
                }

                c = *fmt++;
                switch(c) {
                    case '%':
                        Tty::print('%');
                        ++result;
                        break;
                    case 's':
                        str = va_arg(args, char*);
                        result += strlen(str);
                        Tty::print(str);
                        break;
                    case 'c':
                        Tty::print((char)va_arg(args, int));
                        ++result;
                        break;
                    case 'i':
                    case 'd':
                        result += itoa(va_arg(args, int), buffer);
                        Tty::print(buffer);
                        break;
                    case 'b':
                        result += utoa(va_arg(args, unsigned), buffer, 2);
                        Tty::print(buffer);
                        break;
                    case 'o':
                        result += utoa(va_arg(args, unsigned), buffer, 8);
                        Tty::print(buffer);
                        break;
                    case 'x':
                        result += utoa(va_arg(args, unsigned), buffer, 16);
                        Tty::print(buffer);
                        break;
                    case 'u':
                        result += utoa(va_arg(args, unsigned), buffer);
                        Tty::print(buffer);
                        break;
                    default:
                        // TODO: Handle invalid format specifier
                        break;
                }
            }

            return result;
        }
    }

    int kprintf(const char* fmt, ...) {
        int result;
        va_list args;

        va_start(args, fmt);
        result = kprintf_internal(fmt, args);
        va_end(args);

        return result;
    }

    void klog(Log_type type, const char* fmt, ...) {
        va_list args;

        switch(type) {
            case Log_type::INIT:
                kprintf("INIT: ");
                break;
            case Log_type::KEYBOARD:
                kprintf("KEYBOARD: ");
            case Log_type::WARNING:
            case Log_type::ERROR:
            default:
                break;
        }

        va_start(args, fmt);
        kprintf_internal(fmt, args);
        va_end(args);
    }

    void kpanic(const char* fmt, ...) {
        va_list args;

        kprintf("Panic: ");
        va_start(args, fmt);
        kprintf_internal(fmt, args);
        va_end(args);

        // TODO: Aboort/halt
    }
}
}
