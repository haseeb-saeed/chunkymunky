PIC_MASTER      equ 0x20
PIC_MASTER_DATA equ (PIC_MASTER + 1)
OFFSET_MASTER   equ 0x20
PIC_SLAVE       equ 0xA0
PIC_SLAVE_DATA  equ (PIC_SLAVE + 1)
OFFSET_SLAVE    equ 0x28

global remap_irq
remap_irq:
    ; Initialize
    mov al, 0x11
    out PIC_MASTER, al
    out PIC_SLAVE, al

    ; Map to new offsets in IDT
    mov al, OFFSET_MASTER
    out PIC_MASTER_DATA, al
    mov al, OFFSET_SLAVE
    out PIC_SLAVE_DATA, al

    ; Tell master about slave PIC
    mov al, 0x4
    out PIC_MASTER_DATA, al
    mov al, 0x2
    out PIC_SLAVE_DATA, al

    ; Set 8086 mode
    mov al, 0x1
    out PIC_MASTER_DATA, al
    out PIC_SLAVE_DATA, al

    ; Set interrupt masks
    xor al, al
    out PIC_MASTER_DATA, al
    out PIC_SLAVE_DATA, al
    ret

; EOI on master PIC
global irq_eoi_master
irq_eoi_master:
    mov al, 0x20
    out PIC_MASTER, al
    ret

; EOI on slave PIC
global irq_eoi_slave
irq_eoi_slave:
    mov al, 0x20
    out PIC_SLAVE, al
    ret

; Gets the input byte from the keyboard IRQ
global get_keyboard_input
get_keyboard_input:
    in al, 0x60
    ret

; Saves EFLAGS
global save_irqs
save_irqs:
    pushfd
    pop eax
    ret

; Restores EFLAGs
global restore_irqs
restore_irqs:
    mov eax, [esp + 4]
    push eax
    popfd
    ret
