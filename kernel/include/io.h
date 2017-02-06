#ifndef __KERNEL_IO_H__
#define __KERNEL_IO_H__

namespace kernel {
namespace io {
    int kprintf(const char* fmt, ...);
    void kpanic(const char* fmt, ...);      // For non-recoverable errors
    void kerror(const char* fmt, ...);      // For recoverable errors
}
}

#endif // __KERNEL_IO_H__
