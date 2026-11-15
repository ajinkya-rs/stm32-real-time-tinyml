/**
 ******************************************************************************
 * @file    uart.c
 * @brief USART1 driver implementation for the STM32F303xC.
 *
 * @details
 * Configures USART1 and provides polling transfer functions, interrupt-driven
 * reception through a one-byte mailbox, and queued interrupt-driven
 * transmission through a ring buffer.
 *
 * @author  Ajinkya Shinde
 * @date    2026-11-11
 ******************************************************************************
 */

/* ***************************************************************************
 *                              INCLUDES
 * ************************************************************************* */

#include "uart.h"
#include "gpio.h"
#include "stm32f303xc.h"

/* ***************************************************************************
 *                              DEFINES
 * ************************************************************************* */

#define USART1_TX_BUFFER_SIZE               (64U)

/* ***************************************************************************
 *                             VARIABLES
 * ************************************************************************* */

/* Global variables */
volatile uint8_t received_byte = 0U;
volatile uint8_t received_byte_ready = 0U;

/* Private transmit queue */
static volatile uint8_t tx_buffer[USART1_TX_BUFFER_SIZE];
static volatile uint8_t tx_head = 0U; /* Next slot main code writes. */
static volatile uint8_t tx_tail = 0U; /* Next slot the interrupt handler reads. */

/* ***************************************************************************
 *                        FUNCTION DEFINITIONS
 * ************************************************************************* */

void USART1_Init(GPIO_TypeDef *tx_port, GPIO_Pin_t tx_pin, GPIO_AlternateFunction_t tx_af,
                GPIO_TypeDef *rx_port, GPIO_Pin_t rx_pin, GPIO_AlternateFunction_t rx_af,
                uint32_t usart_clock_hz,
                uint32_t baud_rate)
{
    uint32_t brr_value;
    if ((usart_clock_hz == 0U) || (baud_rate == 0U))
    {
        return;
    }

    /* Disable USART1 before changing its configuration. */
    USART1->USART_CR1 &= ~USART_CR1_UE_Msk;

    /* Configure the selected pin for USART1_TX. */
    GPIO_SetAlternateFunction(tx_port, tx_pin, tx_af);
    GPIO_Init(tx_port,
              tx_pin,
              GPIO_MODE_AF,
              GPIO_OUTPUT_PUSH_PULL,
              GPIO_SPEED_LOW,
              GPIO_PULL_NONE);

    /* Configure the selected pin for USART1_RX. */
    GPIO_SetAlternateFunction(rx_port, rx_pin, rx_af);
    GPIO_Init(rx_port,
              rx_pin,
              GPIO_MODE_AF,
              GPIO_OUTPUT_PUSH_PULL,
              GPIO_SPEED_LOW,
              GPIO_PULL_NONE);

    /*
     * Set asynchronous 8-N-1 framing:
     * CR1 baseline: 8 data bits, no parity, oversampling by 16.
     * CR2 = 0: asynchronous mode and one stop bit.
     * CR3 = 0: no hardware flow control or DMA.
     * RXNEIE is enabled separately for interrupt-driven reception.
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

    /*
    * Enable the receiver, transmitter, and RXNE interrupt source
    * while UE is clear. The next statement enables USART1.
    */
    USART1->USART_CR1 |= (USART_CR1_RE_Msk | USART_CR1_TE_Msk | USART_CR1_RXNEIE_Msk);
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
    while (*tx_text != '\0') 
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

bool USART1_TryQueueByte(uint8_t data)
{
    uint8_t next_head = (tx_head + 1U) % USART1_TX_BUFFER_SIZE;

    if (next_head == tx_tail)
    {
        return false;   /* Queue is full */
    }

    /* Write at the current head. */
    tx_buffer[tx_head] = data;

    /* Advance head to the next slot. */
    tx_head = next_head;

    /* A queued byte is ready; request a TXE interrupt. */
    USART1->USART_CR1 |= USART_CR1_TXEIE_Msk;
    return true;
}

void USART1_QueueString(const char *text)
{
    while (*text != '\0') 
    {
        while (!USART1_TryQueueByte((uint8_t)*text))
        {
            /* Wait for the TX interrupt to make queue space */
        }

        text++;
    }
}

void USART1_IRQHandler(void)
{
    /* Handle RX only when its interrupt is enabled and a byte is ready. */
    if (((USART1->USART_CR1 & USART_CR1_RXNEIE_Msk) != 0U) &&
       ((USART1->USART_ISR & USART_ISR_RXNE_Msk) != 0U))
    {
        received_byte = (uint8_t)USART1->USART_RDR;
        received_byte_ready = 1U;
    }

    /* Handle TX only when its interrupt is enabled and TDR can accept a byte. */
    if (((USART1->USART_CR1 & USART_CR1_TXEIE_Msk) != 0U) &&
       ((USART1->USART_ISR & USART_ISR_TXE_Msk) != 0U))
    {
        if (tx_tail != tx_head)
        {
            USART1->USART_TDR = tx_buffer[tx_tail];
            tx_tail = (uint8_t)((tx_tail + 1U) % USART1_TX_BUFFER_SIZE);
        }
        else
        {
            /* No bytes are queued; disable TXE interrupts until more data is queued. */
            USART1->USART_CR1 &= ~USART_CR1_TXEIE_Msk;
        }
    }
}