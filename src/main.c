/**
 ******************************************************************************
 * @file    main.c
 *
 * @brief   GPIO LED application entry point for the STM32F303xC.
 *
 * @details
 * This file contains the main application entry point.
 *
 * The application enables the GPIOA and GPIOE peripheral clocks and
 * configures the GPIO pins connected to the user LEDs PE8 and PE9 and
 * the USER button on PA0. User LED PE8 toggles continuously, while
 * user LED PE9 toggles only when the USER button is pressed.
 *
 * A simple software busy-wait loop is currently used to create a visible
 * delay between LED state changes. This delay is intended only for the
 * initial GPIO demonstration and is not a precise time-delay mechanism.
 *
 * This software delay should be replaced with a hardware timer or an
 * RTOS timing mechanism when those components are implemented.
 *
 * @author  Ajinkya Shinde
 * @date    2026-08-31
 ******************************************************************************
 */

#include "stm32f303xc.h"
#include "rcc.h"
#include "gpio.h"

int main(void)
{
    /* Enable the clock for GPIOx port before accessing its registers. */
    RCC_GPIO_EnableClock(RCC_AHBENR_GPIOAEN_Msk |
                         RCC_AHBENR_GPIOEEN_Msk);

    /* Configure GPIOE pin 8 as a push-pull output with no pull resistor. */
    GPIO_Init(GPIOE,
              GPIO_PIN_8,
              GPIO_MODE_OUTPUT,
              GPIO_OUTPUT_PUSH_PULL,
              GPIO_PULL_NONE);
    
    /* Configure GPIOE pin 9 as a push-pull output with no pull resistor. */
    GPIO_Init(GPIOE,
              GPIO_PIN_9,
              GPIO_MODE_OUTPUT,
              GPIO_OUTPUT_PUSH_PULL,
              GPIO_PULL_NONE);

    /* Configure GPIOA pin 0 as a push-pull input with no pull resistor. */
    GPIO_Init(GPIOA,
              GPIO_PIN_0,
              GPIO_MODE_INPUT,
              GPIO_OUTPUT_PUSH_PULL,
              GPIO_PULL_NONE);

    while (1)
    {
        if (GPIO_ReadPin(GPIOA, GPIO_PIN_0) != 0U) {
            /* If button is pressed, then toogle the user LED PE9. */
            GPIO_TogglePin(GPIOE, GPIO_PIN_9);
        }

        /* Toggle the current output state of the user LED PE8. */
        GPIO_TogglePin(GPIOE, GPIO_PIN_8);
        
        /*
         * Temporary software delay.
         *
         * 'volatile' prevents the compiler from optimizing away the loop
         * because the counter is considered observable.
         *
         * '100000U' is an unsigned iteration count. It does not represent
         * a fixed time such as milliseconds or microseconds.
         *
         * The actual delay depends on CPU frequency, compiler, optimization
         * level, and generated machine instructions.
         */
        for (volatile uint32_t i = 0U; i < 100000U; i++)
        {
        }
    }
}