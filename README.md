```
    888b     d888  .d8888b.   .d8888b.      d8888  888    d8P
    8888b   d8888 d88P  Y88b d88P  Y88b    d8P888  888   d8P
    88888b.d88888 888    888 888          d8P 888  888  d8P
    888Y88888P888 888        888d888b.   d8P  888  888d88K
    888 Y888P 888 888        888P "Y88b d88   888  8888888b
    888  Y8P  888 888    888 888    888 8888888888 888  Y88b
    888   "   888 Y88b  d88P Y88b  d88P       888  888   Y88b
    888       888  "Y8888P"   "Y8888P"        888  888    Y88b

      - 64-bit 680x0-inspired Virtual Machine and Assembler -
```
## About

MC64K aims to implement a Virtual Machine and assembler syntax insired by the legendary 680x0 series of microprocessors from Motorola.

### Motivation
* We want to write 680x0 style assembler for an imaginary 64-bit implementation of the 68000, because:
    - We're old.
    - It's our happy place.
    - Why not?
    - We can.
    - Take your pick.
* We want to assemble that code into some easily interpretable binary format:
    - Rapid execution.
    - Should also be JITable.
* We want to execute that binary in a host application that provides basic IO, graphics and sound services:
    - To amuse ourselves.
    - Any resemblance to genuine usefulness is purely coincidental and unintended.

### Features

* 16 64-bit general purpose registers for integer / address operations:
    - r0 - r15
    - r15 is used as the stack pointer.
    - Byte, Word, Long and Quad operation sizes.
* 16 64-bit IEEE-754 registers for floating point operations.
    - fp0 - fp15
* Set of 680x0 inspired addressing modes:
    - Register Direct.
    - Register Indirect.
    - Register Indirect, Post Increment.
    - Register Indirect, Post Decrement.
    - Register Indirect, Pre Increment.
    - Register Indirect, Pre Decrement.
    - Register Indirect with Displacement.
    - Register Indirect, Indexed.
    - Register Indirect, Indexed with Displacement.
    - Program Counter Indirect with Displacement.
    - Program Counter Indirect, Indexed, with Displacement.
    - Integer Immediate.
    - Absolute Address Immediate.
* Large Code / Memory Model:
    - 64-bit addresses
    - All displacements are 32-bit signed values.
* Simplified compare and branch programming model:
    - e.g. `beq.l r0, (r1), label`
* 680x0 inspired instruction set and syntax:
    - Same mnenomics used for equivalent operations.
    - Same effective address syntax for equivalent modes.
    - 680x0 register names d0 - d7 recognised as aliases for r0 - r7.
    - 680x0 register names a0 - a7 recognised as aliases for r8 - r15.
* All supported effective addressing modes can be used for each operand in an instruction:
    - Destination operand cannot use Program Counter Indirect or Integer Immediate modes.

### Intentional Limitations

While we love the 680x0, there are some things we don't support.

* We aren't interested in hardware emulation or Operating Systems compatibility:
    - No supervisor model or traps.
    - No MMU.
    - No atomic memory operations.
* We aren't interested in unecessary complexity:
    - No binary-level compatibility.
    - No extended precision FPU support.
    - No Condition Code Register:
        - Compare and branch model.
        - No extended arithmetic (addx, etc).
    - No Binary Coded Decimal.
    - No memory indirect addressing modes.
* We aren't interested in your safety. If you use this, the chances are you aren't either:
    - No memory access checks, access any address.
    - No branch target validation, jump to any address.
    - No checks for invalid operations, e.g. zero divide.
    - No checks for infinite loop conditions.
    - You're probably running the interpreter on an OS that will handle these anyway.
