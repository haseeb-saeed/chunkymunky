#ifndef __KERNEL_IO_H__
#define __KERNEL_IO_H__

namespace kernel {
namespace io {
    enum class Log_type {
        INIT,
        WARNING,
        ERROR,
        KEYBOARD,
    };

    int kprintf(const char* fmt, ...);
    void klog(Log_type type, const char* fmt, ...);
    void kpanic(const char* fmt, ...);
}
}

#endif // __KERNEL_IO_H__
