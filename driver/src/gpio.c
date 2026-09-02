/**
 ******************************************************************************
 * @file    gpio.c
 * @brief   GPIO driver implementation for the STM32F303xC.
 *
 * @details
 * This file implements the GPIO driver APIs used to configure and control
 * individual GPIO pins.
 *
 * The driver provides functions for:
 * - GPIO pin initialization
 * - GPIO input reading
 * - GPIO output writing
 * - GPIO output toggling
 *
 * The implementation accesses the STM32 GPIO registers through the
 * GPIO_TypeDef register map defined in stm32f303xc.h.
 *
 * @note
 * The GPIO peripheral clock must be enabled before accessing the
 * corresponding GPIO registers.
 *
 * @author  Ajinkya Shinde
 * @date    2026-08-31
 ******************************************************************************
 */

#include "gpio.h"
#include "stm32f303xc.h"



void GPIO_Init(GPIO_TypeDef *GPIOx,
               GPIO_Pin_t gpio_pin,
               GPIO_Mode_t mode,
               GPIO_OutputType_t output_type,
               GPIO_Speed_t speed,
               GPIO_Pull_t pull)
{
    /*
     * MODER and PUPDR use two bits for each GPIO pin.
     * Therefore, the starting bit position of the field is:
     *
     *     pin number × 2
     *
     * Example:
     *     GPIO pin 5 → bits [11:10]
     */
    uint32_t shift = (uint32_t)gpio_pin * 2U;


    /* 
     * Clear the existing two-bit fields of MODER first because the new mode can
     * contain either 0 or 1 in either bit position.
     */
    GPIOx->MODER &= ~(3UL << shift);
    GPIOx->MODER |= ((uint32_t)mode << shift);


    /* 
     * OTYPER[n] controls the output type of GPIO pin n:
     * Clear the existing bit first so that the requested value can be
     * correctly written whether it is 0 or 1.
     */
    GPIOx->OTYPER &= ~(1UL << gpio_pin);
    GPIOx->OTYPER |= ((uint32_t)output_type << gpio_pin);

    /*
     * OSPEEDR[2n+1:2n] configures the output speed
     * Clear the exisiting two-bit field before writing the new configuration
    */
    GPIOx->OSPEEDR &= ~(3UL << shift);
    GPIOx->OSPEEDR |= ((uint32_t)speed << shift);


    /* 
     * PUPDR[2n+1:2n] controls the internal pull configuration:
     * Clear the existing two-bit field before writing the new configuration.
     */
    GPIOx->PUPDR &= ~(3UL << shift);
    GPIOx->PUPDR |= ((uint32_t)pull << shift);
}

uint8_t GPIO_ReadPin(GPIO_TypeDef *GPIOx,
                     GPIO_Pin_t gpio_pin)
{
    /*
     * IDR contains one bit for each GPIO pin.
     *
     * Shift the requested pin to bit 0 and mask all other bits.
     * The returned value is therefore always either 0 or 1.
     */
    return (uint8_t)((GPIOx->IDR >> gpio_pin) & 1U);
}

void GPIO_WritePin(GPIO_TypeDef *GPIOx,
                   GPIO_Pin_t gpio_pin,
                   uint8_t value)
{
    if (value != 0U)
    {
        /*
         * Write 1 to BSRR bit [0:15] to SET the corresponding GPIO pin.
         */
        GPIOx->BSRR = (1UL << gpio_pin);
    }
    else
    {
        /*
         * Write 1 to BSRR bit [16:31] to RESET the corresponding GPIO pin.
         *
         * Adding 16 moves the pin position from the SET half of BSRR
         * to the RESET half.
         */
        GPIOx->BSRR = (1UL << (gpio_pin + 16U));
    }
}


void GPIO_TogglePin(GPIO_TypeDef *GPIOx,
                    GPIO_Pin_t gpio_pin)
{
    /*
     * ODR represents the current output data state of the GPIO pin.
     *
     * If the output is HIGH, reset it.
     * If the output is LOW, set it.
     */
    if ((GPIOx->ODR & (1UL << gpio_pin)) != 0U)
    {
        /* Pin is HIGH: reset it using the RESET half of BSRR. */
        GPIOx->BSRR = (1UL << (gpio_pin + 16U));
    }
    else
    {
        /* Pin is LOW: set it using the SET half of BSRR. */
        GPIOx->BSRR = (1UL << gpio_pin);
    }
}