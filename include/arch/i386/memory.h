#ifndef __I386_MEMORY_H__
#define __I386_MEMORY_H__

namespace Arch {
namespace Memory {
    const int PAGE_SIZE = 0x1000;
    const int NUM_DIR_ENTRIES = 1024;
    const int NUM_TABLE_ENTRIES = 1024;
    const int START_FRAME = NUM_TABLE_ENTRIES * PAGE_SIZE;
}
}

#endif // __I386_MEMORY_H__
