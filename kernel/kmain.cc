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
#include <arch/keyboard.h>

using namespace kernel;

extern "C" void kinit() {
    frame_buffer::clear();

    // Hardware-related initialization
    gdt::init();
    interrupt::init_idt();
    keyboard::init();
}

extern "C" void kmain() {
    frame_buffer::print("Starting OS...\n");

    // Loop for interrupt testing
    for (;;) {
        asm("hlt");
    }
}
