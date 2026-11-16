/**
 * ****************************************************************************
 * @file    uart.c
 * @brief   USART1 driver implementation for the STM32F303xC.
 *
 * @details
 * Configures USART1 and provides interrupt-driven RX and TX using ring buffers.
 * Received bytes are queued by the USART interrupt and read by the main code.
 * Transmit bytes are queued by the main code and sent by the USART interrupt.
 *
 * @author  Ajinkya Shinde
 * ****************************************************************************
 */

/* ***************************************************************************
 *                              INCLUDES
 * ************************************************************************* */

#include <stddef.h>

#include "uart.h"
#include "stm32f303xc.h"

/* ***************************************************************************
 *                              DEFINES
 * ************************************************************************* */

#define USART1_TX_BUFFER_SIZE    (64U)
#define USART1_RX_BUFFER_SIZE    (64U)

/* ***************************************************************************
 *                             VARIABLES
 * ************************************************************************* */

/* TX producer: main code. TX consumer: USART1 interrupt handler. */
static volatile uint8_t tx_buffer[USART1_TX_BUFFER_SIZE];
static volatile uint8_t tx_head;
static volatile uint8_t tx_tail;

/* RX producer: USART1 interrupt handler. RX consumer: main code. */
static volatile uint8_t rx_buffer[USART1_RX_BUFFER_SIZE];
static volatile uint8_t rx_head;
static volatile uint8_t rx_tail;

/* Counts bytes dropped because the software RX buffer was full. */
static volatile uint32_t rx_buffer_overflow;

/* ***************************************************************************
 *                      PRIVATE FUNCTION DECLARATIONS
 * ************************************************************************* */

/**
 * @brief Store a received byte in the RX buffer from the interrupt handler.
 *
 * @param[in] byte Received byte to store.
 *
 * @note If the RX buffer is full, the new byte is discarded and the overflow
 *       counter is incremented.
 */
static void USART1_QueueRxByteFromISR(uint8_t byte);

/* ***************************************************************************
 *                        FUNCTION DEFINITIONS
 * ************************************************************************* */

void USART1_Init(GPIO_TypeDef *tx_port,
                 GPIO_Pin_t tx_pin,
                 GPIO_AlternateFunction_t tx_af,
                 GPIO_TypeDef *rx_port,
                 GPIO_Pin_t rx_pin,
                 GPIO_AlternateFunction_t rx_af,
                 uint32_t usart_clock_hz,
                 uint32_t baud_rate)
{
    uint32_t brr_value;

    if ((tx_port == NULL) ||
        (rx_port == NULL) ||
        (usart_clock_hz == 0U) ||
        (baud_rate == 0U))
    {
        return;
    }

    /* Disable USART1 while configuring it. */
    USART1->USART_CR1 &= ~USART_CR1_UE_Msk;

    /* Configure the USART1_TX pin for alternate-function mode. */
    GPIO_SetAlternateFunction(tx_port, tx_pin, tx_af);
    GPIO_Init(tx_port,
              tx_pin,
              GPIO_MODE_AF,
              GPIO_OUTPUT_PUSH_PULL,
              GPIO_SPEED_LOW,
              GPIO_PULL_NONE);

    /* Configure the USART1_RX pin for alternate-function mode. */
    GPIO_SetAlternateFunction(rx_port, rx_pin, rx_af);
    GPIO_Init(rx_port,
              rx_pin,
              GPIO_MODE_AF,
              GPIO_OUTPUT_PUSH_PULL,
              GPIO_SPEED_LOW,
              GPIO_PULL_NONE);

    /*
     * Configure asynchronous 8-N-1 framing:
     * CR1 = 0: 8 data bits, no parity, oversampling by 16.
     * CR2 = 0: asynchronous mode, one stop bit.
     * CR3 = 0: no hardware flow control or DMA.
     */
    USART1->USART_CR1 = 0U;
    USART1->USART_CR2 = 0U;
    USART1->USART_CR3 = 0U;

    /* Calculate BRR for oversampling by 16, rounding to the nearest integer. */
    brr_value = (usart_clock_hz + (baud_rate / 2U)) / baud_rate;
    USART1->USART_BRR = brr_value;

    /* Reset the TX and RX ring-buffer indexes and the RX overflow counter. */
    tx_head = 0U;
    tx_tail = 0U;
    rx_head = 0U;
    rx_tail = 0U;
    rx_buffer_overflow = 0U;

    /* Clear a previously latched overrun flag before enabling RX interrupts. */
    USART1->USART_ICR = USART_ICR_ORECF_Msk;

    /* Enable the transmitter, receiver, and RXNE interrupt source. */
    USART1->USART_CR1 |= (USART_CR1_TE_Msk |
                          USART_CR1_RE_Msk |
                          USART_CR1_RXNEIE_Msk);

    /* Enable USART1. */
    USART1->USART_CR1 |= USART_CR1_UE_Msk;
}

