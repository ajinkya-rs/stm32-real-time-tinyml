/**
 ******************************************************************************
 * @file    core_cm4.h
 * @brief   Cortex-M4 specific header.
 *
 * @details
 * The file provides the register definitions and base addresses of the
 * Cortex-M4 processor core.
 *
 * @author  Ajinkya Shinde
 * @date    2026-09-18
 ******************************************************************************
 */

#ifndef CORE_M4_H_
#define CORE_M4_H_

/* ***************************************************************************
 *                              INCLUDES
 * ************************************************************************* */

#include <stdint.h>

/* ***************************************************************************
 *                    STRUCTURE, ENUMS AND TYPE-DEFINITIONS
 * ************************************************************************* */

/** @addtogroup Core_peripheral_registers
 * @{
 */

/**
 * @brief NVIC register map.
 */
typedef struct
{
    volatile uint32_t ISER[8U];        /* 0x000: Interrupt Set-Enable Registers */
    uint32_t RESERVED0[24U];           /* 0x020: Reserved */

    volatile uint32_t ICER[8U];        /* 0x080: Interrupt Clear-Enable Registers */
    uint32_t RESERVED1[24U];           /* 0x0A0: Reserved */

    volatile uint32_t ISPR[8U];        /* 0x100: Interrupt Set-Pending Registers */
    uint32_t RESERVED2[24U];           /* 0x120: Reserved */

    volatile uint32_t ICPR[8U];        /* 0x180: Interrupt Clear-Pending Registers */
    uint32_t RESERVED3[24U];           /* 0x1A0: Reserved */

    volatile uint32_t IABR[8U];        /* 0x200: Interrupt Active-Bit Registers */
    uint32_t RESERVED4[56U];           /* 0x220: Reserved */

    volatile uint8_t IPR[240U];        /* 0x300: Interrupt Priority Registers */
    uint32_t RESERVED5[644U];          /* 0x3F0: Reserved */

    volatile uint32_t STIR;            /* 0xE00: Software Trigger Interrupt Register */

} NVIC_TypeDef;

/** @} */

/* ***************************************************************************
 *                                DEFINES
 * ************************************************************************* */

/** @addtogroup Core_peripheral_memory_map
 * @{
 */

/* ---------------------------------------------------------------------------
 *                        CORE PERIPHERAL MEMORY MAP
 * ------------------------------------------------------------------------- */

#define NVIC_BASE   0xE000E100UL  /* NVIC base address. */

/** @} */

/** @addtogroup Core_peripheral_instances
 * @{
 */

/* ---------------------------------------------------------------------------
 *                       CORE PERIPHERAL INSTANCES
 * ------------------------------------------------------------------------- */

#define NVIC    ((NVIC_TypeDef *)NVIC_BASE)

/** @} */

#endif /* CORE_M4_H_ */