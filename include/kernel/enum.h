#ifndef __KERNEL_ENUM_H__
#define __KERNEL_ENUM_H__

#define KENUM(NAME, TYPE)                           \
    enum class NAME : TYPE;                         \
                                                    \
    inline TYPE underlying_type(NAME e) {           \
        return (TYPE)e;                             \
    }                                               \
                                                    \
    enum class NAME : TYPE                          \

#endif // __KERNEL_ENUM_H__
