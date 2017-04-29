#ifndef __ARCH_MEMORY_H__
#define __ARCH_MEMORY_H__

#if defined(__i386__)
#include <arch/i386/memory.h>
#else
#error "No memory.h for specified platform"
#endif

#include <limits.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <multiboot.h>

#include <kernel/addr.h>

namespace Arch {
namespace Memory {

    typedef uintptr_t Vaddr;
    typedef uintptr_t Paddr;

    namespace Pm_manager {
        void init(multiboot_info_t* mbd, Kernel::Kernel_addr* kaddr);
        Paddr alloc_frame();
        void free_frame(Paddr paddr);
    }

    namespace Vm_manager {
        void init(Kernel::Kernel_addr* kaddr);
        Vaddr map_page(Vaddr vaddr, Paddr paddr);
        Paddr umap_page(Vaddr vaddr);
        Paddr to_paddr(Vaddr vaddr);
    }
}
}

#endif // _ARCH_MEMORY_H__
