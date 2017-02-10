#if defined(__linux__)
#error "You are not using a cross-compiler"
#endif

#if !defined(__i386__)
#error "You must compile with an ix86-elf compiler"
#endif

#include <stddef.h>
#include <stdint.h>

#include <multiboot.h>

#include <arch/gdt.h>
#include <arch/interrupt.h>
#include <arch/keyboard.h>
#include <arch/tty.h>

#include <kernel/io.h>

using namespace kernel;
using namespace kernel::io;

struct kernel_addr {
    unsigned kernel_vaddr_low;
    unsigned kernel_vaddr_high;
    unsigned kernel_paddr_low;
    unsigned kernel_paddr_high;
};

extern "C" void kinit() {
    tty::init();

    // Hardware-related initialization
    gdt::init();
    interrupt::init_idt();
    keyboard::init();

    kprintf("\n");
}

extern "C" void kmain(multiboot_info_t* mbd, kernel_addr *kaddr) {
    kprintf("Starting ChunkyMunkyOS v0.1\n");

    // Loop for interrupt testing
    for (;;) {
        asm("hlt");
    }
}
