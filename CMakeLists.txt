cmake_minimum_required(VERSION 3.20)

# Define the embedded firmware project and enable C language support.
project(stm32-real-time-tinyml C)

# Create the STM32 firmware ELF executable from the application source file.
# The ELF output contains debug symbols and memory information for debugging.
add_executable(firmware.elf
    src/main.c
)

# Configure compiler options for the STM32F3 Cortex-M4 processor.
# Thumb mode generates the compact instruction set used by Cortex-M devices.
target_compile_options(firmware.elf PRIVATE
    -mcpu=cortex-m4
    -mthumb
)

# Configure the linker for a bare-metal STM32 firmware image.
# The linker script defines flash/RAM memory placement and startup layout.
# -nostdlib removes dependency on the host C runtime and standard libraries.
target_link_options(firmware.elf PRIVATE 
    -T${CMAKE_SOURCE_DIR}/linker/linker_stm32f303x.ld
    -nostdlib
)