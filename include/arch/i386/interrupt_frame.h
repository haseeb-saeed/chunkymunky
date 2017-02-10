#ifndef __I386_INTERRUPT_FRAME_H__
#define __I386_INTERRUPT_FRAME_H__

namespace kernel {
namespace interrupt {
    struct Interrupt_frame {
        uint32_t gs, fs, es, ds;
        uint32_t edi, esi, ebp, useless;
        uint32_t ebx, edx, ecx, eax;
        uint32_t int_num, err_code;
        uint32_t eip, cs, eflags, esp, ss;
    } __attribute__((packed));
}
}

#endif // __I386_INTERRUPT_FRAME_H__
