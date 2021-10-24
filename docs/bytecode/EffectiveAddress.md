## [Documentation](../README.md) > [Bytecode Format](./README.md) > Effective Address

The bytecode formats for the suppored addressing modes are documented here.

One byte is used for the Effective Address Mode of an operand:

* The simplest addressing modes do not require any additional bytes.
* Fixed 32-bit displacements require an additional 4 bytes to represent the displacement value.
* Indexed addressing modes encode the index size and scale in the EA byte and the register pair in one additional byte.
* Integer Immediates require as many additional bytes as are required by the operation size.
* Absolute Address Immediates require an additional 2, 4 or 8 bytes, depending on the word size.
* In the following examples, extension byte values in italics refer to elements of the Effective address, e.g:
    - 0x*DD* refers to a byte that forms part of a displacement value.
___

* ### [General Purpose Register Direct](./ea/p_01.md)
    - The contents of the register are used as the operand data.
* ### [General Purpose Register Indirect](./ea/p_02.md)
    - The contents of the register are used as the address of the operand data in memory.
* ### [General Purpose Register Indirect, Post Increment](./ea/p_03.md)
    - The contents of the register are used as the address of the operand data in memory. The contents of the register are then incremented by the operation size.
* ### [General Purpose Register Indirect, Post Decrement](./ea/p_04.md)
    - The contents of the register are used as the address of the operand data in memory. The contents of the register are then decremented by the operation size.
* ### [General Purpose Register Indirect, Pre Increment](./ea/p_05.md)
    - The contents of the register are incremented by the operation size. The contents of the register are then used as the address of the operand data in memory.
* ### [General Purpose Register Indirect, Pre Decrement](./ea/p_06.md)
    - The contents of the register are decremented by the operation size. The contents of the register are then used as the address of the operand data in memory.
* ### [General Purpose Register Indirect with Displacement](./ea/p_07.md)
    - The contents of the register, plus the signed 32-bit displacement are used as the address of the operand data in memory.
* ### [Floating Point Register Direct](./ea/p_08.md)
    - The contents of the register are used as the operand data. This mode is only available for floating point operations.
* ### [General Purpose Register Indirect with (Scaled) Index](./ea/p_09.md)
    - The contents of the register, plus an optionally scaled index value taken from a second register are used as the address of the operand in memory. The index size and scale factor are selectable.
* ### [General Purpose Register Indirect with (Scaled) Index and Displacement](./ea/p_10.md)
    - The contents of the register, plus an optionally scaled index value taken from a second register, plus the signed 32-bit displacement are used as the address of the operand in memory. The index size and scale factor are selectable.
* ### [Integer Immediate](./ea/p_11.md)
    - A signed immediate integer value encoded into the instruction stream is used.
* ### [Float Immediate](./ea/p_12.md)
    - A floating point value encoded into the instruction stream is used.
* ### [Program Indirect Counter with Displacement](./ea/p_13.md)
    - The contents of the program counter, plus the signed 32-bit displacement are used as the address of the operand data in memory.
* ### [Same As Destination](./ea/p_14.md)
    - The fully evaluated destination operand address is used for the source operand in memory.
* ### [Import Index](./ea/p_15.md)
    - Enumerated index for imported symbol reference.
* ### [Program Counter Indirect with (Scaled) Index (currently unsupported)](./ea/p_16.md)
    - The contents of the program counter, plus an optionally scaled index value taken from a second register are used as the address of the operand in memory. The index size and scale factor are selectable.
* ### [Program Counter Indirect with (Scaled) Index and Displacement (currently unsupported)](./ea/p_17.md)
    - The contents of the program counter, plus an optionally scaled index value taken from a second register, plus the signed 32-bit displacement are used as the address of the operand in memory. The index size and scale factor are selectable.
