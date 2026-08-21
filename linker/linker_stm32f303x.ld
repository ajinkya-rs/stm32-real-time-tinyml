/*
 * =============================================================================
 * File:    linker.ld
 * Brief:   Memory layout for a minimal bare-metal STM32F3 firmware.
 *
 * Places the vector table and program instructions in internal flash.
 * Defines the initial stack-pointer value at the top of internal RAM.
 * =============================================================================
 */

/* Available memory regions of this STM32F3.
 *
 * Attributes:
 *   r = readable
 *   w = writable
 *   x = executable
 */
MEMORY
{
    FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 256K
    RAM   (rwx) : ORIGIN = 0x20000000, LENGTH = 48K
}

/*
 * Symbol used by startup.s as the initial Main Stack Pointer (MSP).
 * The stack starts at the end of RAM and grows toward lower addresses.
 */
_end_address_of_stack = ORIGIN(RAM) + LENGTH(RAM);

/*
 * Define output sections and place them in memory regions.
 */
SECTIONS
{
    /*
     * Place the Cortex-M vector table first in flash.
     *
     * After reset, the CPU reads the initial MSP and Reset_Handler
     * address from the first two words at 0x08000000.
     */
    .isr_vector :
    {
        KEEP(*(.isr_vector)) /* Keep vector table; prevent linker removal. */
    } > FLASH

    /*
     * Store program instructions in flash memory.
     *
     * The .text and .text.* patterns collect executable code sections from
     * all linked object files.
     */
    .text :
    {
        *(.text)
        *(.text*)
    } > FLASH
}