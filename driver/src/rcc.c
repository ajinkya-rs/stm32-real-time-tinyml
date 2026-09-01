/**
 ******************************************************************************
 * @file    rcc.c
 * @brief   RCC driver implementation for the STM32F303xC.
 *
 * @details
 * This file implements the RCC driver APIs used to control peripheral
 * clock enable functions.
 *
 * The current implementation provides GPIO AHB clock control through
 * the RCC AHBENR register.
 *
 * @author  Ajinkya Shinde
 * @date    2026-08-31
 ******************************************************************************
 */

#include "rcc.h"
#include "stm32f303xc.h"


/* ***************************************************************************
 *                           GPIO CLOCK CONTROL
 * ************************************************************************* */

void RCC_GPIO_EnableClock(uint32_t gpio_ahbenr_mask)
{
    RCC->AHBENR |= gpio_ahbenr_mask;
}