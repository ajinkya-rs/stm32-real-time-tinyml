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
#include "uart.h"

#define USART1_INPUT_CLOCK_HZ           8000000UL  /* PCLK2 with the current clock setup */
#define USART1_BAUD_RATE                115200UL   /* Must match PuTTY's baud rate */

int main(void)
{
    /* Enable the GPIOA, GPIOE and GPIOC peripheral clocks. */
    RCC_AHB_EnableClock(RCC_AHBENR_GPIOAEN_Msk |
                        RCC_AHBENR_GPIOEEN_Msk |
                        RCC_AHBENR_GPIOCEN_Msk);

    /* Enable the SYSCFG peripheral clock and USART1 peripheral clock */
    RCC_APB2_EnableClock(RCC_APB2ENR_SYSCFGEN_Msk |
                         RCC_APB2ENR_USART1EN_Msk);

    /* Configure GPIOE pin 9 as a push-pull output for the user LED. */
    GPIO_Init(GPIOE,
              GPIO_PIN_9,
              GPIO_MODE_OUTPUT,
              GPIO_OUTPUT_PUSH_PULL,
              GPIO_SPEED_LOW,
              GPIO_PULL_NONE);

    /* Configure PA0 as the USER button input. */
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

    /* Initialize USART1 TX on PC4 */
    USART1_Init(GPIOC, GPIO_PIN_4, GPIO_AF_7, USART1_INPUT_CLOCK_HZ, USART1_BAUD_RATE);

    /* Send 'A' as a polling test */
    USART1_SendByte('A');

    /*
     * The USER button is handled by EXTI0_IRQHandler().
     * No polling is required in the main loop.
     */
    while (1)
    {
    }
}