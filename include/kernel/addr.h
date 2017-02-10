#ifndef __KERNEL_ADDR_H__
#define __KERNEL_ADDR_H__

namespace Kernel {
    struct Kernel_addr {
        unsigned kernel_vaddr_low;
        unsigned kernel_vaddr_high;
        unsigned kernel_paddr_low;
        unsigned kernel_paddr_high;
    };
}

#endif // __KERNEL_ADDR_H__
