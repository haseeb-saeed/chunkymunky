#include <stddef.h>
#include <stdint.h>

#include <arch/gdt.h>

extern "C" void flush_gdt(uint32_t gdt_ptr);

namespace kernel {
namespace gdt {
    static const uint32_t ENTRY_BASE = 0;
    static const uint32_t ENTRY_LIMIT = 0x000FFFFF;
    static const size_t NUM_ENTRIES = 3;

    // Packed to prevent any compiler optimization
    struct Gdt_entry {
        uint16_t limit_low;
        uint16_t base_low;
        uint8_t base_middle;
        uint8_t access;
        uint8_t granularity;
        uint8_t base_high;
    } __attribute__((packed));

    struct Gdt_ptr {
        uint16_t limit;
        uint32_t base;
    } __attribute__((packed));

    Gdt_entry gdt[NUM_ENTRIES];
    Gdt_ptr gdt_ptr;

    static void set_gate(size_t index, uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity) {
        if (index >= NUM_ENTRIES) {
            // TODO: Panic or something
            return;
        }

        gdt[index].limit_low = (limit & 0xFFFF);
        gdt[index].base_low = (base & 0xFFFF);
        gdt[index].base_middle = ((base >> 16) & 0xFF);
        gdt[index].base_high = ((base >> 24) & 0xFF);
        gdt[index].access = access;
        gdt[index].granularity = (((limit >> 16) & 0x0F) | (granularity & 0xF0));
    }

    void init() {
        gdt_ptr.limit = sizeof(gdt) - 1;
        gdt_ptr.base = (uint32_t)&gdt;

        // NULL descriptor, code segment, data segment
        // TODO: Add a TSS entry
        set_gate(0, 0, 0, 0, 0);
        set_gate(1, ENTRY_BASE, ENTRY_LIMIT, 0x9A, 0xCF);
        set_gate(2, ENTRY_BASE, ENTRY_LIMIT, 0x92, 0xCF);

        flush_gdt((uint32_t)&gdt_ptr);
    }
}
}
