# Toolchain file for cross-compiling firmware for ARM microcontrollers.
# CMake uses these settings before project() to select the target compiler.
set(CMAKE_SYSTEM_NAME Generic)

# Target CPU architecture. The Cortex-M core is specified with compiler flags.
set(CMAKE_SYSTEM_PROCESSOR arm)

# ARM GCC compiler used to build C source files for the microcontroller.
set(CMAKE_C_COMPILER arm-none-eabi-gcc)

# Use a static library during compiler checks because bare-metal targets
# do not support normal host executable linking.
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)