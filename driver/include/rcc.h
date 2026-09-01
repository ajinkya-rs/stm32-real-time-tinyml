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
 *                           GPIO CLOCK CONTROL
 * ************************************************************************* */

/**
 * @brief Enable the AHB clock for one or more GPIO peripherals.
 *
 * @param  gpio_ahbenr_mask   GPIO AHB clock-enable bit mask defined by the device header.
 *
 * @note
 * The supplied mask is ORed into the RCC AHBENR register.
 */
void RCC_GPIO_EnableClock(uint32_t gpio_ahbenr_mask);


#endif /* RCC_H_ */
