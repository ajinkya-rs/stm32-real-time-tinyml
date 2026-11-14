/**
 * ****************************************************************************
 * @file    uart.h
 * @brief   UART driver interface.
 *
 * @details
 * Provides APIs to enable and configure UART.
 *
 * @author  Ajinkya Shinde
 * @date    2026-11-11
 * ****************************************************************************
 */
#ifndef UART_H_
#define UART_H_

#include <stdint.h>

#include "gpio.h"

/**
 * @brief Initialize USART1 for polling-based transmission and reception.
 * 
 * @param[in] tx_port           GPIO port used for USART1_TX.
 * @param[in] tx_pin            GPIO pin used for USART1_TX.
 * @param[in] tx_af             Alternate function selction for the TX pin.
 * @param[in] rx_port           GPIO port used for USART1_RX.
 * @param[in] rx_pin            GPIO pin used for USART1_RX.
 * @param[in] rx_af             Alternate function selction for the RX pin.
 * @param[in] usart_clock_hz    USART1 input clock frequency in hertz.
 * @param[in] baud_rate         Requested baud rate.
 */
void USART1_Init(GPIO_TypeDef *tx_port, GPIO_Pin_t tx_pin, GPIO_AlternateFunction_t tx_af,
                GPIO_TypeDef *rx_port, GPIO_Pin_t rx_pin, GPIO_AlternateFunction_t rx_af,
                uint32_t usart_clock_hz,
                uint32_t baud_rate);

/**
 * @brief Send one byte through USART1 using polling
 * 
 * @param[in] data              Byte to send
 * 
 * @note Blocks until the transmit data register is ready.
 */
void USART1_SendByte(uint8_t data);

/**
 * @brief Send a null-terminated string through USART1 using polling.
 *
 * @param[in] text              Pointer to the string to send.
 *
 * @note Blocks while waiting for each byte to be transmitted.
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

#endif /* UART_H_ */