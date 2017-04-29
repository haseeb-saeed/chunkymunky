#include <stddef.h>
#include <stdint.h>

#include <multiboot.h>

#include <kernel/memory.h>
#include <kernel/spinlock.h>

using namespace Arch::Memory;
using namespace Kernel::Lock;

namespace Kernel {
namespace Memory {
    namespace {
        // TODO: Replace free with some sort of checksum or magic header?
        struct Vaddr_descriptor {
            size_t size;
            bool free;
            Vaddr_descriptor* next;
            Vaddr_descriptor* prev;

            Vaddr_descriptor(size_t size, bool free):
                size(size), free(free) {}
        };

        Vaddr heap_base;
        Vaddr heap_break;
        Vaddr_descriptor* free_list;
        Spinlock lock;

        void add_to_free_list(Vaddr_descriptor* node) {
            Vaddr_descriptor* prev = nullptr;
            Vaddr_descriptor* next = free_list;

            while (next != nullptr && next < node) {
                prev = next;
                next = next->next;
            }

            node->prev = prev;
            node->next = next;
        }

        void merge_free_list(Vaddr_descriptor* node) {
            auto next = node->next;
            auto prev = node->next;
            bool contiguous_next = (uintptr_t)node + node->size == (uintptr_t)next;
            bool contiguous_prev = prev != nullptr && (uintptr_t)prev + prev->size == (uintptr_t)node;

            if (contiguous_next && next->free) {
                node->size += next->size;
                node->next = next->next;
            }

            if (contiguous_prev && prev->free) {
                node->size += prev->size;
                node->prev = prev->prev;
            }

            // TODO: If we've recovered a full page, should we deallocate it?
        }

        Vaddr_descriptor* find_free_node(size_t size) {
            // TODO: First fit or best fit?
            return nullptr;
        }

        void split_node(Vaddr_descriptor* node, size_t size) {
            if (node->size <= size) {
                return;
            }

            auto next_size = node->size - size;
            auto next = (Vaddr_descriptor*)((uintptr_t)node + size);

            *next = Vaddr_descriptor(next_size, true);

            node->size = size;
            next->next = node->next;
            node->next = next;
        }
    }

    void init(multiboot_info_t* mbd, Kernel_addr* kaddr) {
        // Initialize architecture-related memory managers
        Pm_manager::init(mbd, kaddr);
        Vm_manager::init(kaddr);

        // Initialize heap
        heap_base = KERNEL_HEAP_START;
        heap_break = heap_base;
        free_list = nullptr;
    }

    void* kmalloc(size_t num_bytes) {
        if (num_bytes == 0) {
            return nullptr;
        }

        num_bytes += sizeof(Vaddr_descriptor);
        Spinlock_guard guard(lock);

        // Check free list
        auto node = find_free_node(num_bytes);
        if (node != nullptr) {
            split_node(node, num_bytes);
            node->free = false;
        } else {
            // Allocate more heap memory if needed
            if (heap_base + num_bytes > heap_break) {
                if (heap_break == KERNEL_HEAP_END) {
                    return nullptr;
                }

                // Unable to get physical memory
                auto frame = Pm_manager::alloc_frame();
                if (frame == 0) {
                    return nullptr;
                }

                // Unable to map virtual memory
                if (Vm_manager::map_page(heap_break, frame) == 0) {
                    Pm_manager::free_frame(frame);
                    return nullptr;
                }

                // Create a new node and add to the free list
                node = (Vaddr_descriptor*)heap_base;
                *node = Vaddr_descriptor(num_bytes, false);
                add_to_free_list(node);

                // Update the heap's bounds
                heap_base += num_bytes;
                heap_break += PAGE_SIZE;
            }
        }

        return (void*)((uintptr_t)node + sizeof(Vaddr_descriptor));
    }

    void kfree(void* addr) {
        auto node = (Vaddr_descriptor*)((uintptr_t)addr - sizeof(Vaddr_descriptor));
        node->free = true;
        merge_free_list(node);
    }
}
}
