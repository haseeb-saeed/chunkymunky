/* Bootloader starts execution at this symbol */
ENTRY(_start)

SECTIONS
{
    /* Begin putting code at 0x00100000 */
    /* When paging is done, move to 0xC0100000 */
    KERNEL_BASE = 0xC0000000;
    . = 0x00100000 + KERNEL_BASE;

    /* Multiboot header and text */
    .text ALIGN(0x1000) : AT(ADDR(.text) - KERNEL_BASE)
    {
        *(.multiboot)
        *(.text)
    }

    /* Read-only data */
    .rodata ALIGN(0x1000) : AT(ADDR(.rodata) - KERNEL_BASE)
    {
        *(.rodata)
    }

    /* Read-write data (initialized) */
    .data ALIGN(0x1000) : AT(ADDR(.data) - KERNEL_BASE)
    {
        *(.data)
    }

    /* Read-write data (uninitialized) */
    .bss ALIGN(0x1000) : AT(ADDR(.bss) - KERNEL_BASE)
    {
        *(.COMMON)
        *(.bss)
    }
}
