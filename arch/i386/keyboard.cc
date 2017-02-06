#include <stddef.h>
#include <stdint.h>

#include <arch/interrupt.h>
#include <arch/keyboard.h>
#include <kernel/io.h>

using namespace kernel::io;

extern "C" uint8_t get_keyboard_input();

namespace kernel {
namespace keyboard {
    void handle_input(interrupt::Interrupt_frame* frame) {
        uint8_t key = get_keyboard_input();
        kprintf("Keyboard handler! %d\n", key);
    }

    void init() {
        interrupt::add_handler(33, &handle_input);
        kprintf("Keyboard initialized\n");
    }
}
}
