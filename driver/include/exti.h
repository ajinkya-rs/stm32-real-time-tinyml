/**
 * ****************************************************************************
 * @file    exti.h
 * @brief   EXTI driver interface.
 *
 * @details
 * Provides configuration of external interrupt lines and their trigger
 * conditions.
 *
 * @author  Ajinkya Shinde
 * @date    2026-09-17
 * ****************************************************************************
 */

#ifndef EXTI_H_
#define EXTI_H_

#include<stdint.h>

/* ***************************************************************************
 *                        ENUMS, TYPEDEFS and DEFINES
 * ************************************************************************* */

/**
 * @brief EXTI trigger selection.
 */
typedef enum 
{
    EXTI_TRIGGER_RISING,
    EXTI_TRIGGER_FALLING,
    EXTI_TRIGGER_RISING_FALLING

} EXTI_Trigger_t;


/* ***************************************************************************
 *                        FUNCTION PROTOTYPES
 * ************************************************************************* */

/**
 * @brief Enable interrupt generation for the an EXTI line
 * 
 * @param[in] exti_line
 * EXTI line number (0 to 15).
 */
void EXTI_EnableInterrupt (uint8_t exti_line);

/**
 * @brief Configure the trigger condition for an EXTI line
 * 
 * @param[in] exti_line
 * EXTI line number (0 to 15).
 * 
 * @param[in] trigger
 * Trigger condition for the EXTI line
 */
void EXTI_SelectTrigger (uint8_t exti_line, EXTI_Trigger_t trigger);

#endif /* EXTI_H_ */