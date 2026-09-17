/**
 * ****************************************************************************
 * @file    exti.c
 * @brief   EXTI driver implementation for the STM32F303xC.
 *
 * @details
 * Implements external interrupt line configuration and trigger selection
 *
 * @author  Ajinkya Shinde
 * @date    2026-09-17
 * ****************************************************************************
 */

#include "exti.h"
#include "syscfg.h"
#include "stm32f303xc.h"

void EXTI_EnableInterrupt (uint8_t exti_line)
{
    /* Check that the external interrupt line is within the valid range 0-15. */
    if(exti_line > 15U) 
    {
        return;
    }

    /* Enable external interrupt for EXTI line by setting the corresponding bit in the IMR */
    EXTI->IMR1 |= (1UL << exti_line);
}

void EXTI_SelectTrigger(uint8_t exti_line, EXTI_Trigger_t trigger)
{
    /* Check that the external interrupt line is within the valid range 0-15. */
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