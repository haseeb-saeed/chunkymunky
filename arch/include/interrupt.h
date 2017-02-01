#ifndef __INTERRUPT_H__
#define __INTERRUPT_H__

namespace kernel {
namespace interrupt {
    void init_idt();
    void init_handlers();
}
}

#endif // __INTERRUPT_H__
