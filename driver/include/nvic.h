/**
 * ****************************************************************************
 * @file    nvic.h
 * @brief   NVIC driver interface.
 *
 * @details
 * Provides APIs to enable external interrupts and configure 
 * their interrupt priorities in the NVIC.
 *
 * @author  Ajinkya Shinde
 * @date    2026-09-18
 * ****************************************************************************
 */
#ifndef NVIC_H_
#define NVIC_H_

#include <stdint.h>
#include "stm32f303xc.h"

/* ***************************************************************************
 *                        FUNCTION PROTOTYPES
 * ************************************************************************* */
/**
 * @brief Enable an external interrupt in the NVIC.
 *
 * Enables the specified external interrupt by setting its corresponding
 * interrupt enable bit in the NVIC Interrupt Set-Enable Register (ISER).
 *
 * @param[in] irq_number External interrupt number.
 */
void NVIC_EnableIRQ(IRQn_Type irq_number);

/**
 * @brief Configure the interrupt priority for an IRQ.
 *
 * The STM32F303 provides 16 programmable interrupt priority levels,
 * using 4 bits of the interrupt priority field (levels 0 to 15).
 *
 * The priority value is stored in the upper four bits [7:4] of the
 * corresponding NVIC Interrupt Priority Register (IPR) byte.
 * Bits [3:0] are not implemented.
 *
 * Priority level 0 has the highest priority, while priority level 15
 * has the lowest priority.
 *
 * @param[in] irq_number
 * IRQ number to configure.
 * 
 * @param[in] priority
 * Interrupt priority level (0 to 15).
 */
void NVIC_SetPriority(IRQn_Type irq_number, uint8_t priority);

#endif /* NVIC_H_ */