#ifndef __FRAME_BUFFER_H__
#define __FRAME_BUFFER_H__

namespace kernel {
namespace frame_buffer {
    void clear();
    void print(char c);
    void print(const char* str);
}
}

#endif // __FRAME_BUFFER_H__
