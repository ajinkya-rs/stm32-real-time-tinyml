cmake_minimum_required(VERSION 3.20)

# Define the embedded firmware project and enable C and assembly support.
project(stm32-real-time-tinyml ASM C)

# Create the STM32 firmware ELF executable from startup and application sources
# The ELF file contains symbol information required for source-level debugging.
add_executable(firmware.elf
    startup/startup_stm32f303xc.s
    driver/src/rcc.c
    driver/src/gpio.c
    driver/src/syscfg.c
    driver/src/exti.c
    driver/src/nvic.c
    src/main.c
)

# Configure compiler options for the STM32F303xC Cortex-M4 target.
# Thumb mode enables the compact instruction set used by Cortex-M devices.
# -g generates debug information for debugging with a debugger.
# -O0 disables compiler optimization to keep code behavior easy to trace.
target_compile_options(firmware.elf PRIVATE
    -mcpu=cortex-m4
    -mthumb
    -g
    -O0
)

# Make the device-specific STM32F303xC header available.
target_include_directories(firmware.elf PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/core
    ${CMAKE_CURRENT_SOURCE_DIR}/device/include
    ${CMAKE_CURRENT_SOURCE_DIR}/driver/include
)


# Configure the linker for the bare-metal STM32 firmware image.
# The linker script defines memory layout and firmware section placement.
# -nostdlib removes dependency on the default C runtime and standard libraries.
target_link_options(firmware.elf PRIVATE
    -T${CMAKE_SOURCE_DIR}/linker/linker_stm32f303xc.ld
    -nostdlib
)