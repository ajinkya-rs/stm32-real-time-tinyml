/*
 * =============================================================================
 * File:    startup.s
 * Brief:   Minimal STM32F3 bare-metal startup code.
 *
 * Defines the initial Cortex-M vector table entries:
 *
 *   1. Initial Main Stack Pointer (MSP)
 *   2. Reset_Handler address
 *
 * After reset, the Cortex-M processor loads these values from Flash memory.
 * Reset_Handler provides the first execution point after reset.
 *
 * The linker script places the .isr_vector section at Flash address
 * 0x08000000.
 * =============================================================================
 */


/*
 * The Cortex-M processor reads the first two 32-bit entries from the vector
 * table after reset:
 *
 *   Word 0: Initial Main Stack Pointer (MSP)
 *   Word 1: Reset_Handler address
 *
 * The "a" flag marks this section as allocatable by the linker.
 */
.section .isr_vector, "a"

.word   _end_address_of_stack      /* Initial MSP value */
.word   Reset_Handler              /* Reset vector address */


/*
 * Executable code section containing startup routines.
 */
.section .text


/*
 * Export Reset_Handler as a function symbol so it can be referenced by
 * the vector table and linker.
 */
.type   Reset_Handler, %function
.global Reset_Handler


/*
 * Reset entry point.
 *
 * First instruction executed after reset.
 * Prepares C runtime memory and starts main().
 */
Reset_Handler:


/*
 * Copy initialized .data values from FLASH to RAM.
 *
 * Source      : initial .data values in FLASH
 * Destination : .data start in RAM
 * End         : end address of .data in RAM
 */
ldr     r0, =_sidata
ldr     r1, =_sdata
ldr     r2, =_edata


copy_data:
    cmp     r1, r2              /* Stop when all .data is copied */
    bcs     data_done

    ldr     r3, [r0]            /* Read word from FLASH */
    str     r3, [r1]            /* Write word to RAM */

    add     r0, r0, #4          /* Next FLASH address */
    add     r1, r1, #4          /* Next RAM address */

    b       copy_data


data_done:


/*
 * C runtime is ready.
 * Enter application code.
 */
bl      main


/*
 * main() should not return.
 * Stay here if it does to avoid executing invalid memory.
 */
b       .
