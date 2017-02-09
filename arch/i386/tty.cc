#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <arch/tty.h>
#include <arch/vga.h>

using namespace kernel::vga;

namespace kernel {
namespace tty {
    static const size_t WIDTH = 80;
    static const size_t HEIGHT = 25;
    static size_t row, column;
    static Vga_entry* buffer = (Vga_entry*)0xC00B8000;
    static Vga_color current_color, default_color;

    static void scroll() {
        memmove(buffer, buffer + WIDTH, WIDTH * (HEIGHT - 1) * sizeof(*buffer));
        for (size_t i = WIDTH * (HEIGHT - 1); i < WIDTH * HEIGHT; ++i) {
            buffer[i] = make_entry(0, current_color);
        }

        row -= 1;
    }

    void init() {
        current_color = make_color(Color::LIGHT_GREY, Color::BLACK);
        default_color = current_color;
        clear();
    }

    void clear() {
        row = 0;
        column = 0;

        for (size_t i = 0; i < WIDTH * HEIGHT; ++i) {
            buffer[i] = make_entry(0, current_color);
        }
    }

    void set_color(Vga_color color) {
        current_color = color;
    }

    void set_default_color(Vga_color color) {
        default_color = color;
    }

    void reset_color() {
        current_color = default_color;
    }

    void print(char c) {
        if (c == '\n') {
            column = 0;
            ++row;
        } else {
            const size_t index = row * WIDTH + column;
            buffer[index] = make_entry(c, current_color);

            if (++column == WIDTH) {
                column = 0;
                ++row;
            }
        }

        if (row == HEIGHT) {
            scroll();
        }
    }

    void print(const char* str) {
        for (size_t i = 0; str[i] != '\0'; ++i) {
            print(str[i]);
        }
    }
}
}
