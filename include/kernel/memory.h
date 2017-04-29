#ifndef __KERNEL_MEMORY_H__
#define __KERNEL_MEMORY_H__

#include <multiboot.h>

#include <arch/memory.h>

namespace Kernel {
namespace Memory {
    void init(multiboot_info_t* mbd, Kernel::Kernel_addr* kaddr);
    void* kmalloc(size_t num_bytes);
    void kfree(void* addr);
}
}

#endif // __KERNEL_MEMORY_H__
