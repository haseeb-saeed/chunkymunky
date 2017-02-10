#ifndef __INTERRUPT_H__
#define __INTERRUPT_H__

#if defined(__i386__)
#include <arch/i386/interrupt_frame.h>
#include <arch/i386/interrupt_type.h>
#else
#error "No interrupt_frame.h for specified platform"
#endif

namespace Kernel {
namespace Interrupt {
    typedef void (*Interrupt_handler)(Interrupt_frame* frame);

    void init();
    void add_handler(Interrupt_type type, Interrupt_handler f);
    extern "C" {
        void enable_interrupts();
        void disable_interrupts();
    }
}
}

#endif // __INTERRUPT_H__
