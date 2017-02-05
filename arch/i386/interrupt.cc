#include <stddef.h>
#include <stdint.h>

#include <arch/frame_buffer.h>
#include <arch/interrupt.h>

#define ADD_ISR_HANDLER(NUM, SEL, FLAGS) \
    set_idt_gate(NUM, (uint32_t)&isr##NUM, SEL, FLAGS);

extern "C" void load_idt(uint32_t idt_ptr);
extern "C" void remap_irq();
extern "C" void irq_eoi_master();
extern "C" void irq_eoi_slave();

// Assembly ISRs
extern "C" void isr0();
extern "C" void isr1();
extern "C" void isr2();
extern "C" void isr3();
extern "C" void isr4();
extern "C" void isr5();
extern "C" void isr6();
extern "C" void isr7();
extern "C" void isr8();
extern "C" void isr9();
extern "C" void isr10();
extern "C" void isr11();
extern "C" void isr12();
extern "C" void isr13();
extern "C" void isr14();
extern "C" void isr15();
extern "C" void isr16();
extern "C" void isr17();
extern "C" void isr18();
extern "C" void isr19();
extern "C" void isr20();
extern "C" void isr21();
extern "C" void isr22();
extern "C" void isr23();
extern "C" void isr24();
extern "C" void isr25();
extern "C" void isr26();
extern "C" void isr27();
extern "C" void isr28();
extern "C" void isr29();
extern "C" void isr30();
extern "C" void isr31();
extern "C" void isr32();
extern "C" void isr33();

namespace kernel {
namespace interrupt {
    static const size_t NUM_ENTRIES = 256;
    static const size_t NUM_EXCEPTIONS = 32;
    static const size_t NUM_IRQS = 16;

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
    Interrupt_handler handlers[NUM_ENTRIES] = {0};

    static inline bool is_exception(uint32_t int_num) {
        return int_num < NUM_EXCEPTIONS;
    }

    static inline uint32_t to_irq(uint32_t int_num) {
        return int_num - NUM_EXCEPTIONS;
    }

    static inline bool is_irq(uint32_t int_num) {
        return to_irq(int_num) < NUM_IRQS;
    }

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

    extern "C" void handle_interrupt(Interrupt_frame* frame) {
        if (frame->int_num >= NUM_ENTRIES) {
            // TODO: Panic
        }

        Interrupt_handler handler = handlers[frame->int_num];
        if (handler != nullptr) {
            handler(frame);
        }

        kernel::frame_buffer::print("Interrupt: ");
        kernel::frame_buffer::print((int)frame->int_num);
        kernel::frame_buffer::print("\n");

        // Notify master and slave ports that we've handled the irq
        if (is_irq(frame->int_num)) {
            kernel::frame_buffer::print("This is an irq!\n");
            if (to_irq(frame->int_num) >= 8) {
                irq_eoi_slave();
            }
            irq_eoi_master();
        }
    }

    static void init_handlers() {
        // Exception handlers
        ADD_ISR_HANDLER(0, 0x08, 0x8E);
        ADD_ISR_HANDLER(1, 0x08, 0x8E);
        ADD_ISR_HANDLER(2, 0x08, 0x8E);
        ADD_ISR_HANDLER(3, 0x08, 0x8E);
        ADD_ISR_HANDLER(4, 0x08, 0x8E);
        ADD_ISR_HANDLER(5, 0x08, 0x8E);
        ADD_ISR_HANDLER(6, 0x08, 0x8E);
        ADD_ISR_HANDLER(7, 0x08, 0x8E);
        ADD_ISR_HANDLER(8, 0x08, 0x8E);
        ADD_ISR_HANDLER(9, 0x08, 0x8E);
        ADD_ISR_HANDLER(10, 0x08, 0x8E);
        ADD_ISR_HANDLER(11, 0x08, 0x8E);
        ADD_ISR_HANDLER(12, 0x08, 0x8E);
        ADD_ISR_HANDLER(13, 0x08, 0x8E);
        ADD_ISR_HANDLER(14, 0x08, 0x8E);
        ADD_ISR_HANDLER(15, 0x08, 0x8E);
        ADD_ISR_HANDLER(16, 0x08, 0x8E);
        ADD_ISR_HANDLER(17, 0x08, 0x8E);
        ADD_ISR_HANDLER(18, 0x08, 0x8E);
        ADD_ISR_HANDLER(19, 0x08, 0x8E);
        ADD_ISR_HANDLER(20, 0x08, 0x8E);
        ADD_ISR_HANDLER(21, 0x08, 0x8E);
        ADD_ISR_HANDLER(22, 0x08, 0x8E);
        ADD_ISR_HANDLER(23, 0x08, 0x8E);
        ADD_ISR_HANDLER(24, 0x08, 0x8E);
        ADD_ISR_HANDLER(25, 0x08, 0x8E);
        ADD_ISR_HANDLER(26, 0x08, 0x8E);
        ADD_ISR_HANDLER(27, 0x08, 0x8E);
        ADD_ISR_HANDLER(28, 0x08, 0x8E);
        ADD_ISR_HANDLER(29, 0x08, 0x8E);
        ADD_ISR_HANDLER(30, 0x08, 0x8E);
        ADD_ISR_HANDLER(31, 0x08, 0x8E);

        // IRQ handlers
        ADD_ISR_HANDLER(32, 0x08, 0x8E);
        ADD_ISR_HANDLER(33, 0x08, 0x8E);
    }

    void init_idt() {
        idt_ptr.limit = sizeof(idt) - 1;
        idt_ptr.base = (uint32_t)&idt;

        init_handlers();
        remap_irq();

        load_idt((uint32_t)&idt_ptr);
        enable_interrupts();
    }

    // TODO: Replace the index with an enum
    // Use the enum to find the index in a lookup table?
    // Would this really be necessary?
    void add_handler(size_t index, Interrupt_handler f) {
        if (index < NUM_ENTRIES) {
            handlers[index] = f;
        } else {
            // TODO: Panic?
        }
    }
}
}
