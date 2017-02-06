#include <stddef.h>
#include <stdint.h>

#include <arch/interrupt.h>
#include <arch/keyboard.h>
#include <kernel/io.h>

using namespace kernel::interrupt;
using namespace kernel::io;

extern "C" uint8_t get_keyboard_input();

namespace kernel {
namespace keyboard {
    void handle_input(Interrupt_frame* frame) {
        uint8_t key = get_keyboard_input();
        klog(Log_type::KEYBOARD, "%d\n", key);
    }

    void init() {
        add_handler(Interrupt_type::KEYBOARD, &handle_input);
        klog(Log_type::INIT, "Keyboard initialized\n");
    }
}
}
