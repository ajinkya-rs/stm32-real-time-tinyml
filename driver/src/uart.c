/**
 ******************************************************************************
 * @file    uart.c
 * @brief   UART driver implementation for the STM32F303xC.
 *
 * @details
 * This file implements the UART driver APIs used to configure and control
 * individual uart pins.
 *
 * @author  Ajinkya Shinde
 * @date    2026-11-11
 ******************************************************************************
 */

#include "uart.h"

#include "gpio.h"
#include "stm32f303xc.h"

void USART1_Init(GPIO_TypeDef *tx_port, GPIO_Pin_t tx_pin, GPIO_AlternateFunction_t tx_af,
                GPIO_TypeDef *rx_port, GPIO_Pin_t rx_pin, GPIO_AlternateFunction_t rx_af,
                uint32_t usart_clock_hz,
                uint32_t baud_rate)
{
    uint32_t brr_value;
    if((usart_clock_hz == 0U) || (baud_rate == 0U))
    {
        return;
    }

    /* Disable USART1 before changing its configuration */
    USART1->USART_CR1 &= ~USART_CR1_UE_Msk;

    /* Configure the selected pin for USART1_TX */
    GPIO_SetAlternateFunction(tx_port, tx_pin, tx_af);
    GPIO_Init(tx_port,
              tx_pin,
              GPIO_MODE_AF,
              GPIO_OUTPUT_PUSH_PULL,
              GPIO_SPEED_LOW,
              GPIO_PULL_NONE);

    /* Configure the selected pin for USART1_RX */
    GPIO_SetAlternateFunction(rx_port, rx_pin, rx_af);
    GPIO_Init(rx_port,
              rx_pin,
              GPIO_MODE_AF,
              GPIO_OUTPUT_PUSH_PULL,
              GPIO_SPEED_LOW,
              GPIO_PULL_NONE);

    /*
     * Establish a simple asynchronous, 8-N-1, polling-TX configuration:
     * CR1 = 0: 8 data bits, no parity, OVER8 = 0; UE, TE and RE off.
     * CR2 = 0: asynchronous mode, one stop bit.
     * CR3 = 0: no flow control, DMA, or USART interrupt sources enabled.
     */
    USART1->USART_CR1 = 0U;
    USART1->USART_CR2 = 0U;
    USART1->USART_CR3 = 0U;

    /*
     * With OVER8 = 0, calculate the rounded BRR value from the 
     * USART1 input clock and baud rate requested by the other end.
     */
    brr_value = (usart_clock_hz + (baud_rate / 2U)) / baud_rate;
    USART1->USART_BRR = brr_value;

    /* Enable the transmitter and receiver while UE is still clear, then enable USART1. */
    USART1->USART_CR1 |= (USART_CR1_RE_Msk | USART_CR1_TE_Msk);
    USART1->USART_CR1 |= USART_CR1_UE_Msk;
}

void USART1_SendByte(uint8_t tx_byte) 
{
    while ((USART1->USART_ISR & USART_ISR_TXE_Msk) == 0U)
    {
        /* Wait until the transmit data register can accept a byte. */
    }

    USART1->USART_TDR = tx_byte;
}

void USART1_SendString(const char *tx_text)
{
    while(*tx_text != '\0') 
    {
        USART1_SendByte((uint8_t)*tx_text);
        tx_text++;
    }
}

uint8_t USART1_ReceiveByte(void)
{
    while ((USART1->USART_ISR & USART_ISR_RXNE_Msk) == 0U)
    {
        /* Wait until the received data is ready to be read */
    }

    return (uint8_t)USART1->USART_RDR;
}