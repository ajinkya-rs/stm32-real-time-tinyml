/* Available memory regions of the STM32F303 */
MEMORY
{
    /* NAME (ATTRIBUTES: r = readable, w = writable, x = executable)
     * ORIGIN = address, LENGTH = size */
    FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 256K
    RAM   (rwx) : ORIGIN = 0x20000000, LENGTH = 48K
}

/* Placement of sections in memory */
SECTIONS
{
    /* Output '.text' section in FLASH */
    .text :
    {
        /* Collect all input '.text' sections from object files.
         * '.text' contain executable program instructions. */
        *(.text)
    } > FLASH
}