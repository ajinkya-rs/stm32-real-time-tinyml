/**
 * ****************************************************************************
 * @file    uart.h
 * @brief   USART1 driver interface.
 *
 * @details
 * Provides interrupt-driven USART1 RX and TX using ring buffers.
 * TX queue functions are nonblocking. The RX read function is nonblocking.
 *
 * @author  Ajinkya Shinde
 * ****************************************************************************
 */
#ifndef UART_H_
#define UART_H_

/* ***************************************************************************
 *                              INCLUDES
 * ************************************************************************* */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "gpio.h"

/* ***************************************************************************
 *                        FUNCTION PROTOTYPES
 * ************************************************************************* */

/**
 * @brief Initialize USART1 for asynchronous 8-N-1 communication.
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
void USART1_Init(GPIO_TypeDef *tx_port,
                 GPIO_Pin_t tx_pin,
                 GPIO_AlternateFunction_t tx_af,
                 GPIO_TypeDef *rx_port,
                 GPIO_Pin_t rx_pin,
                 GPIO_AlternateFunction_t rx_af,
                 uint32_t usart_clock_hz,
                 uint32_t baud_rate);

/**
 * @brief Try to queue one byte for USART1 transmission.
 *
 * @param[in] byte Byte to transmit.
 * @return true if queued; false if the TX buffer is full.
 */
bool USART1_TryQueueTxByte(uint8_t byte);

/**
 * @brief Queue as many characters as possible without waiting.
 *
 * @param[in] text Null-terminated string to queue.
 * @return Number of characters queued. A partial string may be queued if the
 *         TX buffer fills. Returns 0 if text is NULL, empty, or no byte fits.
 */
size_t USART1_TryQueueTxString(const char *text);

/**
 * @brief Try to read one byte from the USART1 RX buffer.
 *
 * @param[out] data Pointer where the received byte is stored.
 * @return true if a byte was read; false if data is NULL or the RX buffer
 *         is empty.
 */
bool USART1_ReadByte(uint8_t *data);

#endif /* UART_H_ */