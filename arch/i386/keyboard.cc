#include <stddef.h>
#include <stdint.h>

#include <arch/frame_buffer.h>
#include <arch/interrupt.h>
#include <arch/keyboard.h>

using namespace kernel;

extern "C" uint8_t get_keyboard_input();

namespace kernel {
namespace keyboard {
    void handle_input(interrupt::Interrupt_frame* frame) {
        uint8_t key = get_keyboard_input();

        frame_buffer::print("Keyboard handler! ");
        frame_buffer::print((int)key);
        frame_buffer::print("\n");
    }

    void init() {
        interrupt::add_handler(33, &handle_input);
    }
}
}
