/**
 * ****************************************************************************
 * @file    uart.c
 * @brief   USART1 driver implementation for the STM32F303xC.
 *
 * @details
 * Provides interrupt-driven RX and TX using ring buffers. The USART1
 * interrupt handler stores received bytes and sends queued transmit bytes.
 *
 * @author  Ajinkya Shinde
 * @date    2026-11-17
 * ****************************************************************************
 */

/* ***************************************************************************
 *                              INCLUDES
 * ************************************************************************* */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "gpio.h"
#include "stm32f303xc.h"
#include "uart.h"

/* ***************************************************************************
 *                              DEFINES
 * ************************************************************************* */

#define USART1_TX_BUFFER_SIZE    64U
#define USART1_RX_BUFFER_SIZE    64U

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

/* Software RX-buffer drops and hardware USART receive errors. */
static volatile uint32_t rx_buffer_overflow;
static volatile uint32_t rx_overrun_count;
static volatile uint32_t rx_framing_error_count;
static volatile uint32_t rx_noise_error_count;

/* ***************************************************************************
 *                      PRIVATE FUNCTION DECLARATIONS
 * ************************************************************************* */

/**
 * @brief Handle a USART1 receive event.
 *
 * Reads RDR when RXNE and RXNEIE are set, then queues the byte unless it
 * has a framing or noise error.
 *
 * @param[in] cr1_status Snapshot of the USART1_CR1 register.
 * @param[in] isr_status Snapshot of the USART1_USART_ISR register.
 */
static void USART1_HandleRxInterrupt(uint32_t cr1_status,
                                     uint32_t isr_status);

/**
 * @brief Count and clear USART1 receive error flags.
 *
 * @param[in] isr_status Snapshot of the USART1_USART_ISR register.
 */
static void USART1_HandleErrorInterrupt(uint32_t isr_status);

/**
 * @brief Send one queued byte when the TX data register is empty.
 *
 * Disables the TXE interrupt source when the TX queue is empty.
 *
 * @param[in] cr1_status Snapshot of the USART1_CR1 register.
 * @param[in] isr_status Snapshot of the USART1_USART_ISR register.
 */
static void USART1_HandleTxInterrupt(uint32_t cr1_status,
                                     uint32_t isr_status);
                            
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
    if ((tx_port == NULL) ||
        (rx_port == NULL) ||
        (usart_clock_hz == 0U) ||
        (baud_rate == 0U))
    {
        return;
    }

    /* Disable USART1 and its interrupt sources during configuration. */
    USART1->USART_CR1 = 0U;
    USART1->USART_CR2 = 0U;
    USART1->USART_CR3 = 0U;

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
     * CR3: enable error interrupts; flow control and DMA remain disabled.
     */
    USART1->USART_BRR =
        (usart_clock_hz + (baud_rate / 2U)) / baud_rate;
    USART1->USART_CR3 = USART_CR3_EIE_Msk;

    /* Reset ring-buffer indexes and diagnostic counters. */
    tx_head = 0U;
    tx_tail = 0U;
    rx_head = 0U;
    rx_tail = 0U;

    rx_buffer_overflow = 0U;
    rx_overrun_count = 0U;
    rx_framing_error_count = 0U;
    rx_noise_error_count = 0U;

    /* Clear any error flags left from an earlier initialization. */
    USART1->USART_ICR = USART_ICR_ORECF_Msk |
                        USART_ICR_FECF_Msk |
                        USART_ICR_NCF_Msk;

    /* Enable the transmitter, receiver, and RXNE interrupt source. */
    USART1->USART_CR1 = USART_CR1_TE_Msk |
                        USART_CR1_RE_Msk |
                        USART_CR1_RXNEIE_Msk;

    /* Enable USART1. */
    USART1->USART_CR1 |= USART_CR1_UE_Msk;
}

bool USART1_TryQueueTxByte(uint8_t byte)
{
    uint8_t current_head = tx_head;
    uint8_t next_head =
        (uint8_t)((current_head + 1U) % USART1_TX_BUFFER_SIZE);

    if (next_head == tx_tail)
    {
        return false;
    }

    tx_buffer[current_head] = byte;
    tx_head = next_head;

    /* Let the TX interrupt send the queued byte. */
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
            /* Stop without waiting; the caller may retry the remainder. */
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

    *data = rx_buffer[current_tail];
    rx_tail =
        (uint8_t)((current_tail + 1U) % USART1_RX_BUFFER_SIZE);

    return true;
}

void USART1_IRQHandler(void)
{
    uint32_t cr1_status = USART1->USART_CR1;
    uint32_t isr_status = USART1->USART_ISR;

    USART1_HandleRxInterrupt(cr1_status, isr_status);
    USART1_HandleErrorInterrupt(isr_status);
    USART1_HandleTxInterrupt(cr1_status, isr_status);
}

static void USART1_HandleRxInterrupt(uint32_t cr1_status,
                                     uint32_t isr_status)
{
    if (((cr1_status & USART_CR1_RXNEIE_Msk) == 0U) ||
        ((isr_status & USART_ISR_RXNE_Msk) == 0U))
    {
        return;
    }

    uint8_t received_byte = (uint8_t)USART1->USART_RDR;
    uint32_t invalid_data_flags = USART_ISR_FE_Msk | USART_ISR_NE_Msk;

    /* Do not queue data marked with a framing or noise error. */
    if ((isr_status & invalid_data_flags) == 0U)
    {
        USART1_QueueRxByteFromISR(received_byte);
    }
}

static void USART1_HandleErrorInterrupt(uint32_t isr_status)
{
    uint32_t clear_flags = 0U;

    if ((isr_status & USART_ISR_ORE_Msk) != 0U)
    {
        rx_overrun_count++;
        clear_flags |= USART_ICR_ORECF_Msk;
    }

    if ((isr_status & USART_ISR_FE_Msk) != 0U)
    {
        rx_framing_error_count++;
        clear_flags |= USART_ICR_FECF_Msk;
    }

    if ((isr_status & USART_ISR_NE_Msk) != 0U)
    {
        rx_noise_error_count++;
        clear_flags |= USART_ICR_NCF_Msk;
    }

    /* ICR is write-one-to-clear; write the selected flags directly. */
    if (clear_flags != 0U)
    {
        USART1->USART_ICR = clear_flags;
    }
}

static void USART1_HandleTxInterrupt(uint32_t cr1_status,
                                     uint32_t isr_status)
{
    if (((cr1_status & USART_CR1_TXEIE_Msk) == 0U) ||
        ((isr_status & USART_ISR_TXE_Msk) == 0U))
    {
        return;
    }

    if (tx_tail != tx_head)
    {
        USART1->USART_TDR = tx_buffer[tx_tail];
        tx_tail =
            (uint8_t)((tx_tail + 1U) % USART1_TX_BUFFER_SIZE);
    }
    else
    {
        /* No data remains; wait for the next queued byte before interrupting. */
        USART1->USART_CR1 &= ~USART_CR1_TXEIE_Msk;
    }
}

static void USART1_QueueRxByteFromISR(uint8_t byte)
{
    uint8_t current_head = rx_head;
    uint8_t next_head =
        (uint8_t)((current_head + 1U) % USART1_RX_BUFFER_SIZE);

    /* Keep one slot empty to distinguish a full buffer from an empty one. */
    if (next_head == rx_tail)
    {
        rx_buffer_overflow++;
        return;
    }

    /* Store the byte before publishing the new head position. */
    rx_buffer[current_head] = byte;
    rx_head = next_head;
}