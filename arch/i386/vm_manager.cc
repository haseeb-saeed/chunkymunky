#include <limits.h>
#include <stddef.h>
#include <stdint.h>

#include <multiboot.h>

#include <arch/interrupt.h>
#include <arch/memory.h>
#include <kernel/addr.h>
#include <kernel/io.h>
#include <kernel/spinlock.h>

using namespace Arch::Interrupt;
using namespace Kernel;
using namespace Kernel::Io;
using namespace Kernel::Lock;

namespace Arch {
namespace Memory {
namespace Vm_manager {
    namespace {
        class Page_entry {
           uint32_t entry;

        public:
           enum Flags {
                PRESENT         = 0x001,
                WRITEABLE       = 0x002,
                USER_PAGE       = 0x004,
                WRITE_THROUGH   = 0x008,
                DISABLE_CACHE   = 0x010,
                ACCESSED        = 0x020,
                DIRTY           = 0x040,    // Page table only
                FOUR_MB         = 0x080,    // Page directory only
                GLOBAL          = 0x100,    // Page table only
            };

            Page_entry(Paddr paddr, uint16_t flags = 0) {
                entry = paddr;
                entry |= flags & 0x1FF;
            }

            inline bool is_present() const {
                return entry & PRESENT;
            }

            inline bool is_writeable() const {
                return entry & WRITEABLE;
            }

            inline bool is_user_page() const {
                return entry & USER_PAGE;
            }

            inline bool is_write_through() const {
                return entry & WRITE_THROUGH;
            }

            inline bool cache_disabled() const {
                return entry & DISABLE_CACHE;
            }

            inline bool is_accessed() const {
                return entry & ACCESSED;
            }

            inline bool is_dirty() const {
                return entry & DIRTY;
            }

            inline bool is_four_mb() const {
                return entry & FOUR_MB;
            }

            inline bool is_global() const {
                return entry & GLOBAL;
            }

            inline Paddr get_paddr() const {
                return entry & 0xFFFFFE00;
            }
        };

        Spinlock lock;
        int kernel_start;
        Page_entry* const CURRENT_PAGE_DIR =
            (Page_entry*)(((NUM_DIR_ENTRIES - 1) << 22) | ((NUM_DIR_ENTRIES - 1) << 12));

        Vaddr to_vaddr(size_t table_num) {
            return ((NUM_DIR_ENTRIES - 1) << 22) | (table_num << 12);
        }

        int get_dir_index(Vaddr vaddr) {
            return (vaddr >> 22) & 0x3FF;
        }

        int get_table_index(Vaddr vaddr) {
            return (vaddr >> 12) & 0x3FF;
        }

        void page_fault_handler(Interrupt_frame* frame) {
            klog(Log_type::PAGING, "Got a page fault with code 0x%x\n", frame->err_code);
        }
    }

    void init(Kernel_addr* kaddr) {
        auto paddr = Pm_manager::alloc_frame();
        if (paddr == 0) {
            kpanic("Unable to allocate frame for kernel page table\n");
        }

        // Allocate a page table
        auto kpage_dir = CURRENT_PAGE_DIR;
        kernel_start = kaddr->base / (NUM_TABLE_ENTRIES * PAGE_SIZE);
        auto frame_base = kpage_dir[kernel_start].get_paddr();
        kpage_dir[kernel_start] = Page_entry(paddr, Page_entry::PRESENT | Page_entry::WRITEABLE);

        klog(Log_type::PAGING, "Kernel page table created at 0x%x\n", paddr);

        // Move the 4MB kernel page into smaller 4KB pages
        Page_entry* page_table = (Page_entry*)to_vaddr(kernel_start);
        for (auto i = 0; i < NUM_TABLE_ENTRIES; ++i) {
            page_table[i] = Page_entry(frame_base + i * PAGE_SIZE, Page_entry::PRESENT | Page_entry::WRITEABLE);
        }

        // Add the page fault handler
        add_handler(Interrupt_type::PAGE_FAULT, &page_fault_handler);
        klog(Log_type::INIT, "Virtual memory manager initialized\n");
    }

    Vaddr map_page(Vaddr vaddr, Paddr paddr) {
        auto dir_index = get_dir_index(vaddr);
        auto table_index = get_table_index(vaddr);
        Lock_guard(lock);

        if (!CURRENT_PAGE_DIR[dir_index].is_present()) {
            auto frame = Pm_manager::alloc_frame();

            // If we cannpt create a page table
            if (frame == 0) {
                return 0;
            }

            CURRENT_PAGE_DIR[dir_index] = Page_entry(frame, Page_entry::PRESENT | Page_entry::WRITEABLE);
        }

        auto table = (Page_entry*)to_vaddr(dir_index);

        if (table[table_index].is_present()) {
            kpanic("Page already mapped to 0x%x\n", vaddr);
        } else {
            table[table_index] = Page_entry(paddr, Page_entry::PRESENT | Page_entry::WRITEABLE);
        }

        return vaddr;
    }

    Paddr unmap_page(Vaddr vaddr) {
        auto dir_index = get_dir_index(vaddr);
        auto table_index = get_table_index(vaddr);
        Lock_guard(lock);

        if (!CURRENT_PAGE_DIR[dir_index].is_present()) {
            kpanic("Unmapping page from missing table 0x%x\n", vaddr);
        }

        auto table = (Page_entry*)to_vaddr(dir_index);
        if (!table[table_index].is_present()) {
            kpanic("Unmapping page that is not present 0x%x\n", vaddr);
        }

        // Unmap page
        // It's up to the caller to free the frame
        // TODO: Invalidate TLB entry
        auto paddr = table[table_index].get_paddr();
        table[table_index] = Page_entry(0);
        return paddr;
    }

    Paddr to_paddr(Vaddr vaddr) {
        auto dir_index = get_dir_index(vaddr);
        auto page_table = (Page_entry*)to_vaddr(CURRENT_PAGE_DIR[dir_index].get_paddr());
        auto table_index = get_table_index(vaddr);

        return page_table[table_index].get_paddr();
    }
}
}
}
