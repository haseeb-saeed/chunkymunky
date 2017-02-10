#ifndef __ARCH_TTY_H__
#define __ARCH_TTY_H__

#include <arch/vga.h>

namespace Kernel {
namespace Tty {
    void init();
    void clear();
    void set_color(Kernel::Vga::Vga_color color);
    void set_default_color(Kernel::Vga::Vga_color color);
    void reset_color(Kernel::Vga::Vga_color color);
    void print(char c);
    void print(const char* str);
}
}

#endif // __ARCH_TTY_H__
