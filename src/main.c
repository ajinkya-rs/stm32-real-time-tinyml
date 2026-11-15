/**
 * ****************************************************************************
 * @file    main.c
 * @brief EXTI button/LED and USART1 serial demonstration.
 *
 * @brief EXTI button/LED and USART1 interrupt-driven echo demo.
 *
 * @details
 * Configures the user button on PA0 to trigger EXTI0 and toggle the LED on PE9.
 * USART1 receives bytes through RXNE interrupts and echoes them through TXE
 * interrupts using a transmit ring buffer.
 *
 * @author  Ajinkya Shinde
 * @date    2026-11-14
 * ****************************************************************************
 */

/* ***************************************************************************
 *                              INCLUDES
 * ************************************************************************* */

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

#define USART1_INPUT_CLOCK_HZ           8000000UL  /* PCLK2 with the current clock setup */
#define USART1_BAUD_RATE                115200UL   /* Must match PuTTY's baud rate */

/* ***************************************************************************
 *                        FUNCTION DEFINITIONS
 * ************************************************************************* */

int main(void)
{
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

    /* Configure and enable the USART1 interrupt in the NVIC. */
    NVIC_SetPriority(USART1_IRQn, 0U);
    NVIC_EnableIRQ(USART1_IRQn);

    USART1_QueueString("USART1 RX interrupt ready\r\n");

    /* RX-interrupt, TX-interrupt echo test. */
    while (1)
    {
        if (received_byte_ready)
        {
            uint8_t byte_to_echo = received_byte;
            received_byte_ready = 0U;

            while (!USART1_TryQueueByte(byte_to_echo))
            {
                /* Wait for the TX interrupt to make queue space. */
            }
        }
    }
}