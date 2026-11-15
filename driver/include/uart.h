/**
 * ****************************************************************************
 * @file    uart.h
 * @brief   USART1 driver interface.
 *
 * @details
 * Provides polling-based transmission and RXNE-interrupt-based reception.
 *
 * @author  Ajinkya Shinde
 * @date    2026-11-14
 * ****************************************************************************
 */
#ifndef UART_H_
#define UART_H_

#include <stdint.h>
#include <stdbool.h>

#include "gpio.h"

/* One-byte mailbox: written by USART1_IRQHandler, consumed by main(). */
extern volatile uint8_t received_byte;
extern volatile uint8_t received_byte_ready;


/**
 * @brief Configure USART1 for polling TX and interrupt-driven RX.
 * 
 * @param[in] tx_port           GPIO port used for USART1_TX.
 * @param[in] tx_pin            GPIO pin used for USART1_TX.
 * @param[in] tx_af             Alternate function selection for the TX pin.
 * @param[in] rx_port           GPIO port used for USART1_RX.
 * @param[in] rx_pin            GPIO pin used for USART1_RX.
 * @param[in] rx_af             Alternate function selection for the RX pin.
 * @param[in] usart_clock_hz    USART1 input clock frequency in hertz.
 * @param[in] baud_rate         Requested baud rate.
 * 
 * @note GPIO and USART1 peripheral clocks must be enabled before calling.
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
 */
void USART1_SendByte(uint8_t data);

/**
 * @brief Send a null-terminated string through USART1 using polling.
 *
 * @param[in] text Pointer to the string to send.
 *
 * @note Blocks until the transmit data register is ready for each byte.
 */
void USART1_SendString(const char *text);

/**
 * @brief Receive one byte from USART1 using polling.
 *
 * @return The received byte.
 *
 * @note Blocks until a byte is received.
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

#endif /* UART_H_ */