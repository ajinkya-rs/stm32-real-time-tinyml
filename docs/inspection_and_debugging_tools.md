# ELF Inspection, Flashing and Debugging

This section covers the tools used to inspect the STM32 ELF,
flash the firmware, and debug it with OpenOCD.

**Project target:** STM32F3 / Cortex-M4 / ST-LINK over SWD


## 1. Firmware Inspection Tools

Use the ELF as the main debug artifact:

```text
build/firmware.elf
```

### A. `readelf` — Inspect ELF Structure

```bash
arm-none-eabi-readelf -h build/firmware.elf
```

- `-h` → show the ELF header, architecture, entry point, and file type.

```bash
arm-none-eabi-readelf -S build/firmware.elf
```

- `-S` → show all ELF sections, addresses, sizes, and attributes.

```bash
arm-none-eabi-readelf -s build/firmware.elf
```

- `-s` → show the symbol table.

Useful symbols:

- `Reset_Handler`
- `_sidata`
- `_sdata`
- `_edata`
- `_sbss`
- `_ebss`
- `main`

### B. `nm` — Inspect Symbols and Addresses

Display ELF symbols sorted by address:

```bash
arm-none-eabi-nm -n build/firmware.elf
```

- `-n` → sort symbols by address.
- Useful for verifying linker symbols and memory layout.

For focused checking:

```bash
arm-none-eabi-nm -n build/firmware.elf | grep -E 'Reset_Handler|main|sdata|edata|sidata|sbss|ebss'
```

### C. `objdump` — Inspect Sections and Disassembly

```bash
arm-none-eabi-objdump -h build/firmware.elf
```

- `-h` → show section layout, including VMA and LMA.

```bash
arm-none-eabi-objdump -d build/firmware.elf
```

- `-d` → disassemble executable code.

```bash
arm-none-eabi-objdump -t build/firmware.elf
```

- `-t` → show the symbol table.

Expected layout:

```text
.isr_vector → FLASH
.text        → FLASH
.rodata      → FLASH
.data image  → FLASH
.data        → RAM at runtime
.bss         → RAM, NOLOAD
```

### D. `objcopy` — Create Firmware Image Formats

Create Intel HEX:

```bash
arm-none-eabi-objcopy -O ihex build/firmware.elf build/firmware.hex
```

Create raw binary:

```bash
arm-none-eabi-objcopy -O binary build/firmware.elf build/firmware.bin
```

Use the ELF for development and debugging because it keeps
symbols and address information.

Use `.hex` or `.bin` when a raw firmware image is required.


## 2. Flash Firmware with OpenOCD

**Hardware:**

```text
STM32F3
   │
   │ SWD
   ▼
ST-LINK
   │
   ▼
OpenOCD
```

### Start OpenOCD

From the project root:

```bash
openocd -f interface/stlink.cfg -f target/stm32f3x.cfg
```

OpenOCD should detect the ST-LINK and STM32 target.

Keep this terminal running when using OpenOCD as a GDB server.

### Flash the Firmware

The simplest ELF programming flow is:

```bash
openocd -f interface/stlink.cfg -f target/stm32f3x.cfg -c "program build/firmware.elf verify reset exit"
```

OpenOCD programs the loadable firmware contents described by the ELF:

```text
FLASH
├── .isr_vector → vector table
├── .text       → executable firmware code
├── .rodata     → read-only constants
└── .data image → initial values for RAM
```

`.bss` is not programmed into Flash because it is `NOLOAD`.
Startup creates its zero-initialized RAM state before `main()`.


## 3. Debug with OpenOCD + GDB

Use two terminals.

### Terminal 1 — Start OpenOCD

```bash
openocd -f interface/stlink.cfg -f target/stm32f3x.cfg
```

Leave OpenOCD running.

The GDB server is normally available at:

```text
localhost:3333
```

### Terminal 2 — Start GDB

```bash
arm-none-eabi-gdb build/firmware.elf
```

Connect to OpenOCD:

```gdb
target extended-remote localhost:3333
```

Always begin from a known reset state:

```gdb
monitor reset halt
```


### Step 1 — Check CPU Reset State

```gdb
info registers pc msp
```

Verify:

```text
MSP → RAM
PC  → Reset_Handler / startup code
```

### Step 2 — Verify Vector Table

```gdb
x/2wx 0x08000000
```

Expected:

```text
Word 0 → initial MSP
Word 1 → Reset_Handler
```

Disassemble the reset handler:

```gdb
x/10i Reset_Handler
```

or:

```gdb
disassemble Reset_Handler
```

### Step 3 — Verify `.data` Initialization

Set a breakpoint after `.data` copying:

```gdb
break data_done
continue
```

Check the linker symbols:

```gdb
p/x &_sidata
p/x &_sdata
p/x &_edata
```

Check the test variable:

```gdb
p/x &test_data
p/x test_data
```

Expected:

```text
&test_data → 0x200.....
test_data  → 0x12345678
```

This verifies that the initialized `.data` value reached RAM.

### Step 4 — Verify `.bss` Initialization

Set a breakpoint after `.bss` clearing:

```gdb
break bss_done
continue
```

Check:

```gdb
p/x &_sbss
p/x &_ebss
p/x &test_bss
p/x test_bss
```

Expected:

```text
test_bss → 0
```

This verifies that startup cleared `.bss`.

### Step 5 — Verify `.rodata`

Check the addresses:

```gdb
p/x &device_id
p/x &message
```

Expected:

```text
0x080.....
```

They should point into Flash.

Check the values:

```gdb
p/x device_id
x/s message
```

Expected:

```text
device_id = 1234
message   = "Hello STM32"
```

No startup copy is required for `.rodata`.

### Step 6 — Verify `BL main`

Disassemble:

```gdb
disassemble Reset_Handler
```

Locate:

```text
bl main
```

Step through it:

```gdb
stepi
```

Then:

```gdb
info registers pc lr
```

Expected:

```text
PC → main()
LR → return address + Thumb bit
```

### Step 7 — Verify `main()`

```gdb
disassemble main
```

Continue execution:

```gdb
continue
```

The program should remain inside the application loop.

Stop execution:

```gdb
interrupt
```

Check the PC:

```gdb
info registers pc
```

PC should be inside the `main()` loop.


## 4. Debug Checklist

```text
[ ] ELF sections inspected with readelf
[ ] Symbols inspected with nm
[ ] Section layout inspected with objdump
[ ] Firmware image generated with objcopy when required

[ ] OpenOCD detects ST-LINK
[ ] Firmware flashed successfully
[ ] Flash verification passes

[ ] MSP points to RAM
[ ] Vector table verified
[ ] PC reaches Reset_Handler

[ ] .data FLASH source verified
[ ] .data RAM destination verified
[ ] .data value == 0x12345678

[ ] .bss RAM range verified
[ ] .bss variable == 0

[ ] .rodata address is in FLASH
[ ] .rodata values verified

[ ] BL main verified
[ ] PC → main()
[ ] LR → return address + Thumb bit
[ ] main() reaches application loop
```