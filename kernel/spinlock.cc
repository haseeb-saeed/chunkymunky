#include <stddef.h>
#include <stdint.h>

#include <arch/interrupt.h>
#include <kernel/spinlock.h>

using namespace Arch::Interrupt;

namespace Kernel {
namespace Lock {
    Spinlock::Spinlock(): lock(0) {}

    void Spinlock::acquire() {
        // Save whether irqs are enabled or not
        unsigned state = save_irqs();
        disable_interrupts();

        // If we cannot acquire it, spin until it is free
        // Then try to acquire it again
        while (!__sync_bool_compare_and_swap(&this->lock, 0, 1)) {
            restore_irqs(state);
            while (this->is_acquired()) {}
            disable_interrupts();
        }

        __sync_synchronize();

        this->irq_state = state;
    }

    void Spinlock::release() {
        unsigned state = this->irq_state;

        __sync_synchronize();

        this->lock = 0;
        restore_irqs(state);
    }

    Spinlock_guard::Spinlock_guard(Spinlock& lock): lock(lock) {
        lock.acquire();
    }

    Spinlock_guard::~Spinlock_guard() {
        lock.release();
    }
}
}
