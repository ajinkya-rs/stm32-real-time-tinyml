# STM32 ARM Cortex-M Bare-Metal Setup

A minimal STM32F3 bare-metal project using CMake, Ninja, and the GNU Arm toolchain.

## Hardware

- STM32F3 with Cortex-M4 core
- ST-LINK debug probe over SWD
- Detected flash size: 256 KB
- Flash start address: `0x08000000`
- RAM start address: `0x20000000`
- RAM size: 40 KB

## Tools

- VS Code
- CMake
- Ninja
- GNU Arm Embedded Toolchain (`arm-none-eabi-gcc`)
- OpenOCD
- ST-LINK

## Build

### with ARM-Toolchain

```bash
arm-none-eabi-gcc \
    -mcpu=cortex-m4 \
    -mthumb \
    -nostdlib \
    -T linker/linker_stm32f303xc.ld \
    -o firmware.elf \
    src/main.c
```

### with CMake

Configure the project:
```bash
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=cmake/toolchain/arm-none-eabi.cmake
```

Build the firmware ELF:
```bash
cmake --build build
```

## Documentation

Project documentation is organized in the  `docs/` directory.

- [Reset and Startup Sequence of ARM Cortex-M](docs/reset_and_startup_sequence.md)
- [ELF Inspection, Flashing and Debugging](docs/inspection_and_debugging_tools.md)