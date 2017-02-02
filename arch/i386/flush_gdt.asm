global flush_gdt
flush_gdt:
    ;cli     ; Prevents triple faulting
    mov eax, [esp + 4]
    lgdt [eax]
    jmp 0x08:.flush_cs

.flush_cs:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    ;sti     ; Re-enable the disabled interrupts
    ret
