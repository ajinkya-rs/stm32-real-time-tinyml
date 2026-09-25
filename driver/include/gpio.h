/**
 ******************************************************************************
 * @file    gpio.h
 * @brief   GPIO driver interface for the STM32F303xC.
 *
 * @details
 * This file provides the public API for configuring and controlling
 * individual GPIO pins.
 *
 * The driver provides APIs for:
 * - GPIO pin initialization
 * - GPIO pin input reading
 * - GPIO pin output writing
 * - GPIO pin output toggling
 *
 * @note
 * The corresponding GPIO peripheral clock must be enabled before
 * configuring or accessing a GPIO peripheral.
 *
 * @author  Ajinkya Shinde
 * @date    2026-08-31
 ******************************************************************************
 */

#ifndef GPIO_H_
#define GPIO_H_


/* ***************************************************************************
 *                              INCLUDES
 * ************************************************************************* */

#include <stdint.h>
#include "stm32f303xc.h"


/* ***************************************************************************
 *                    TYPES, ENUMS AND DEFINITIONS
 * ************************************************************************* */

/**
 * @brief GPIO pin number.
 *
 * @details
 * The enumeration value corresponds to the GPIO register bit position
 * of the selected pin.
 *
 * For example:
 * GPIO_PIN_0  = bit position 0
 * GPIO_PIN_1  = bit position 1
 * ...
 * GPIO_PIN_15 = bit position 15
 *
 * @note
 * The enumeration uses GPIO_PIN_0 through GPIO_PIN_15 as the public
 * API names, while the underlying hardware GPIO bit positions are 0
 * through 15.
 */
typedef enum
{
    GPIO_PIN_0  = 0U,
    GPIO_PIN_1  = 1U,
    GPIO_PIN_2  = 2U,
    GPIO_PIN_3  = 3U,
    GPIO_PIN_4  = 4U,
    GPIO_PIN_5  = 5U,
    GPIO_PIN_6  = 6U,
    GPIO_PIN_7  = 7U,
    GPIO_PIN_8  = 8U,
    GPIO_PIN_9  = 9U,
    GPIO_PIN_10 = 10U,
    GPIO_PIN_11 = 11U,
    GPIO_PIN_12 = 12U,
    GPIO_PIN_13 = 13U,
    GPIO_PIN_14 = 14U,
    GPIO_PIN_15 = 15U

} GPIO_Pin_t;


/**
 * @brief GPIO pin operating mode.
 *
 * @details
 * Selects how the GPIO pin is connected to and controlled by the
 * GPIO peripheral.
 */
typedef enum
{
    GPIO_MODE_INPUT  = 0U,
    GPIO_MODE_OUTPUT = 1U,
    GPIO_MODE_AF     = 2U,
    GPIO_MODE_ANALOG = 3U

} GPIO_Mode_t;


/**
 * @brief GPIO output driver type.
 *
 * @details
 * Defines the electrical behavior of the GPIO output driver.
 *
 * - GPIO_OUTPUT_PUSH_PULL  - GPIO actively drives both HIGH and LOW.
 * - GPIO_OUTPUT_OPEN_DRAIN - GPIO actively drives LOW and releases the
 *                       output for HIGH.
 *
 * @note
 * This configuration is relevant when the pin is used as an output
 * or as an alternate-function output.
 */
typedef enum
{
    GPIO_OUTPUT_PUSH_PULL  = 0U,
    GPIO_OUTPUT_OPEN_DRAIN = 1U

} GPIO_OutputType_t;

/**
 * @brief GPIO output-speed configuration.
 *
 * @details
 * Selects the output speed for a GPIO output or alternate-function output.
 * 
 * - GPIO_SPEED_LOW             - Low speed
 * - GPIO_SPEED_MEDIUM          - Medium speed
 * - GPIO_SPEED_HIGH            - High speed
 *
 * @note
 * This setting is relevant when the pin is used as an output or
 * alternate-function output.
 */
typedef enum
{
    GPIO_SPEED_LOW              = 0U,
    GPIO_SPEED_MEDIUM           = 1U,
    GPIO_SPEED_HIGH             = 2U

} GPIO_Speed_t;


