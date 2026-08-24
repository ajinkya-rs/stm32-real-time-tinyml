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
volatile static unsigned int test_static_data = 0xA5A5A5A5;

/*
 * .bss test variable.
 *
 * No initial value, so startup clears it to zero before main().
 * Volatile keeps it available for debugger verification.
 */
volatile unsigned int test_bss;

/*
 * .rodata test variable.
 *
 * Constant data is stored directly in FLASH.
 * No startup copy to RAM is required.
 */
const unsigned int device_id = 1234;


/*
 * .rodata string test.
 *
 * String literals are stored in FLASH because
 * they are not modified at runtime.
 */
const char message[] = "Hello STM32";


int main(void)
{
    /*
     * Application code runs here after startup is complete.
     */
    while (1) {

    }

    return 0;
}