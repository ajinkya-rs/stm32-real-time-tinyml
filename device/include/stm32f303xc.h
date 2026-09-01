/**
 ******************************************************************************
 * @file    stm32f303xc.h
 * @brief   Minimal device header for the STM32F303xC microcontroller.
 *
 * @details
 * This file provides the register definitions, peripheral base addresses,
 * peripheral instances, and hardware bit definitions required to access
 * the STM32F303xC peripherals directly.
 *
 * The header is implemented without CMSIS and is intended for bare-metal
 * peripheral programming.
 *
 * @note
 * This file is specific to the STM32F303xC device family.
 *
 * @author  Ajinkya Shinde
 * @date    2026-08-31
 ******************************************************************************
 */

#ifndef STM32F303XC_H_
#define STM32F303XC_H_


/* ***************************************************************************
 *                              INCLUDES
 * ************************************************************************* */

#include <stdint.h>


/* ***************************************************************************
 *                    STRUCTURES, TYPEDEFS AND ENUMS
 * ************************************************************************* */

/** @addtogroup Peripheral_registers
 * @{
 */

/**
 * @brief RCC peripheral register map.
 */
typedef struct
{
    volatile uint32_t CR;         /*!< 0x00: RCC clock control register. */
    volatile uint32_t CFGR;       /*!< 0x04: RCC clock configuration register. */
    volatile uint32_t CIR;        /*!< 0x08: RCC clock interrupt register. */
    volatile uint32_t APB2RSTR;   /*!< 0x0C: RCC APB2 peripheral reset register. */
    volatile uint32_t APB1RSTR;   /*!< 0x10: RCC APB1 peripheral reset register. */
    volatile uint32_t AHBENR;     /*!< 0x14: RCC AHB peripheral clock enable register. */
    volatile uint32_t APB2ENR;    /*!< 0x18: RCC APB2 peripheral clock enable register. */
    volatile uint32_t APB1ENR;    /*!< 0x1C: RCC APB1 peripheral clock enable register. */
    volatile uint32_t BDCR;       /*!< 0x20: RCC backup domain control register. */
    volatile uint32_t CSR;        /*!< 0x24: RCC clock control and status register. */
    volatile uint32_t AHBRSTR;    /*!< 0x28: RCC AHB peripheral reset register. */
    volatile uint32_t CFGR2;      /*!< 0x2C: RCC clock configuration register 2. */
    volatile uint32_t CFGR3;      /*!< 0x30: RCC clock configuration register 3. */

} RCC_TypeDef;


/**
 * @brief GPIO peripheral register map.
 */
typedef struct
{
    volatile uint32_t MODER;     /*!< 0x00: GPIO port mode register. */
    volatile uint32_t OTYPER;    /*!< 0x04: GPIO port output type register. */
    volatile uint32_t OSPEEDR;   /*!< 0x08: GPIO port output speed register. */
    volatile uint32_t PUPDR;     /*!< 0x0C: GPIO port pull-up/pull-down register. */
    volatile uint32_t IDR;       /*!< 0x10: GPIO port input data register. */
    volatile uint32_t ODR;       /*!< 0x14: GPIO port output data register. */
    volatile uint32_t BSRR;      /*!< 0x18: GPIO port bit set/reset register. */
    volatile uint32_t LCKR;      /*!< 0x1C: GPIO port configuration lock register. */
    volatile uint32_t AFR[2];    /*!< 0x20-0x24: GPIO alternate function registers. */
    volatile uint32_t BRR;       /*!< 0x28: GPIO port bit reset register. */

} GPIO_TypeDef;

/** @} */

/* ***************************************************************************
 *                                DEFINES
 * ************************************************************************* */

/** @addtogroup Peripheral_memory_map
 * @{
 */
/* ---------------------------------------------------------------------------
 *                          PERIPHERAL MEMORY MAP
 * ------------------------------------------------------------------------- */

/* Memory base addresses */

