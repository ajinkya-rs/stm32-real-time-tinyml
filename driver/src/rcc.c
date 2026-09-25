/**
 ******************************************************************************
 * @file    rcc.c
 * @brief   RCC driver implementation for the STM32F303xC.
 *
 * @details
 * This file implements the RCC driver APIs used to control peripheral
 * clock enable functions.
 *
 * The current implementation provides AHB and APB2 clock control through
 * the RCC registers.
 *
 * @author  Ajinkya Shinde
 * @date    2026-08-31
 ******************************************************************************
 */

#include "rcc.h"
#include "stm32f303xc.h"

void RCC_AHB_EnableClock(uint32_t ahbenr_mask)
{
    /* Enable the requested AHB peripheral clocks. */
    RCC->AHBENR |= ahbenr_mask;
}

void RCC_APB2_EnableClock(uint32_t apb2enr_mask)
{
    /* Enable the requested APB2 peripheral clocks. */
    RCC->APB2ENR |= apb2enr_mask;
}