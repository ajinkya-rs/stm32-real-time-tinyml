cmake_minimum_required(VERSION 3.20)

# Define the embedded firmware project with C and assembly language support.
project(stm32-real-time-tinyml ASM C)

# Create the STM32 firmware ELF executable from startup and application sources.
# The ELF file contains symbol information and memory addresses for debugging.
add_executable(firmware.elf
    startup/startup_stm32f303x.s
    src/main.c
)

# Configure compiler options for the STM32F3 Cortex-M4 target.
# Thumb mode enables the compact instruction set used by Cortex-M processors.
target_compile_options(firmware.elf PRIVATE
    -mcpu=cortex-m4
    -mthumb
    -g
    -O0
)

# Configure linker options for the bare-metal STM32 firmware image.
# The linker script defines memory layout and places firmware sections.
# -nostdlib disables the default C runtime and standard library linking.
target_link_options(firmware.elf PRIVATE
    -T${CMAKE_SOURCE_DIR}/linker/linker_stm32f303x.ld
    -nostdlib
)