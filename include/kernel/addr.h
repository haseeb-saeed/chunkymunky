#ifndef __KERNEL_ADDR_H__
#define __KERNEL_ADDR_H__

namespace Kernel {
    struct Kernel_addr {
        unsigned vaddr_start;
        unsigned vaddr_end;
        unsigned base;
        unsigned physical_start;
    };
}

#endif // __KERNEL_ADDR_H__