bool USART1_TryQueueTxByte(uint8_t byte)
{
    uint8_t current_head = tx_head;
    uint8_t next_head =
        (uint8_t)((current_head + 1U) % USART1_TX_BUFFER_SIZE);

    /* One unused slot distinguishes a full buffer from an empty one. */
    if (next_head == tx_tail)
    {
        return false;
    }

    /* Store the byte before publishing the updated head. */
    tx_buffer[current_head] = byte;
    tx_head = next_head;

    /* Request a TX interrupt so the queued byte can be sent. */
    USART1->USART_CR1 |= USART_CR1_TXEIE_Msk;

    return true;
}

size_t USART1_TryQueueTxString(const char *text)
{
    if (text == NULL)
    {
        return 0U;
    }

    size_t queued_count = 0U;

    while (text[queued_count] != '\0')
    {
        if (!USART1_TryQueueTxByte((uint8_t)text[queued_count]))
        {
            /* Stop without waiting; the caller can retry the remaining text. */
            break;
        }

        queued_count++;
    }

    return queued_count;
}

bool USART1_ReadByte(uint8_t *data)
{
    if (data == NULL)
    {
        return false;
    }

    uint8_t current_tail = rx_tail;

    /* Equal indexes mean there are no unread bytes. */
    if (current_tail == rx_head)
    {
        return false;
    }

    /* Copy the next byte to the caller, then advance the read index. */
    *data = rx_buffer[current_tail];
    rx_tail =
        (uint8_t)((current_tail + 1U) % USART1_RX_BUFFER_SIZE);

    return true;
}

void USART1_IRQHandler(void)
{
    uint32_t cr1_status = USART1->USART_CR1;
    uint32_t isr_status = USART1->USART_ISR;

    /* Read RDR when RXNE is set; this clears RXNE. */
    if (((cr1_status & USART_CR1_RXNEIE_Msk) != 0U) &&
        ((isr_status & USART_ISR_RXNE_Msk) != 0U))
    {
        uint8_t received_byte = (uint8_t)USART1->USART_RDR;
        USART1_QueueRxByteFromISR(received_byte);
    }

    /*
     * RXNEIE also interrupts on ORE. Clear ORE through ICR so it does not
     * keep requesting interrupts. Any available RDR byte was handled above.
     */
    if ((isr_status & USART_ISR_ORE_Msk) != 0U)
    {
        USART1->USART_ICR = USART_ICR_ORECF_Msk;
    }

    /* Send the next queued byte when the transmit data register is empty. */
    if (((cr1_status & USART_CR1_TXEIE_Msk) != 0U) &&
        ((isr_status & USART_ISR_TXE_Msk) != 0U))
    {
        if (tx_tail != tx_head)
        {
            USART1->USART_TDR = tx_buffer[tx_tail];
            tx_tail =
                (uint8_t)((tx_tail + 1U) % USART1_TX_BUFFER_SIZE);
        }
        else
        {
            /* No data remains; disable TX interrupts until more is queued. */
            USART1->USART_CR1 &= ~USART_CR1_TXEIE_Msk;
        }
    }
}

static void USART1_QueueRxByteFromISR(uint8_t byte)
{
    uint8_t current_head = rx_head;
    uint8_t next_head =
        (uint8_t)((current_head + 1U) % USART1_RX_BUFFER_SIZE);

    /* If the next write position meets the read position, the buffer is full. */
    if (next_head == rx_tail)
    {
        rx_buffer_overflow++;
        return;
    }

    /* Store the byte before publishing the updated head. */
    rx_buffer[current_head] = byte;
    rx_head = next_head;
}