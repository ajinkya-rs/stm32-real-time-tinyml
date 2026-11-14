/*
 * =============================================================================
 * File:    startup_stm32f303xc.s
 * Brief:   Minimal STM32F3 bare-metal startup code.
 *
 * Defines the Cortex-M vector table and startup code:
 *
 *   1. Initial Main Stack Pointer (MSP)
 *   2. Reset_Handler address
 *   3. Cortex-M system exception vectors
 *   4. STM32F303xC external interrupt vectors
 *
 * After reset, the Cortex-M processor loads the initial Main Stack Pointer (MSP)
 * and the reset vector from the vector table. 
 * The reset vector provides the address of `Reset_Handler`, which is the 
 * first code executed after reset.
 *
 * The linker script places the .isr_vector section at Flash address
 * 0x08000000.
 * =============================================================================
 */


/*
 * Cortex-M vector table.
 *
 * The first 16 entries are defined by the Cortex-M4 architecture:
 *   0:  Initial Main Stack Pointer (MSP)
 *   1:  Reset_Handler address
 *   2-15: System exceptions
 *
 * The "a" flag marks this section as allocatable by the linker.
 */
.section .isr_vector, "a"

.word   _end_address_of_stack       /* Initial Main Stack Pointer */
.word   Reset_Handler               /* Reset */

.word   Default_Handler             /* NMI */
.word   Default_Handler             /* HardFault */
.word   Default_Handler             /* MemManage */
.word   Default_Handler             /* BusFault */
.word   Default_Handler             /* UsageFault */
.word   Default_Handler             /* Reserved */
.word   Default_Handler             /* Reserved */
.word   Default_Handler             /* Reserved */
.word   Default_Handler             /* Reserved */
.word   Default_Handler             /* SVCall */
.word   Default_Handler             /* DebugMonitor */
.word   Default_Handler             /* Reserved */
.word   Default_Handler             /* PendSV */
.word   Default_Handler             /* SysTick */


/*
 * STM32F303xC external interrupt vectors.
 *
 * Positions correspond to the interrupt positions listed in RM0316 Table 82.
 * Reserved positions are kept to preserve the vector-table layout.
 */
.word   Default_Handler             /* Position 0  - WWDG */
.word   Default_Handler             /* Position 1  - PVD */
.word   Default_Handler             /* Position 2  - TAMPER_STAMP */
.word   Default_Handler             /* Position 3  - RTC_WKUP */
.word   Default_Handler             /* Position 4  - FLASH */
.word   Default_Handler             /* Position 5  - RCC */
.word   EXTI0_IRQHandler            /* Position 6  - EXTI0 */
.word   EXTI1_IRQHandler            /* Position 7  - EXTI1 */
.word   EXTI2_TS_IRQHandler         /* Position 8  - EXTI2_TS */
.word   EXTI3_IRQHandler            /* Position 9  - EXTI3 */
.word   EXTI4_IRQHandler            /* Position 10 - EXTI4 */
.word   Default_Handler             /* Position 11 - DMA1_Channel1 */
.word   Default_Handler             /* Position 12 - DMA1_Channel2 */
.word   Default_Handler             /* Position 13 - DMA1_Channel3 */
.word   Default_Handler             /* Position 14 - DMA1_Channel4 */
.word   Default_Handler             /* Position 15 - DMA1_Channel5 */
.word   Default_Handler             /* Position 16 - DMA1_Channel6 */
.word   Default_Handler             /* Position 17 - DMA1_Channel7 */
.word   Default_Handler             /* Position 18 - ADC1_2 */
.word   Default_Handler             /* Position 19 - USB_HP/CAN_TX */
.word   Default_Handler             /* Position 20 - USB_LP/CAN_RX0 */
.word   Default_Handler             /* Position 21 - CAN_RX1 */
.word   Default_Handler             /* Position 22 - CAN_SCE */
.word   EXTI9_5_IRQHandler          /* Position 23 - EXTI9_5 */
.word   Default_Handler             /* Position 24 - TIM1_BRK/TIM15 */
.word   Default_Handler             /* Position 25 - TIM1_UP/TIM16 */
.word   Default_Handler             /* Position 26 - TIM1_TRG_COM/TIM17 */
.word   Default_Handler             /* Position 27 - TIM1_CC */
.word   Default_Handler             /* Position 28 - TIM2 */
.word   Default_Handler             /* Position 29 - TIM3 */
.word   Default_Handler             /* Position 30 - Reserved */
.word   Default_Handler             /* Position 31 - I2C1_EV */
.word   Default_Handler             /* Position 32 - I2C1_ER */
.word   Default_Handler             /* Position 33 - Reserved */
.word   Default_Handler             /* Position 34 - Reserved */
.word   Default_Handler             /* Position 35 - SPI1 */
.word   Default_Handler             /* Position 36 - Reserved */
.word   USART1_IRQHandler           /* Position 37 - USART1 */
.word   Default_Handler             /* Position 38 - USART2 */
.word   Default_Handler             /* Position 39 - USART3 */
.word   EXTI15_10_IRQHandler        /* Position 40 - EXTI15_10 */
.word   Default_Handler             /* Position 41 - RTC_Alarm */
.word   Default_Handler             /* Position 42 - USBWakeUp */
.word   Default_Handler             /* Position 43 - TIM8_BRK */
.word   Default_Handler             /* Position 44 - TIM8_UP */
.word   Default_Handler             /* Position 45 - TIM8_TRG_COM */
.word   Default_Handler             /* Position 46 - TIM8_CC */
.word   Default_Handler             /* Position 47 - ADC3 */
.word   Default_Handler             /* Position 48 - FMC */
.word   Default_Handler             /* Position 49 - Reserved */
.word   Default_Handler             /* Position 50 - Reserved */
.word   Default_Handler             /* Position 51 - SPI3 */
.word   Default_Handler             /* Position 52 - UART4 */
.word   Default_Handler             /* Position 53 - UART5 */
.word   Default_Handler             /* Position 54 - TIM6_DAC */
.word   Default_Handler             /* Position 55 - TIM7 */
.word   Default_Handler             /* Position 56 - DMA2_Channel1 */
.word   Default_Handler             /* Position 57 - DMA2_Channel2 */
.word   Default_Handler             /* Position 58 - DMA2_Channel3 */
.word   Default_Handler             /* Position 59 - DMA2_Channel4 */
.word   Default_Handler             /* Position 60 - DMA2_Channel5 */
.word   Default_Handler             /* Position 61 - ADC4 */
.word   Default_Handler             /* Position 62 - Reserved */
.word   Default_Handler             /* Position 63 - Reserved */
.word   Default_Handler             /* Position 64 - COMP1_2_3 */
.word   Default_Handler             /* Position 65 - COMP4_5_6 */
.word   Default_Handler             /* Position 66 - COMP7 */
.word   Default_Handler             /* Position 67 - Reserved */
.word   Default_Handler             /* Position 68 - Reserved */
.word   Default_Handler             /* Position 69 - Reserved */
.word   Default_Handler             /* Position 70 - Reserved */
.word   Default_Handler             /* Position 71 - Reserved */
.word   Default_Handler             /* Position 72 - I2C3_EV */
.word   Default_Handler             /* Position 73 - I2C3_ER */
.word   Default_Handler             /* Position 74 - USB_HP */
.word   Default_Handler             /* Position 75 - USB_LP */
.word   Default_Handler             /* Position 76 - USB_WakeUp_RMP */
.word   Default_Handler             /* Position 77 - TIM20_BRK */
.word   Default_Handler             /* Position 78 - TIM20_UP */
.word   Default_Handler             /* Position 79 - TIM20_TRG_COM */
.word   Default_Handler             /* Position 80 - TIM20_CC */
.word   Default_Handler             /* Position 81 - FPU */
.word   Default_Handler             /* Position 82 - Reserved */
.word   Default_Handler             /* Position 83 - Reserved */
.word   Default_Handler             /* Position 84 - SPI4 */


