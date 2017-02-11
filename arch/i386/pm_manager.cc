#include <limits.h>
#include <stddef.h>
#include <stdint.h>

#include <multiboot.h>

#include <arch/memory.h>
#include <kernel/addr.h>
#include <kernel/io.h>

using namespace Kernel;
using namespace Kernel::Io;

namespace Arch {
namespace Memory {
namespace Pm_manager {
    namespace {
        // TODO: Use a stack allocator
        const unsigned EMPTY_BLOCK = 0;
        const unsigned FULL_BLOCK = ~EMPTY_BLOCK;
        unsigned* bitmap;
        size_t num_blocks;
        size_t num_frames;
        size_t frames_per_block;
        size_t free_block;

        inline size_t get_block(Paddr paddr) {
            paddr -= START_FRAME;
            return (paddr / PAGE_SIZE) / frames_per_block;
        }

        inline int get_offset(Paddr paddr) {
            paddr -= START_FRAME;
            return (paddr / PAGE_SIZE) % frames_per_block;
        }

        inline Paddr to_paddr(size_t block, int offset) {
            Paddr paddr = block * frames_per_block + offset;
            return (paddr * PAGE_SIZE) + START_FRAME;
        }
    }

    void init(multiboot_info_t* mbd, Kernel_addr* kaddr) {
        if ((mbd->flags & 0x1) == 0) {
            kpanic("Unable to get amount of memory from multiboot info\n");
        }

        // Get the number of frames
        num_frames = ((mbd->mem_upper * 1024) - START_FRAME) / PAGE_SIZE;
        frames_per_block = sizeof(*bitmap) * CHAR_BIT;
        num_blocks = num_frames / frames_per_block;
        free_block = 0;

        // Allocate memory for the bitmap (align to 4 bytes)
        bitmap = (unsigned*)((kaddr->vaddr_end + 3) & ~0x3);
        memset(bitmap, EMPTY_BLOCK, num_blocks * sizeof(*bitmap));

        // Update the ending virtual address for the kernel
        ptrdiff_t adjustment = bitmap - (unsigned*)kaddr->vaddr_end;
        adjustment += num_blocks * sizeof(*bitmap);
        kaddr->vaddr_end += adjustment;

        klog(Log_type::INIT, "Physical memory manager initialized\n");
    }

    Paddr alloc_frame() {
        Paddr paddr = 0;
        auto i = free_block;
        int offset;

        // Find an unallocated frame
        for (; i < num_blocks; ++i) {
            if (bitmap[i] != FULL_BLOCK) {
                offset = __builtin_ffs(~bitmap[i]) - 1;
                paddr = to_paddr(i, offset);
                break;
            }
        }

        // Set found frame to allocated
        if (paddr != 0) {
            bitmap[i] |= (1 << offset);
            if (bitmap[i] == FULL_BLOCK) {
                free_block = i + 1;
            }
        }

        klog(Log_type::PAGING, "Allocating frame 0x%x\n", paddr);
        return paddr;
    }

    void free_frame(Paddr paddr) {
        auto block = get_block(paddr);
        auto mask = 1 << get_offset(paddr);

        // Check the frame is allocated
        if ((bitmap[block] & mask) == EMPTY_BLOCK) {
            kpanic("Attempting to free unallocated frame 0x%x\n", paddr);
        } else {
            bitmap[block] &= ~mask;
            if (block < free_block) {
                free_block = block;
            }
        }

        klog(Log_type::PAGING, "Freeing frame 0x%x\n", paddr);
    }
}
}
}
