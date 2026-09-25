/**
 * ****************************************************************************
 * @file    nvic.c
 * @brief   NVIC driver implementation for the STM32F303xC.
 *
 * @details
 * Implements NVIC interrupt enable and priority configuration 
 * for external interrupts.
 *
 * @author  Ajinkya Shinde
 * @date    2026-09-21
 * ****************************************************************************
 */

#include "nvic.h"
#include "core_cm4.h"

void NVIC_EnableIRQ(IRQn_Type irq_number)
{
    /* Select the ISER register containing the requested IRQ bit. */
    uint8_t register_index = irq_number / 32U;

    /* Select the IRQ bit within the ISER register. */
    uint8_t bit_position = irq_number % 32U;

    /* Enable the requested external interrupt. */
    NVIC->ISER[register_index] |= (1UL << bit_position);
}


void NVIC_SetPriority(IRQn_Type irq_number, uint8_t priority)
{
    if (priority > 15U)
    {
        return;
    }

    /* Store the 4-bit priority value in the NVIC IPR field [7:4]. */
    NVIC->IPR[irq_number] = (uint8_t)(priority << 4U);
}