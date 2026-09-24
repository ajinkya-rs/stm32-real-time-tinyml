/**
 * ****************************************************************************
 * @file    nvic.c
 * @brief   NVIC driver implementation for the STM32F303xC.
 *
 * @details
 * Implements NVIC driver api to enbale and set the priority of the EXTI line
 *
 * @author  Ajinkya Shinde
 * @date    2026-09-21
 * ****************************************************************************
 */

#include "nvic.h"
#include "core_cm4.h"

void NVIC_EnableIRQ(IRQn_Type irq_number)
{
    /*
     * The Cortex-M4 NVIC provides Interrupt Set-Enable Registers (ISER),
     * where each ISER register is 32 bits wide and each bit corresponds
     * to one external interrupt position.
     *
     * Therefore, divide the STM32F303 interrupt position by 32 to determine
     * which ISER register contains the enable bit for the requested IRQ.
     *
     * Example:
     *     EXTI0 = 6
     *     6 / 32 = 0  -> NVIC->ISER[0]
     */
    uint8_t register_index = irq_number / 32U;

    /*
     * Each ISER register contains 32 interrupt-enable bits.
     * The remainder after dividing the interrupt position by 32 gives
     * the bit position within the selected ISER register.
     *
     * Example:
     *     EXTI0 = 6
     *     6 % 32 = 6  -> bit 6 of NVIC->ISER[0]
     */
    uint8_t bit_position = irq_number % 32U;

    NVIC->ISER[register_index] |= (1UL << bit_position);
}


void NVIC_SetPriority(IRQn_Type irq_number, uint8_t priority)
{
    if (priority > 15U)
    {
        return;
    }

    NVIC->IPR[irq_number] = (uint8_t)(priority << 4U);
}