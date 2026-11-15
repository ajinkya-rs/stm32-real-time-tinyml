/**
 ******************************************************************************
 * @file    rcc.h
 * @brief   RCC driver interface for the STM32F303xC.
 *
 * @details
 * This file provides the public API for controlling the Reset and Clock
 * Control (RCC) peripheral.
 *
 * The current implementation provides APIs to enable AHB and APB2 
 * peripheral clocks.
 *
 * @author  Ajinkya Shinde
 * @date    2026-08-31
 ******************************************************************************
 */

#ifndef RCC_H_
#define RCC_H_

/* ***************************************************************************
 *                              INCLUDES
 * ************************************************************************* */

#include <stdint.h>

/* ***************************************************************************
 *                        FUNCTION PROTOTYPES
 * ************************************************************************* */

/**
 * @brief Enable one or more AHB peripheral clocks.
 *
 * @param[in] ahbenr_mask AHB clock-enable bit mask.
 *
 * @note
 * The supplied mask is ORed into RCC AHBENR.
 */
void RCC_AHB_EnableClock(uint32_t ahbenr_mask);

/**
 * @brief Enable one or more APB2 peripheral clocks.
 *
 * @param[in] apb2enr_mask APB2 clock-enable bit mask.
 *
 * @note
 * The supplied mask is ORed into RCC APB2ENR.
 */
void RCC_APB2_EnableClock(uint32_t apb2enr_mask);


#endif /* RCC_H_ */
