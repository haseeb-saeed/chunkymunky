; Interrupts that do not have an error code
%macro isr_noerror 1
    global isr%1
    isr%1:
        push 0
        push %1
        jmp call_interrupt_handler
%endmacro

; Interrupts that have an error code
%macro isr_error 1
    global isr%1
    isr%1:
        push %1
        jmp call_interrupt_handler
%endmacro

isr_noerror 0   ; Divide by zero
isr_noerror 1   ; Debug
isr_noerror 2   ; Non-maskable interrupt
isr_noerror 3   ; Breakpoint
isr_noerror 4   ; Overflow
isr_noerror 5   ; Bound range exceeded
isr_noerror 6   ; Invalid opcode
isr_noerror 7   ; Device not available

isr_error 8     ; Double fault

isr_noerror 9   ; Coprocessor segment overrun

isr_error 10    ; Invalid TSS
isr_error 11    ; Segment not present
isr_error 12    ; Stack segment fault
isr_error 13    ; General protection fault
isr_error 14    ; Page fault

isr_noerror 15  ; Reserved
isr_noerror 16  ; Floating-point exception

isr_error 17    ; Alignment check

isr_noerror 18  ; Machine check
isr_noerror 19  ; SIMD floating-point exception
isr_noerror 20  ; Virtualization exception
isr_noerror 21  ; Reserved
isr_noerror 22  ; Reserved
isr_noerror 23  ; Reserved
isr_noerror 24  ; Reserved
isr_noerror 25  ; Reserved
isr_noerror 26  ; Reserved
isr_noerror 27  ; Reserved
isr_noerror 28  ; Reserved
isr_noerror 29  ; Reserved

isr_error 30    ; Security exception

isr_noerror 31  ; Reserved

isr_noerror 32  ; PIT
isr_noerror 33  ; Keyboard

; Save registers and call C++ handler
call_interrupt_handler:
    pusha
    push ds
    push es
    push fs
    push gs

    mov eax, esp
    push eax

    extern handle_interrupt
    call handle_interrupt

    add esp, 0x4

    pop gs
    pop fs
    pop es
    pop ds
    popa

    add esp, 0x8
    iret

global enable_interrupts
enable_interrupts:
    sti
    ret

global disable_interrupts
disable_interrupts:
    cli
    ret
