/**
 * ****************************************************************************
 * @file    main.c
 * @brief   Demonstrate EXTI button/LED control and USART1 interrupt-driven echo.
 *
 * @details
 * Configures PA0 to trigger EXTI0 and PE9 as the user LED output.
 * USART1 receives bytes through an RX interrupt into the RX ring buffer.
 * The main loop reads received bytes and queues them for TX-interrupt
 * transmission using the TX ring buffer.
 *
 * @author  Ajinkya Shinde
 * @date    2026-11-16
 * ****************************************************************************
 */

/* ***************************************************************************
 *                              INCLUDES
 * ************************************************************************* */

#include <stdbool.h>
#include <stdint.h>

#include "stm32f303xc.h"
#include "rcc.h"
#include "gpio.h"
#include "syscfg.h"
#include "exti.h"
#include "nvic.h"
#include "uart.h"

/* ***************************************************************************
 *                              DEFINES
 * ************************************************************************* */

#define USART1_INPUT_CLOCK_HZ  8000000UL
#define USART1_BAUD_RATE       115200UL

/* ***************************************************************************
 *                        FUNCTION DEFINITIONS
 * ************************************************************************* */

int main(void)
{
    bool echo_pending = false;
    uint8_t pending_byte = 0U;

    /* Enable the GPIOA, GPIOE and GPIOC peripheral clocks. */
    RCC_AHB_EnableClock(RCC_AHBENR_GPIOAEN_Msk |
                        RCC_AHBENR_GPIOEEN_Msk |
                        RCC_AHBENR_GPIOCEN_Msk);

    /* Enable the SYSCFG and USART1 peripheral clocks. */
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

    /* Initialize USART1 TX on PC4 and RX on PC5. */
    USART1_Init(GPIOC, GPIO_PIN_4, GPIO_AF_7,
                GPIOC, GPIO_PIN_5, GPIO_AF_7,
                USART1_INPUT_CLOCK_HZ,
                USART1_BAUD_RATE);

    /* Enable USART1 interrupts in the NVIC. */
    NVIC_SetPriority(USART1_IRQn, 0U);
    NVIC_EnableIRQ(USART1_IRQn);

    while (1)
    {
        /*
         * Read another RX byte only after the previous byte has been accepted
         * by the TX queue.
         */
        if (!echo_pending)
        {
            echo_pending = USART1_ReadByte(&pending_byte);
        }

        /* Keep the byte pending if the TX queue is temporarily full. */
        if (echo_pending && USART1_TryQueueTxByte(pending_byte))
        {
            echo_pending = false;
        }
    }
}