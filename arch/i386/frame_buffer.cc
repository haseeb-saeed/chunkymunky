#include <stddef.h>
#include <stdint.h>

#include <arch/frame_buffer.h>

namespace kernel {
namespace frame_buffer {
    enum Color {
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

    static const size_t WIDTH = 80;
    static const size_t HEIGHT = 25;
    static int row, column;
    static uint16_t* buffer = (uint16_t*)0xB8000;

    static inline uint8_t make_color(Color fg, Color bg) {
        return fg | bg << 4;
    }

    static inline uint16_t make_entry(char c, uint8_t color) {
        return (uint16_t)c | (uint16_t)color << 8;
    }

    void clear() {
        row = 0;
        column = 0;

        // TODO: Replace with memcpy
        for (size_t i = 0; i < WIDTH * HEIGHT; ++i) {
            buffer[i] = make_entry(' ', make_color (LIGHT_GREY, BLACK));
        }
    }

    void print(char c) {
        if (c == '\n') {
            column = 0;
            ++row;
        } else {
            const size_t index = row * WIDTH + column;
            buffer[index] = make_entry(c, make_color (LIGHT_GREY, BLACK));

            if (++column == WIDTH) {
                column = 0;
                ++row;
            }
        }

        // TODO: Shift rows if we're at the last row
        if (row == HEIGHT) {
        }
    }

    void print(const char* str) {
        for (size_t i = 0; str[i] != '\0'; ++i) {
            print(str[i]);
        }
    }
}
}
