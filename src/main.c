/**
 ******************************************************************************
 * @file    main.c
 * @brief   GPIO LED application entry point for the STM32F303xC.
 *
 * @details
 * This file contains the main application entry point.
 *
 * The application enables the GPIOE peripheral clock, configures the GPIO
 * pin connected to the user LED as a push-pull output with no internal
 * pull-up or pull-down, and continuously toggles the LED.
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
    /* Enable the clock for GPIOE before accessing its registers. */
    RCC_GPIO_EnableClock(RCC_AHBENR_GPIOEEN_Msk);

    /* Configure GPIOE pin 8 as a push-pull output with no pull resistor. */
    GPIO_Init(GPIOE,
              GPIO_PIN_8,
              GPIO_MODE_OUTPUT,
              GPIO_OUTPUT_PUSH_PULL,
              GPIO_PULL_NONE);

    while (1)
    {
        /* Toggle the current output state of the user LED. */
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