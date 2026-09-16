/**
 * ****************************************************************************
 * @file    syscfg.c
 * @brief   SYSCFG driver implementation.
 *
 * @details
 * Implements GPIO port selection for EXTI lines using the SYSCFG EXTICR
 * registers.
 *
 * @author  Ajinkya Shinde
 * @date    2026-09-16
 * ****************************************************************************
 */

#include "syscfg.h"
#include "stm32f303xc.h"

void SYSCFG_EXTI_SetPort(uint8_t exti_line, SYSCFG_EXTI_Port_t port)
{
    /* Check that the EXTI line number is within the valid range 0-15. */
    if (exti_line >= TOTAL_EXTI_LINES)
    {
        return;
    }

    /* Check that the GPIO port value is within the supported range A-F. */
    if (port > EXTI_PORT_F)
    {
        return;
    }

    /* Calculate which EXTICR register contains the selected EXTI line. */
    uint8_t exticr_index = exti_line / EXTI_LINES_PER_REGISTER;
    /* Calculate the starting bit position of the EXTI configuration field. */
    uint8_t bit_position = (exti_line % EXTI_LINES_PER_REGISTER) * EXTI_CONFIGURATION_BITS;

    /* Get the address of the required EXTICR register. */
    volatile uint32_t *exticr = &SYSCFG->EXTICR1 + exticr_index;

     /* Clear the existing 4-bit EXTI configuration field. */
    *exticr &= ~(0x0FU << bit_position);
     /* Write the selected GPIO port into the EXTI configuration field. */
    *exticr |= ((uint32_t)port << bit_position);
}