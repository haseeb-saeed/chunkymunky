PIC_MASTER      equ 0x20
PIC_MASTER_DATA equ (PIC_MASTER + 1)
OFFSET_MASTER   equ 0x20
PIC_SLAVE       equ 0xA0
PIC_SLAVE_DATA  equ (PIC_SLAVE + 1)
OFFSET_SLAVE    equ 0x28

global remap_irq
remap_irq:
    ; Save the interrupt masks
    in al, PIC_MASTER_DATA
    mov cl, al
    in al, PIC_SLAVE_DATA
    mov ch, al

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

    ; Restore interrupt masks
    mov al, cl
    out PIC_MASTER_DATA, al
    mov al, ch
    out PIC_SLAVE_DATA, al

    ret
