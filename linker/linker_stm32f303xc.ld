/*
 * =============================================================================
 * File:    linker_stm32f303xc.ld
 * Brief:   Memory layout for the STM32F303xC bare-metal firmware.
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
    RAM   (rwx) : ORIGIN = 0x20000000, LENGTH = 40K
}

/*
 * Symbol used by startup_stm32f303xc.s as the initial Main Stack Pointer (MSP).
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

    /*
     * .rodata: read-only data.
     *
     * Contains constants and string literals.
     * Stored directly in FLASH because values are not modified at runtime.
     */
    .rodata :
    {
        . = ALIGN(4);

        *(.rodata)
        *(.rodata*)          /* Collect read-only data sections */

        . = ALIGN(4);

    } > FLASH

    /*
     * Contains initialized writable global/static variables.
     *
     * Runtime  : Variable lives in RAM because it must be writable.
     * Load     : Initial value is stored in Flash so it survives reset.
     *
     * Startup copies FLASH (_sidata) → RAM (_sdata to _edata).
     */
    .data :
    {
        . = ALIGN(4);

        _sdata = .;     /* Start of .data in RAM */

        *(.data)
        *(.data*)       /* Collect input .data sections */

        . = ALIGN(4);

        _edata = .;     /* End of .data in RAM */

    } > RAM AT > FLASH

    /*
    * Flash load address of .data initial image.
    *
    * _sidata = Start of Initial Data
    * Used as source address during startup copy.
    */
    _sidata = LOADADDR(.data);

    /*
     * .bss: zero-initialized writable variables.
     *
     * Runtime location: RAM (No Flash load image required.)
     *
     * Startup clears: _sbss → _ebss
     */
    .bss (NOLOAD) :
    {
        . = ALIGN(4);

        _sbss = .;          /* Start of .bss in RAM */

        *(.bss)
        *(.bss*)            /* Collect all input .bss sections from object files */
        *(COMMON)           /* Include uninitialized global variables */

        . = ALIGN(4);

        _ebss = .;          /* End of .bss in RAM */

    } > RAM
}