/**
 * @brief GPIO internal pull-up/pull-down configuration.
 *
 * @details
 * Configures the internal weak pull resistor associated with the
 * GPIO input/output structure.
 *
 * - GPIO_PULL_NONE            - No internal pull resistor.
 * - GPIO_PULL_UP              - Internal pull-up enabled.
 * - GPIO_PULL_DOWN            - Internal pull-down enabled.
 */
typedef enum
{
    GPIO_PULL_NONE              = 0U,
    GPIO_PULL_UP                = 1U,
    GPIO_PULL_DOWN              = 2U

} GPIO_Pull_t;


/* ***************************************************************************
 *                              GPIO API
 * ************************************************************************* */

/**
 * @brief  Initialize a GPIO pin.
 *
 * @param[in] GPIOx       GPIO peripheral instance.
 * @param[in] gpio_pin    GPIO pin to configure.
 * @param[in] mode        GPIO operating mode.
 * @param[in] output_type GPIO output driver type.
 * @param[in] speed       GPIO output speed configuration.
 * @param[in] pull        GPIO internal pull-up/pull-down configuration.
 *
 * @details
 * Configures the selected GPIO pin's:
 * - Operating mode through MODER.
 * - Output driver type through OTYPER.
 * - Output speed through OSPEEDR.
 * - Internal pull-up/pull-down configuration through PUPDR.
 *
 * The existing register configuration for the selected pin is cleared
 * before the new configuration is written.
 *
 * @note
 * The GPIO peripheral clock must be enabled before calling this function.
 */
void GPIO_Init(GPIO_TypeDef *GPIOx,
               GPIO_Pin_t gpio_pin,
               GPIO_Mode_t mode,
               GPIO_OutputType_t output_type,
               GPIO_Speed_t speed,
               GPIO_Pull_t pull);


/**
 * @brief  Read the current logic level of a GPIO pin.
 *
 * @param[in] GPIOx      GPIO peripheral instance.
 * @param[in] gpio_pin   GPIO pin to read.
 *
 * @return GPIO pin logic level.
 *         0U = LOW.
 *         1U = HIGH.
 *
 * @details
 * Reads the selected bit from the GPIO Input Data Register (IDR).
 *
 * @note
 * The returned value represents the logic level currently observed
 * at the GPIO pin.
 */
uint8_t GPIO_ReadPin(GPIO_TypeDef *GPIOx,
                     GPIO_Pin_t gpio_pin);


/**
 * @brief  Write a logic level to a GPIO output pin.
 *
 * @param[in] GPIOx      GPIO peripheral instance.
 * @param[in] gpio_pin   GPIO pin to write.
 * @param[in] value      Output value.
 *                    0U = LOW.
 *                    Non-zero = HIGH.
 *
 * @details
 * The GPIO Bit Set/Reset Register (BSRR) is used to change the
 * output state of the selected GPIO pin.
 *
 * Writing to BSRR allows the pin to be set or reset without directly
 * performing a read-modify-write operation on the Output Data
 * Register (ODR).
 *
 * @note
 * The GPIO pin should be configured as an output or an appropriate
 * alternate-function output before using this function.
 */
void GPIO_WritePin(GPIO_TypeDef *GPIOx,
                   GPIO_Pin_t gpio_pin,
                   uint8_t value);


/**
 * @brief  Toggle the current output state of a GPIO pin.
 *
 * @param[in] GPIOx GPIO peripheral instance.
 * @param[in] gpio_pin GPIO pin to toggle.
 *
 * @details
 * Reads the current output data state from ODR and then uses BSRR
 * to change the pin to the opposite state.
 *
 * If the current output state is HIGH, the pin is reset to LOW.
 * If the current output state is LOW, the pin is set to HIGH.
 *
 * @note
 * This function is intended for GPIO pins configured as outputs.
 */
void GPIO_TogglePin(GPIO_TypeDef *GPIOx,
                    GPIO_Pin_t gpio_pin);


#endif /* GPIO_H_ */