/**
 * ****************************************************************************
 * @file    uart.h
 * @brief   USART1 driver interface.
 *
 * @details
 * Provides polling transfer functions, RXNE-interrupt reception through a
 * one-byte mailbox, and TXE-interrupt transmission through a ring buffer.
 * 
 * @author  Ajinkya Shinde
 * @date    2026-11-14
 * ****************************************************************************
 */
#ifndef UART_H_
#define UART_H_

/* ***************************************************************************
 *                              INCLUDES
 * ************************************************************************* */

#include <stdbool.h>

#include "gpio.h"

/* ***************************************************************************
 *                            VARIABLES
 * ************************************************************************* */

/* One-byte mailbox: written by USART1_IRQHandler, consumed by main(). */
extern volatile uint8_t received_byte;
extern volatile uint8_t received_byte_ready;

/* ***************************************************************************
 *                        FUNCTION PROTOTYPES
 * ************************************************************************* */

/**
 * @brief Initialize USART1 for 8-N-1 and interrupt-driven RX/TX operation.
 *
 * @param[in] tx_port        GPIO port used for USART1_TX.
 * @param[in] tx_pin         GPIO pin used for USART1_TX.
 * @param[in] tx_af          Alternate function for the TX pin.
 * @param[in] rx_port        GPIO port used for USART1_RX.
 * @param[in] rx_pin         GPIO pin used for USART1_RX.
 * @param[in] rx_af          Alternate function for the RX pin.
 * @param[in] usart_clock_hz USART1 input clock frequency in hertz.
 * @param[in] baud_rate      Requested baud rate.
 *
 * @note Enable the GPIO port clocks and USART1 peripheral clock before calling.
 * @note This function enables RXNEIE. The caller must enable USART1_IRQn in
 *       the NVIC separately.
 */
void USART1_Init(GPIO_TypeDef *tx_port, GPIO_Pin_t tx_pin, GPIO_AlternateFunction_t tx_af,
                GPIO_TypeDef *rx_port, GPIO_Pin_t rx_pin, GPIO_AlternateFunction_t rx_af,
                uint32_t usart_clock_hz,
                uint32_t baud_rate);

/**
 * @brief Send one byte through USART1 using polling
 * 
 * @param[in] data Byte to send
 * 
 * @note Blocks until the transmit data register is ready. 
 * Do not use while queued interrupt-driven transmission is active.
 */
void USART1_SendByte(uint8_t data);

/**
 * @brief Send a null-terminated string through USART1 using polling.
 *
 * @param[in] text Pointer to the string to send.
 *
 * @note Blocks until the transmit data register is ready for each byte.
 * Do not use while queued interrupt-driven transmission is active.
 */
void USART1_SendString(const char *text);

/**
 * @brief Receive one byte from USART1 using polling.
 *
 * @return The received byte.
 *
 * @note Blocks until a byte is received.
 * Do not call while RXNE interrupts are enabled.
 */
uint8_t USART1_ReceiveByte(void);

/**
 * @brief Queue one byte for interrupt-driven transmission.
 * 
 * @param[in] data Byte to queue.
 * 
 * @return true if queued; false if the transmit queue is full.
 */
bool USART1_TryQueueByte(uint8_t data);

/**
 * @brief Queue a null-terminated string for interrupt-driven transmission.
 *
 * @param[in] text String to queue.
 *
 * @note Waits for queue space when the TX queue is full. Call from main code,
 *       not from an interrupt handler. Returns when all characters are queued,
 *       not necessarily when transmission is complete.
 */
void USART1_QueueString(const char *text);

#endif /* UART_H_ */