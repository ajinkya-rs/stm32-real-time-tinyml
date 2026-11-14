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
 * @brief SYSCFG peripheral register map. 
 */
typedef struct 
{
    volatile uint32_t CFGR1;        /*!< 0x00:  SYSCFG configuration register 1 */
    volatile uint32_t RCR;          /*!< 0x04:  SYSCFG CCM SRAM protection register */
    volatile uint32_t EXTICR1;      /*!< 0x08:  SYSCFG external interrupt configuration register 1 */
    volatile uint32_t EXTICR2;      /*!< 0x0C:  SYSCFG external interrupt configuration register 2 */
    volatile uint32_t EXTICR3;      /*!< 0x10:  SYSCFG external interrupt configuration register 3 */
    volatile uint32_t EXTICR4;      /*!< 0x14:  SYSCFG external interrupt configuration register 4 */
    volatile uint32_t CFGR2;        /*!< 0x18:  SYSCFG configuration register 2 */

} SYSCFG_TypeDef;

/**
 * @brief EXTI peripheral register map.
 */
typedef struct
{
    volatile uint32_t IMR1;         /*!< 0x00: EXTI interrupt mask register 1 */
    volatile uint32_t EMR1;         /*!< 0x04: EXTI event mask register 1 */
    volatile uint32_t RTSR1;        /*!< 0x08: EXTI rising trigger selection register 1 */
    volatile uint32_t FTSR1;        /*!< 0x0C: EXTI falling trigger selection register 1 */
    volatile uint32_t SWIER1;       /*!< 0x10: EXTI software interrupt event register 1 */
    volatile uint32_t PR1;          /*!< 0x14: EXTI pending register 1 (W1C) */
    volatile uint32_t IMR2;         /*!< 0x18: EXTI interrupt mask register 2 */
    volatile uint32_t EMR2;         /*!< 0x1C: EXTI event mask register 2 */
    volatile uint32_t RTSR2;        /*!< 0x20: EXTI rising trigger selection register 2 */
    volatile uint32_t FTSR2;        /*!< 0x24: EXTI falling trigger selection register 2 */
    volatile uint32_t SWIER2;       /*!< 0x28: EXTI software interrupt event register 2 */
    volatile uint32_t PR2;          /*!< 0x2C: EXTI pending register 2 (W1C) */

} EXTI_TypeDef;


/**
 * @brief RCC peripheral register map.
 */
typedef struct
{
    volatile uint32_t CR;           /*!< 0x00: RCC clock control register */
    volatile uint32_t CFGR;         /*!< 0x04: RCC clock configuration register */
    volatile uint32_t CIR;          /*!< 0x08: RCC clock interrupt register */
    volatile uint32_t APB2RSTR;     /*!< 0x0C: RCC APB2 peripheral reset register */
    volatile uint32_t APB1RSTR;     /*!< 0x10: RCC APB1 peripheral reset register */
    volatile uint32_t AHBENR;       /*!< 0x14: RCC AHB peripheral clock enable register */
    volatile uint32_t APB2ENR;      /*!< 0x18: RCC APB2 peripheral clock enable register */
    volatile uint32_t APB1ENR;      /*!< 0x1C: RCC APB1 peripheral clock enable register */
    volatile uint32_t BDCR;         /*!< 0x20: RCC backup domain control register */
    volatile uint32_t CSR;          /*!< 0x24: RCC clock control and status register */
    volatile uint32_t AHBRSTR;      /*!< 0x28: RCC AHB peripheral reset register */
    volatile uint32_t CFGR2;        /*!< 0x2C: RCC clock configuration register 2 */
    volatile uint32_t CFGR3;        /*!< 0x30: RCC clock configuration register 3 */

} RCC_TypeDef;


/**
 * @brief GPIO peripheral register map.
 */
