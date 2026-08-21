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
 * After reset, the Cortex-M processor loads these values from flash memory.
 * Reset_Handler provides the first execution point after reset.
 *
 * The linker script places the .isr_vector section at flash address
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

.word   _end_address_of_stack     /* Initial MSP value */
.word   Reset_Handler             /* Reset vector address */


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
 * First instruction executed after reset.
 *
 * Calls the application entry point and then remains in an infinite loop.
 * The loop prevents execution from continuing into unexpected memory.
 */
Reset_Handler:

    bl      main
    b       .