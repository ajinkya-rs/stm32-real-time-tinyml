/**
 ******************************************************************************
 * @file    rcc.h
 * @brief   RCC driver interface for the STM32F303xC.
 *
 * @details
 * This file provides the public API for controlling the Reset and Clock
 * Control (RCC) peripheral.
 *
 * The current implementation provides an API to enable the AHB clock
 * for a selected GPIO peripheral.
 *
 * @author  Ajinkya Shinde
 * @date    2026-08-31
 ******************************************************************************
 */

#ifndef RCC_H_
#define RCC_H_

#include <stdint.h>

/* ***************************************************************************
 *                           FUNCTION DECLARATIONS
 * ************************************************************************* */

/**
 * @brief Enable one or more AHB peripheral clocks.
 *
 * @param ahbenr_mask AHB clock-enable bit mask.
 *
 * @note
 * The supplied mask is ORed into RCC AHBENR.
 */
void RCC_AHB_EnableClock(uint32_t ahbenr_mask);

/**
 * @brief Enable one or more APB2 peripheral clocks.
 *
 * @param apb2enr_mask APB2 clock-enable bit mask.
 *
 * @note
 * The supplied mask is ORed into RCC APB2ENR.
 */
void RCC_APB2_EnableClock(uint32_t apb2enr_mask);


#endif /* RCC_H_ */