typedef struct
{
    volatile uint32_t MODER;        /*!< 0x00: GPIO port mode register */
    volatile uint32_t OTYPER;       /*!< 0x04: GPIO port output type register */
    volatile uint32_t OSPEEDR;      /*!< 0x08: GPIO port output speed register */
    volatile uint32_t PUPDR;        /*!< 0x0C: GPIO port pull-up/pull-down register */
    volatile uint32_t IDR;          /*!< 0x10: GPIO port input data register */
    volatile uint32_t ODR;          /*!< 0x14: GPIO port output data register */
    volatile uint32_t BSRR;         /*!< 0x18: GPIO port bit set/reset register */
    volatile uint32_t LCKR;         /*!< 0x1C: GPIO port configuration lock register */
    volatile uint32_t AFR[2];       /*!< 0x20-0x24: GPIO alternate function registers */
    volatile uint32_t BRR;          /*!< 0x28: GPIO port bit reset register */

} GPIO_TypeDef;

/**
 * @brief USART register map
 */
typedef struct
{
    volatile uint32_t USART_CR1;    /*!< 0x00: USART control register 1 */
    volatile uint32_t USART_CR2;    /*!< 0x04: USART control register 2 */
    volatile uint32_t USART_CR3;    /*!< 0x08: USART control register 3 */
    volatile uint32_t USART_BRR;    /*!< 0x0C: USART baud rate register */
    volatile uint32_t USART_GTPR;   /*!< 0x10: USART guard time and prescaler register */
    volatile uint32_t USART_RTOR;   /*!< 0x14: USART receiver timout register */
    volatile uint32_t USART_RQR;    /*!< 0x18: USART request register */
    volatile uint32_t USART_ISR;    /*!< 0x1C: USART interrupt status register */
    volatile uint32_t USART_ICR;    /*!< 0x20: USART interrupt flag clear register */
    volatile uint32_t USART_RDR;    /*!< 0x24: USART receive data register */
    volatile uint32_t USART_TDR;    /*!< 0x28: USART transmitt data register */

} USART_TypeDef;

/**
 * @brief STM32F303 external interrupt numbers.
 *
 * Values correspond to the interrupt positions listed in RM0316 Table 82
 * and are used as IRQ numbers when accessing the NVIC.
 */
typedef enum
{
    EXTI0_IRQn       = 6U,          /*!< EXTI line 0 interrupt */
    EXTI1_IRQn       = 7U,          /*!< EXTI line 1 interrupt */
    EXTI2_TS_IRQn    = 8U,          /*!< EXTI line 2 and touch sensing interrupt */
    EXTI3_IRQn       = 9U,          /*!< EXTI line 3 interrupt */
    EXTI4_IRQn       = 10U,         /*!< EXTI line 4 interrupt */
    EXTI9_5_IRQn     = 23U,         /*!< EXTI lines [9:5] interrupt */
    EXTI15_10_IRQn   = 40U          /*!< EXTI lines [15:10] interrupt */
} IRQn_Type;

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

#define APB1PERIPH_BASE         (PERIPH_BASE + 0x00000000UL)
#define APB2PERIPH_BASE         (PERIPH_BASE + 0x00010000UL)
#define AHB1PERIPH_BASE         (PERIPH_BASE + 0x00020000UL)
#define AHB2PERIPH_BASE         (PERIPH_BASE + 0x08000000UL)
#define AHB3PERIPH_BASE         (PERIPH_BASE + 0x10000000UL)

/* APB2 peripherals */
#define SYSCFG_BASE             (APB2PERIPH_BASE + 0x00000000UL)
#define EXTI_BASE               (APB2PERIPH_BASE + 0x00000400UL)
#define USART1_BASE             (APB2PERIPH_BASE + 0x00003800UL)


/* AHB1 peripherals */
#define RCC_BASE                (AHB1PERIPH_BASE + 0x00001000UL)


/* AHB2 peripherals */
#define GPIOA_BASE              (AHB2PERIPH_BASE + 0x00000000UL)
#define GPIOB_BASE              (AHB2PERIPH_BASE + 0x00000400UL)
#define GPIOC_BASE              (AHB2PERIPH_BASE + 0x00000800UL)
#define GPIOD_BASE              (AHB2PERIPH_BASE + 0x00000C00UL)
#define GPIOE_BASE              (AHB2PERIPH_BASE + 0x00001000UL)
#define GPIOF_BASE              (AHB2PERIPH_BASE + 0x00001400UL)

/** @} */

/** @addtogroup Peripheral_instances
 * @{
 */
/* ---------------------------------------------------------------------------
 *                          PERIPHERAL INSTANCES
 * ------------------------------------------------------------------------- */
