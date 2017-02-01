#include <stddef.h>
#include <stdint.h>

#include <arch/interrupt.h>

extern "C" void load_idt(uint32_t idt_ptr);

namespace kernel {
namespace interrupt {
    static const size_t NUM_ENTRIES = 256;

    // Packed to prevent any compiler optimization
    struct Idt_entry {
        uint16_t base_low;
        uint16_t sel;
        uint8_t zero;
        uint8_t flags;
        uint16_t base_high;
    } __attribute__((packed));

    struct Idt_ptr {
        uint16_t limit;
        uint32_t base;
    } __attribute__((packed));

    Idt_entry idt[NUM_ENTRIES] = {{0, 0, 0, 0, 0}};
    Idt_ptr idt_ptr;

    // TODO: Uncomment this when adding interrupt handlers
    /*
    static void set_idt_gate(size_t index, uint32_t base, uint16_t sel, uint8_t flags) {
        if (index >= NUM_ENTRIES) {
            // TODO: Panic or something
            return;
        }

        idt[index].base_low = (base & 0xFFFF);
        idt[index].base_high = ((base >> 16) & 0xFFFF);
        idt[index].sel = sel;
        idt[index].zero = 0;
        idt[index].flags = flags;
    }
    */

    void init_idt() {
        idt_ptr.limit = sizeof(idt) - 1;
        idt_ptr.base = (uint32_t)&idt;

        load_idt((uint32_t)&idt_ptr);
    }

    void init_handlers() {
    }
}
}
