#ifndef __KERNEL_SPINLOCK_H__
#define __KERNEL_SPINLOCK_H__

namespace Kernel {
    class Spinlock {
        volatile int lock;
        unsigned irq_state;

    public:
        Spinlock();
        void acquire();
        void release();

        inline bool is_acquired() {
            return lock != 0;
        }
    };
}

#endif // __KERNEL_SPINLOCK_H__