#define SYSCFG                  ((SYSCFG_TypeDef *)SYSCFG_BASE)
#define EXTI                    ((EXTI_TypeDef *)EXTI_BASE)

#define RCC                     ((RCC_TypeDef *)RCC_BASE)

#define GPIOA                   ((GPIO_TypeDef *)GPIOA_BASE)
#define GPIOB                   ((GPIO_TypeDef *)GPIOB_BASE)
#define GPIOC                   ((GPIO_TypeDef *)GPIOC_BASE)
#define GPIOD                   ((GPIO_TypeDef *)GPIOD_BASE)
#define GPIOE                   ((GPIO_TypeDef *)GPIOE_BASE)
#define GPIOF                   ((GPIO_TypeDef *)GPIOF_BASE)

#define USART1                  ((USART_TypeDef *) USART1_BASE)

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

/* .................................................................. APB2ENR */

#define RCC_APB2ENR_SYSCFGEN_Pos            (0U)
#define RCC_APB2ENR_SYSCFGEN_Msk            (1UL << RCC_APB2ENR_SYSCFGEN_Pos)

#define RCC_APB2ENR_USART1EN_Pos            (14U)
#define RCC_APB2ENR_USART1EN_Msk            (1UL << RCC_APB2ENR_USART1EN_Pos)

/* .................................................................. AHBENR */

#define RCC_AHBENR_GPIOAEN_Pos              (17U)
#define RCC_AHBENR_GPIOAEN_Msk              (1UL << RCC_AHBENR_GPIOAEN_Pos)

#define RCC_AHBENR_GPIOBEN_Pos              (18U)
#define RCC_AHBENR_GPIOBEN_Msk              (1UL << RCC_AHBENR_GPIOBEN_Pos)

#define RCC_AHBENR_GPIOCEN_Pos              (19U)
#define RCC_AHBENR_GPIOCEN_Msk              (1UL << RCC_AHBENR_GPIOCEN_Pos)

#define RCC_AHBENR_GPIODEN_Pos              (20U)
#define RCC_AHBENR_GPIODEN_Msk              (1UL << RCC_AHBENR_GPIODEN_Pos)

#define RCC_AHBENR_GPIOEEN_Pos              (21U)
#define RCC_AHBENR_GPIOEEN_Msk              (1UL << RCC_AHBENR_GPIOEEN_Pos)

#define RCC_AHBENR_GPIOFEN_Pos              (22U)
#define RCC_AHBENR_GPIOFEN_Msk              (1UL << RCC_AHBENR_GPIOFEN_Pos)

/* ...........................................................................
 *                         USART
 * ......................................................................... */

/* .................................................................... CR1 */

#define USART_CR1_UE_Pos                    (0U)
#define USART_CR1_UE_Msk                    (1UL << USART_CR1_UE_Pos)

#define USART_CR1_RE_Pos                    (2U)
#define USART_CR1_RE_Msk                    (1UL << USART_CR1_RE_Pos)

#define USART_CR1_TE_Pos                    (3U)
#define USART_CR1_TE_Msk                    (1UL << USART_CR1_TE_Pos)

#define USART_CR1_M0_Pos                    (12U)
#define USART_CR1_M0_Msk                    (1UL << USART_CR1_M0_Pos)

#define USART_CR1_M1_Pos                    (28U)
#define USART_CR1_M1_Msk                    (1UL << USART_CR1_M1_Pos)

#define USART_CR1_PCE_Pos                   (10U)
#define USART_CR1_PCE_Msk                   (1UL << USART_CR1_PCE_Pos)

/* .................................................................... CR2 */

#define USART_CR2_STOP_Pos                  (12U)
#define USART_CR2_STOP_Msk                  (3UL << USART_CR2_STOP_Pos)

/* .................................................................... ISR */

#define USART_ISR_RXNE_Pos                  (5U)
#define USART_ISR_RXNE_Msk                  (1UL << USART_ISR_RXNE_Pos)

#define USART_ISR_TXE_Pos                   (7U)
#define USART_ISR_TXE_Msk                   (1UL << USART_ISR_TXE_Pos)

/** @} */


#endif /* STM32F303XC_H_ */