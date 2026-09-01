cmake_minimum_required(VERSION 3.20)

# Define the embedded firmware project and enable C and assembly support.
project(stm32-real-time-tinyml ASM C)

# Create the STM32 firmware ELF executable from startup and application sources
# The ELF file contains symbol information required for source-level debugging.
add_executable(firmware.elf
    startup/startup_stm32f303xc.s
    src/main.c
)

# Configure compiler options for the STM32F3 Cortex-M4 processor.
# Thumb mode enables the compact instruction set used by Cortex-M devices.
# -g generates debug information for debugging with a debugger.
# -O0 disables compiler optimization to keep code behavior easy to trace.
target_compile_options(firmware.elf PRIVATE
    -mcpu=cortex-m4
    -mthumb
    -g
    -O0
)

# Configure the linker for the bare-metal STM32 firmware image.
# The linker script defines memory layout and firmware section placement.
# -nostdlib removes dependency on the default C runtime and standard libraries.
target_link_options(firmware.elf PRIVATE
    -T${CMAKE_SOURCE_DIR}/linker/linker_stm32f303xc.ld
    -nostdlib
)