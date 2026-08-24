# Reset and Startup Sequence of ARM Cortex-M

## Mental Model of Reset Flow

```text
                         MCU RESET
                             │
                             ▼
                    CPU loads first two
                     32-bit words from
                    ┌──────────────────┐
                    │   Vector Table   │
                    │    0x08000000    │
                    └────────┬─────────┘
                             │
                 ┌───────────┴───────────┐
                 ▼                       ▼
           Initial MSP           Address of Reset_Handler
           → top of RAM                  │
                                         ▼
                               ┌──────────────────┐
                               │   Startup Code   │
                               └────────┬─────────┘
                                        │
                         ┌──────────────┴──────────────┐
                         ▼                             ▼
                .data initialization          .bss initialization
                FLASH ──────► RAM              RAM ──────► 0
                         │                             │
                         └──────────────┬──────────────┘
                                        ▼
                                     BL main
                                        │
                         ┌──────────────┴──────────────┐
                         │                             │
                         ▼                             ▼
                        PC                            LR
                    → address of main()        ← return address 
                         |                   (after BL main instruction) 
                         |                         + Thumb bit
                         │
                         ▼
                  Application Code
                         │
                         ▼
                  ┌───────────────┐
                  │   while (1)   │
                  │  loop forever │
                  └───────────────┘
```

The CPU does not start directly in `main()`.

On reset:

1. The CPU loads the **initial MSP** from the first vector-table entry.
2. The CPU loads the **Reset_Handler address** from the second vector-table entry.
3. Execution begins at `Reset_Handler`.
4. `Reset_Handler` initializes the C runtime:
   - `.data`: Flash → RAM
   - `.bss`: RAM → 0
5. `Reset_Handler` executes `BL main`.
6. `BL` sets:
   - `PC` → address of `main`
   - `LR` → return address after `BL main`, with the Thumb-state bit set.
7. `main()` executes the application code.
8. Because `main()` does not return, execution remains in the application loop.


## 1. Vector Table

The linker places `.isr_vector` first in Flash at `0x08000000`.

The first two 32-bit entries are:

```text
Word 0 → Initial Main Stack Pointer (MSP)
Word 1 → Reset_Handler address
```

The CPU loads these values after reset and begins execution at `Reset_Handler`.

## 2. Reset_Handler

`Reset_Handler` is the first startup code executed after reset.

It prepares the C runtime before calling `main()`:

- Copies initialized `.data` values from Flash to RAM.
- Clears the `.bss` section in RAM.
- Calls `main()` with `BL main`.
- If `main()` returns, stays in an infinite loop.

## 3. `.data` Initialization

`.data` contains initialized writable global/static variables.

```text
Initial values: FLASH
Runtime values : RAM
```

Startup copies:

```text
_sidata → _sdata ... _edata
FLASH       →       RAM
```

## 4. `.bss` Initialization

`.bss` contains writable variables without initial values.

It has no Flash load image because it is `NOLOAD`.

Before `main()`:

```text
_sbss ... _ebss → 0
```

Startup writes zero to the entire `.bss` RAM range.

## 5. Entering `main()`

After `.data` and `.bss` initialization:

```text
bl main
```

`BL` transfers execution to `main()` and stores the return address in `LR`.

`main()` then runs the application code.

## Memory Roles

```text
FLASH 0x08000000
├── .isr_vector
├── .text
├── .rodata
└── .data initial image

RAM 0x20000000
├── .data runtime
├── .bss
└── stack
```

**Key idea:** Flash provides the firmware and initial data; startup creates the required RAM state before `main()`.
