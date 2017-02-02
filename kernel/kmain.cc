#if defined(__linux__)
#error "You are not using a cross-compiler"
#endif

#if !defined(__i386__)
#error "You must compile with an ix86-elf compiler"
#endif

#include <stddef.h>
#include <stdint.h>

#include <arch/frame_buffer.h>
#include <arch/gdt.h>
#include <arch/interrupt.h>

using namespace kernel;

extern "C" void kmain() {
    gdt::init();
    interrupt::init_idt();
    frame_buffer::clear();

    frame_buffer::print("Starting OS...\n");

    for (char i = 0; i <= 127; ++i) {
            frame_buffer::print(i);
            frame_buffer::print('\n');
    }
}
