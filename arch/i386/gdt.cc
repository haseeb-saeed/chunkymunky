#include <stddef.h>
#include <stdint.h>

#include <arch/gdt.h>
#include <kernel/io.h>

using namespace Kernel::Io;

extern "C" void flush_gdt(uint32_t gdt_ptr);

namespace Kernel {
namespace Gdt {
    namespace {
        const uint32_t ENTRY_BASE = 0;
        const uint32_t ENTRY_LIMIT = ~0;
        const size_t NUM_ENTRIES = 3;

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

        void set_gate(size_t index, uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity) {
            if (index >= NUM_ENTRIES) {
                kpanic("Setting gdt gate for out of bounds index: %u\n", (unsigned)index);
                return;
            }

            gdt[index].limit_low = (limit & 0xFFFF);
            gdt[index].base_low = (base & 0xFFFF);
            gdt[index].base_middle = ((base >> 16) & 0xFF);
            gdt[index].base_high = ((base >> 24) & 0xFF);
            gdt[index].access = access;
            gdt[index].granularity = granularity;
        }
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
        klog(Log_type::INIT, "GDT loaded at 0x%x\n", gdt_ptr.base);
    }
}
}
