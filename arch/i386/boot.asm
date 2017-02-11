MBALIGN     equ 1<<0
MEMINFO     equ 1<<1
FLAGS       equ MBALIGN | MEMINFO
MAGIC       equ 0x1BADB002
CHECKSUM    equ -(MAGIC + FLAGS)
STACKSIZE   equ 1 << 14
PAGEDIRSIZE equ 1024
KERNELBASE  equ 0xC0000000
DIRENTRY    equ (KERNELBASE >> 22)

; Defined in linker.ld
extern kernel_vaddr_low
extern kernel_vaddr_high
extern PHYSICAL_START

; Declare a multiboot header
section .multiboot
align 0x4
    dd MAGIC
    dd FLAGS
    dd CHECKSUM

; Allocate a stack for the kernel
; Stack must be 16-byte aligned
section .bss
align 0x4
    stack_bottom:
    resb STACKSIZE
    stack_top:

; Allocate the kernel's page directory
; Page directory must be 4KB aligned
align 0x1000
    global kpage_dir
    resd PAGEDIRSIZE
    kpage_dir:

; _start is the entry point
section .text
global _start:function (_start.end - _start)
_start:
    ; Map the page table
    kpage_dir_paddr equ (kpage_dir - KERNELBASE)
    mov eax, 0x00000083
    mov [kpage_dir_paddr], eax
    mov [kpage_dir_paddr + DIRENTRY * 4], eax

    ; Load the page table
    mov eax, kpage_dir_paddr
    mov cr3, eax

    ; Enable 4MB pages
    mov eax, cr4
    or eax, 0x00000010
    mov cr4, eax

    ; Enable paging
    mov eax, cr0
    or eax, 0x80000000
    mov cr0, eax

    ; Long jump to higher half
    lea eax, [.higher_half]
    jmp eax

.higher_half:
    ; All addresses are now virtual addresses
    ; Remove the identity mapped page entry
    mov dword [kpage_dir], 0x0
    invlpg [0]

    ; Move the stack pointer to the top of the kernel stack
    mov esp, stack_top

    ; Push the kernel address information on the stack
    push PHYSICAL_START
    push KERNELBASE
    push kernel_vaddr_high
    push kernel_vaddr_low
    push esp

    ; Push the multiboot info on the stack
    add ebx, KERNELBASE
    push ebx

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
