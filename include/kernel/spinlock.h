#ifndef __KERNEL_SPINLOCK_H__
#define __KERNEL_SPINLOCK_H__

namespace Kernel {
namespace Lock {
    class Spinlock {
        volatile int lock;
        unsigned irq_state;

        Spinlock(const Spinlock&) = delete;
        Spinlock& operator=(const Spinlock&) = delete;

    public:
        Spinlock();
        void acquire();
        void release();

        inline bool is_acquired() {
            return lock != 0;
        }
    };

    class Spinlock_guard {
        Spinlock& lock;

        Spinlock_guard(const Spinlock_guard&) = delete;
        Spinlock_guard& operator=(const Spinlock_guard&) = delete;

    public:
        Spinlock_guard(Spinlock& lock);
        ~Spinlock_guard();
    };
}
}

#endif // __KERNEL_SPINLOCK_H__
