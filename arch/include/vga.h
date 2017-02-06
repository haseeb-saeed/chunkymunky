#ifndef __ARCH_VGA_H__
#define __ARCH_VGA_H__

#include <stddef.h>
#include <stdint.h>

#include <kernel/enum.h>

namespace kernel {
namespace vga {
    typedef uint8_t Vga_color;
    typedef uint16_t Vga_entry;

    KENUM(Color, uint8_t) {
        BLACK,
        BLUE,
        GREEN,
        CYAN,
        RED,
        MAGENTA,
        BROWN,
        LIGHT_GREY,
        DARK_GREY,
        LIGHT_BLUE,
        LIGHT_GREEN,
        LIGHT_CYAN,
        LIGHT_RED,
        LIGHT_MAGENTA,
        LIGHT_BROWN,
        WHITE,
    };

    static inline Vga_color make_color(Color fg, Color bg) {
        return (underlying_type(bg) << 4) | underlying_type(fg);
    }

    static inline Vga_entry make_entry(char c, Vga_color color) {
        return (color << 8) | c;
    }
}
}

#endif // __ARCH_VGA_H__
