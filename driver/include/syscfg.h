/**
 * ****************************************************************************
 * @file    syscfg.h
 * @brief   SYSCFG driver interface.
 *
 * @details
 * Provides configuration of the GPIO port connected to an EXTI line.
 *
 * @author  Ajinkya Shinde
 * @date    2026-09-16
 * ****************************************************************************
 */


#ifndef SYSCFG_H_
#define SYSCFG_H_

#include <stdint.h>

/* ***************************************************************************
 *                        ENUMS, TYPEDEFS and DEFINES
 * ************************************************************************* */
/* Total number of EXTI lines supported by the device. */
#define TOTAL_EXTI_LINES             16U

/* Number of EXTI lines configured by each EXTICR register. */
#define EXTI_LINES_PER_REGISTER       4U

/* Number of bits allocated to each EXTI configuration field. */
#define EXTI_CONFIGURATION_BITS       4U

/**
 * @brief GPIO port selection for an EXTI line.
 */
typedef enum
{
    EXTI_PORT_A = 0U,                   /**< Select GPIO port A. */
    EXTI_PORT_B = 1U,                   /**< Select GPIO port B. */
    EXTI_PORT_C = 2U,                   /**< Select GPIO port C. */
    EXTI_PORT_D = 3U,                   /**< Select GPIO port D. */
    EXTI_PORT_E = 4U,                   /**< Select GPIO port E. */
    EXTI_PORT_F = 5U                    /**< Select GPIO port F. */
} SYSCFG_EXTI_Port_t;


/* ***************************************************************************
 *                        FUNCTION PROTOTYPES
 * ************************************************************************* */

/**
 * @brief Select the GPIO port connected to an EXTI line.
 *
 * @param[in] exti_line
 * EXTI line number (0 to 15).
 *
 * @param[in] port
 * GPIO port selected as the EXTI source.
 *
 * @details
 * The EXTI line number corresponds to the GPIO pin number.
 *
 * Example:
 *     SYSCFG_EXTI_SetPort(0U, EXTI_PORT_A);
 *
 * selects PA0 as the source for EXTI0.
 */
void SYSCFG_EXTI_SetPort(uint8_t exti_line, SYSCFG_EXTI_Port_t port);

#endif /* SYSCFG_H_ */