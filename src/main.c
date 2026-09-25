/**
 * ****************************************************************************
 * @file    main.c
 * @brief   GPIO and external interrupt demonstration for the STM32F303xC.
 *
 * @details
 * Configures GPIOA pin 0 as the external interrupt source and GPIOE pin 9
 * as the user LED output. GPIOA pin 0 is connected to EXTI line 0 through
 * SYSCFG and configured to generate an interrupt on a rising edge.
 *
 * The EXTI0 interrupt is enabled through the NVIC. When the USER button
 * generates the configured edge, EXTI0_IRQHandler() handles the interrupt
 * and toggles the user LED on PE9.
 *
 * The main loop does not poll the USER button. The LED is controlled by
 * the EXTI interrupt handler.
 *
 * @author  Ajinkya Shinde
 * @date    2026-09-25
 * ****************************************************************************
 */

#include "stm32f303xc.h"
#include "rcc.h"
#include "gpio.h"
#include "syscfg.h"
#include "exti.h"
#include "nvic.h"

int main(void)
{
    /* Enable the GPIOA and GPIOE peripheral clocks. */
    RCC_AHB_EnableClock(RCC_AHBENR_GPIOAEN_Msk |
                        RCC_AHBENR_GPIOEEN_Msk);

    /* Enable the SYSCFG peripheral clock. */
    RCC_APB2_EnableClock(RCC_APB2ENR_SYSCFGEN_Msk);

    /* Configure GPIOE pin 9 as a push-pull output for the user LED. */
    GPIO_Init(GPIOE,
              GPIO_PIN_9,
              GPIO_MODE_OUTPUT,
              GPIO_OUTPUT_PUSH_PULL,
              GPIO_SPEED_LOW,
              GPIO_PULL_NONE);

    /* Configure GPIOA pin 0 as the USER button input. */
    GPIO_Init(GPIOA,
              GPIO_PIN_0,
              GPIO_MODE_INPUT,
              GPIO_OUTPUT_PUSH_PULL,
              GPIO_SPEED_LOW,
              GPIO_PULL_NONE);

    /* Connect GPIOA pin 0 to EXTI line 0 through SYSCFG. */
    SYSCFG_EXTI_SetPort(0U, EXTI_PORT_A);

    /* Configure EXTI line 0 to trigger on a rising edge. */
    EXTI_SelectTrigger(0U, EXTI_TRIGGER_RISING);

    /* Unmask EXTI line 0 so it can generate an interrupt. */
    EXTI_EnableInterrupt(0U);

    /* Configure and enable the EXTI0 interrupt in the NVIC. */
    NVIC_SetPriority(EXTI0_IRQn, 0U);
    NVIC_EnableIRQ(EXTI0_IRQn);

    /*
     * The USER button is handled by EXTI0_IRQHandler().
     * No polling is required in the main loop.
     */
    while (1)
    {
    }
}