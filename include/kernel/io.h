#ifndef __KERNEL_IO_H__
#define __KERNEL_IO_H__

namespace Kernel {
namespace Io {
    enum class Log_type {
        INIT,
        WARNING,
        ERROR,
        KEYBOARD,
        PAGING,
    };

    int kprintf(const char* fmt, ...);
    void klog(Log_type type, const char* fmt, ...);
    void kpanic(const char* fmt, ...);
}
}

#endif // __KERNEL_IO_H__