/*
 * Executable code section containing startup routines.
 */
.section .text


/*
 * Export Reset_Handler as a function symbol so it can be referenced by
 * the vector table and linker.
 */
.type   Reset_Handler, %function
.global Reset_Handler


/*
 * Reset entry point.
 *
 * First instruction executed after reset.
 * Prepares C runtime memory and starts main().
 */
Reset_Handler:


/*
 * Copy initialized .data values from FLASH to RAM.
 *
 * Source      : initial .data values in FLASH
 * Destination : .data start in RAM
 * End         : end address of .data in RAM
 */
ldr     r0, =_sidata
ldr     r1, =_sdata
ldr     r2, =_edata


copy_data:
    cmp     r1, r2              /* Stop when all .data is copied */
    bcs     data_done

    ldr     r3, [r0]            /* Read word from FLASH */
    str     r3, [r1]            /* Write word to RAM */

    add     r0, r0, #4          /* Next FLASH address */
    add     r1, r1, #4          /* Next RAM address */

    b       copy_data


data_done:

/*
 * Clear .bss section.
 *
 * .bss contains variables without initial values.
 * C requires them to start as zero before main().
 */
ldr     r0, =_sbss              /* Start of .bss in RAM */
ldr     r1, =_ebss              /* End of .bss in RAM */
movs    r2, #0                  /* Value used to clear memory */


clear_bss:
    cmp     r0, r1              /* Stop when entire .bss is cleared */
    bcs     bss_done

    str     r2, [r0]            /* Write zero to RAM */

    add     r0, r0, #4          /* Next RAM address */

    b       clear_bss


bss_done:


/*
 * C runtime is ready.
 * Enter application code.
 */
bl      main


/*
 * main() should not return.
 * Stay here if it does to avoid executing invalid memory.
 */
b       .

/*
 * Default handler.
 * 
 * Interrupts without a dedicated handler use this function.
 * The handler loops indefinitely until reset.
 */
.type   Default_Handler, %function
.global Default_Handler

Default_Handler:
    b       .

/*
 * Weak EXTI handler symbols.
 *
 * The vector table references these symbols before the actual EXTI
 * interrupt handlers are implemented.
 *
 * The weak aliases allow the default handler to be used until the 
 * dedicated handlers are provided by the EXTI interrupt-handler
 * implementation.
 */
.weak   EXTI0_IRQHandler
.set    EXTI0_IRQHandler, Default_Handler

.weak   EXTI1_IRQHandler
.set    EXTI1_IRQHandler, Default_Handler

.weak   EXTI2_TS_IRQHandler
.set    EXTI2_TS_IRQHandler, Default_Handler

.weak   EXTI3_IRQHandler
.set    EXTI3_IRQHandler, Default_Handler

.weak   EXTI4_IRQHandler
.set    EXTI4_IRQHandler, Default_Handler

.weak   EXTI9_5_IRQHandler
.set    EXTI9_5_IRQHandler, Default_Handler

.weak   EXTI15_10_IRQHandler
.set    EXTI15_10_IRQHandler, Default_Handler

/* Weak USART handler symbols */
.weak   USART1_IRQHandler
.set    USART1_IRQHandler, Default_Handler
