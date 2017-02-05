MBALIGN     equ 1<<0
MEMINFO     equ 1<<1
FLAGS       equ MBALIGN | MEMINFO
MAGIC       equ 0x1BADB002
CHECKSUM    equ -(MAGIC + FLAGS)
STACKSIZE   equ 1 << 14

; Declare a multiboot header
section .multiboot
align 4
    dd MAGIC
    dd FLAGS
    dd CHECKSUM

; Allocate a stack for the kernel
; Stack must be 16-byte aligned
section .bss
align 4
stack_bottom:
resb STACKSIZE
stack_top:

; _start is the entry point
section .text
global _start:function (_start.end - _start)
_start:
    ; Move the stack pointer to the top of the kernel stack
    mov esp, stack_top

    ; Initialize the kernel
    extern kinit
    call kinit

    ; Run global destructors
    extern _init
    call _init

    extern kmain
    call kmain

    ; Run global destructors
    extern _fini
    call _fini

    ; Run an infinite loop when kernel code returns
    cli
.hang:
    hlt
    jmp .hang
.end:
