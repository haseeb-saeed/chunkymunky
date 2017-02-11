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
#include <arch/memory.h>
#include <arch/tty.h>

#include <kernel/addr.h>
#include <kernel/io.h>

using namespace Arch;
using namespace Kernel::Io;

namespace Kernel {
extern "C" void kinit(multiboot_info_t* mbd, Kernel_addr *kaddr) {
    Tty::init();

    // Hardware-related initialization
    Gdt::init();
    Interrupt::init();
    Keyboard::init();

    // Memory initialization
    Memory::Pm_manager::init(mbd, kaddr);

    kprintf("\n");
}

extern "C" void kmain(multiboot_info_t* mbd, Kernel_addr *kaddr) {
    kprintf("Starting ChunkyMunkyOS v0.1\n");
    kprintf("Kernel loaded at 0x%x\n", kaddr->vaddr_start);
    kprintf("Kernel space starts at 0x%x\n", kaddr->base);
    kprintf("Free memory starts at 0x%x\n", kaddr->vaddr_end);

    for (int i = 0; i < 15; ++i) {
        Memory::Pm_manager::alloc_frame();
    }
}
}