#define FLASH_BASE              0x08000000UL                  /* Flash memory base address */
#define SRAM_BASE               0x20000000UL                  /* SRAM memory base address */


/* Peripheral memory base address */

#define PERIPH_BASE             0x40000000UL                  /* Peripheral base address */

#define APB1PERIPH_BASE        (PERIPH_BASE + 0x00000000UL)
#define APB2PERIPH_BASE        (PERIPH_BASE + 0x00010000UL)
#define AHB1PERIPH_BASE        (PERIPH_BASE + 0x00020000UL)
#define AHB2PERIPH_BASE        (PERIPH_BASE + 0x08000000UL)
#define AHB3PERIPH_BASE        (PERIPH_BASE + 0x10000000UL)


/* AHB1 peripherals */

#define RCC_BASE               (AHB1PERIPH_BASE + 0x00001000UL)


/* AHB2 peripherals */

#define GPIOA_BASE             (AHB2PERIPH_BASE + 0x00000000UL)
#define GPIOB_BASE             (AHB2PERIPH_BASE + 0x00000400UL)
#define GPIOC_BASE             (AHB2PERIPH_BASE + 0x00000800UL)
#define GPIOD_BASE             (AHB2PERIPH_BASE + 0x00000C00UL)
#define GPIOE_BASE             (AHB2PERIPH_BASE + 0x00001000UL)
#define GPIOF_BASE             (AHB2PERIPH_BASE + 0x00001400UL)

/** @} */

/** @addtogroup Peripheral_instances
 * @{
 */
/* ---------------------------------------------------------------------------
 *                          PERIPHERAL INSTANCES
 * ------------------------------------------------------------------------- */

#define RCC                     ((RCC_TypeDef *)RCC_BASE)

#define GPIOA                   ((GPIO_TypeDef *)GPIOA_BASE)
#define GPIOB                   ((GPIO_TypeDef *)GPIOB_BASE)
#define GPIOC                   ((GPIO_TypeDef *)GPIOC_BASE)
#define GPIOD                   ((GPIO_TypeDef *)GPIOD_BASE)
#define GPIOE                   ((GPIO_TypeDef *)GPIOE_BASE)
#define GPIOF                   ((GPIO_TypeDef *)GPIOF_BASE)

/** @} */


/** @addtogroup Peripheral_bit_definitions
 * @{
 */

/* ---------------------------------------------------------------------------
 *                          PERIPHERAL BIT DEFINITIONS
 * ------------------------------------------------------------------------- */

/* ...........................................................................
 *                         RESET AND CLOCK CONTROL (RCC)
 * ......................................................................... */

/* .................................................................. AHBENR */

#define RCC_AHBENR_GPIOAEN_Pos  (17U)
#define RCC_AHBENR_GPIOAEN_Msk  (1UL << RCC_AHBENR_GPIOAEN_Pos)

#define RCC_AHBENR_GPIOBEN_Pos  (18U)
#define RCC_AHBENR_GPIOBEN_Msk  (1UL << RCC_AHBENR_GPIOBEN_Pos)

#define RCC_AHBENR_GPIOCEN_Pos  (19U)
#define RCC_AHBENR_GPIOCEN_Msk  (1UL << RCC_AHBENR_GPIOCEN_Pos)

#define RCC_AHBENR_GPIODEN_Pos  (20U)
#define RCC_AHBENR_GPIODEN_Msk  (1UL << RCC_AHBENR_GPIODEN_Pos)

#define RCC_AHBENR_GPIOEEN_Pos  (21U)
#define RCC_AHBENR_GPIOEEN_Msk  (1UL << RCC_AHBENR_GPIOEEN_Pos)

#define RCC_AHBENR_GPIOFEN_Pos  (22U)
#define RCC_AHBENR_GPIOFEN_Msk  (1UL << RCC_AHBENR_GPIOFEN_Pos)

/** @} */


#endif /* STM32F303XC_H_ */