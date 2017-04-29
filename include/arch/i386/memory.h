#ifndef __I386_MEMORY_H__
#define __I386_MEMORY_H__

namespace Arch {
namespace Memory {
    const int PAGE_SIZE = 0x1000;
    const int NUM_DIR_ENTRIES = 1024;
    const int NUM_TABLE_ENTRIES = 1024;
    const unsigned START_FRAME = NUM_TABLE_ENTRIES * PAGE_SIZE;
    const unsigned KERNEL_HEAP_START = 0xC1000000;
    const unsigned KERNEL_HEAP_END = 0xF1000000;
}
}

#endif // __I386_MEMORY_H__
