/* Bootloader starts execution at this symbol */
ENTRY(_start)

SECTIONS
{
    /* Begin putting code at 0x00100000 */
    /* When paging is done, move to 0xC0100000 */
    . = 0x00100000;

    /* Multiboot header and text */
    .text BLOCK(0x1000) : ALIGN(0x1000)
    {
        *(.multiboot)
        *(.text)
    }

    /* Read-only data */
    .rodata BLOCK(0x1000) : ALIGN(0x1000)
    {
        *(.rodata)
    }

    /* Read-write data (initialized) */
    .data BLOCK(0x1000) : ALIGN(0x1000)
    {
        *(.data)
    }

    /* Read-write data (uninitialized) */
    .bss BLOCK(0x1000) : ALIGN(0x1000)
    {
        *(.COMMON)
        *(.bss)
    }
}
