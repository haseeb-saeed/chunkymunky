#ifndef __I386_INTERRUPT_TYPE_H__
#define __I386_INTERRUPT_TYPE_H__

#include <stddef.h>
#include <stdint.h>

#include <kernel/enum.h>

namespace Arch {
namespace Interrupt {
    KENUM(Interrupt_type, size_t) {
        PAGE_FAULT = 14,
        PIT = 32,
        KEYBOARD = 33,
    };
}
}

#endif // __I386_INTERRUPT_TYPE_H__
