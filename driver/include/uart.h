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
 * @brief Initialize USART1 for polling-based transmission.
 * 
 * @param[in] tx_port           GPIO port used for USART1_TX.
 * @param[in] tx_pin            GPIO pin used for USART1_TX.
 * @param[in] tx_af             Alternate function selction for the TX pin.
 * @param[in] usart_clock_hz    USART1 input clock frequency in hertz.
 * @param[in] baud_rate         Requested baud rate.
 */
void USART1_Init (GPIO_TypeDef *tx_port,
                  GPIO_Pin_t tx_pin,
                  GPIO_AlternateFunction_t tx_af,
                  uint32_t usart_clock_hz,
                  uint32_t baud_rate);

/**
 * @brief Send one byte through USART1 using polling
 * 
 * @param[in] data              Byte to sent
 * 
 * @note Blocks until the transmit data register is ready.
 */
void USART1_SendByte (uint8_t data);

#endif /* UART_H_ */