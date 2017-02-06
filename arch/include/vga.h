#ifndef __ARCH_VGA_H__
#define __ARCH_VGA_H__

#include <stddef.h>
#include <stdint.h>

namespace kernel {
namespace vga {
    typedef uint8_t Vga_color;
    typedef uint16_t Vga_entry;

    enum class Color : uint8_t {
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
        return ((uint8_t)bg << 4) | (uint8_t)fg;
    }

    static inline Vga_entry make_entry(char c, Vga_color color) {
        return ((uint16_t)color << 8) | (uint16_t)c;
    }
}
}

#endif // __ARCH_VGA_H__
