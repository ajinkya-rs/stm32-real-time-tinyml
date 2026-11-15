/**
 * ****************************************************************************
 * @file    exti.c
 * @brief   EXTI driver implementation for the STM32F303xC.
 *
 * @details
 * Implements EXTI line interrupt configuration, trigger selection, and
 * interrupt handling.
 *
 * @author  Ajinkya Shinde
 * @date    2026-09-25
 * ****************************************************************************
 */

/* ***************************************************************************
 *                              INCLUDES
 * ************************************************************************* */

#include "exti.h"
#include "syscfg.h"
#include "gpio.h"
#include "stm32f303xc.h"

/* ***************************************************************************
 *                        FUNCTION DEFINITIONS
 * ************************************************************************* */

void EXTI_EnableInterrupt(uint8_t exti_line)
{
    /* Validate the EXTI line number. */
    if (exti_line >= TOTAL_EXTI_LINES)
    {
        return;
    }

    /* Unmask interrupt generation for the selected EXTI line. */
    EXTI->IMR1 |= (1UL << exti_line);
}

void EXTI_SelectTrigger(uint8_t exti_line, EXTI_Trigger_t trigger)
{
    /* Validate the EXTI line number. */
    if (exti_line >= TOTAL_EXTI_LINES)
    {
        return;
    }

    /* Create a mask for the selected EXTI line. */
    uint32_t line_mask = (1UL << exti_line);

    /* Clear both rising and falling trigger configuration. */
    EXTI->RTSR1 &= ~line_mask;
    EXTI->FTSR1 &= ~line_mask;

    /* Select the requested trigger condition. */
    switch (trigger)
    {
        case EXTI_TRIGGER_RISING:
            EXTI->RTSR1 |= line_mask;
            break;

        case EXTI_TRIGGER_FALLING:
            EXTI->FTSR1 |= line_mask;
            break;

        case EXTI_TRIGGER_RISING_FALLING:
            EXTI->RTSR1 |= line_mask;
            EXTI->FTSR1 |= line_mask;
            break;

        default:
            return;
    }
}

void EXTI0_IRQHandler(void)
{
    if (EXTI->PR1 & (1UL << 0))
    {
        /* Clear the EXTI0 pending flag (W1C: write 1 to clear). */
        EXTI->PR1 = (1UL << 0);

        /* Toggle the user LED in response to the EXTI0 event. */
        GPIO_TogglePin(GPIOE, GPIO_PIN_9);
    }
}