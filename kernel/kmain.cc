#include <stddef.h>
#include <stdint.h>

#include "../arch/frame_buffer.h"

using namespace kernel;

#if defined(__linux__)
#error "You are not using a cross-compiler"
#endif

#if !defined(__i386__)
#error "You must compile with an ix86-elf compiler"
#endif

extern "C"
void kmain() {
    frame_buffer::clear();
    frame_buffer::print("Hello world!\n");
}
