/*
 * =============================================================================
 * File:    main.c
 * Brief:   Minimal bare-metal application entry point for STM32F3.
 *
 * Called from Reset_Handler after startup initialization:
 *
 *   - .data initialization (FLASH → RAM)
 *
 * =============================================================================
 */

/*
 * .data test variable.
 *
 * Initial value is stored in FLASH.
 * Startup copies it to RAM before main().
 * Volatile keeps it available for debugger verification.
 */
volatile unsigned int test_data = 0x12345678;

/*
 * .bss test variable.
 *
 * No initial value, so startup clears it to zero before main().
 * Volatile keeps it available for debugger verification.
 */
volatile unsigned int test_bss;


int main(void)
{
    /*
     * Application code runs here after startup is complete.
     */
    while (1) {

    }

    return 0;
}