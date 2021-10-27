## [Documentation](../README.md) > [Bytecode Format](./README.md) > [Instruction Layout](./Instructions.md) > Logical Group

The bytecode formats for the suppored bitwise logical instructions are documented here.

One byte is used for the Instruction Opcode:

* Opcodes without any operands are 1 byte.
* Opcodes that have operands place them in the following order:
    - Destination [Effective Address](EffectiveAddress.md).
    - Source [Effective Address](EffectiveAddress.md).
    - Branch displacements.
    - Other data.

* ### [AND](./op/l_01.md)
    - Bitwise AND.
* ### [OR](./op/l_02.md)
    - Bitwise OR.
* ### [EOR](./op/l_03.md)
    - Bitwise Exclusive OR.
* ### [NOT](./op/l_04.md)
    - Bitwise Complement.
* ### [LSL](./op/l_05.md)
    - Logical Shift Left.
* ### [LSR](./op/l_06.md)
    - Logical Shift Right.
* ### [ROL](./op/l_07.md)
    - Rotate Left.
* ### [ROR](./op/l_08.md)
    - Rotate Right.
* ### [BCLR](./op/l_09.md)
    - Clear a bit in the destination.
* ### [BSET](./op/l_10.md)
    - Set a bit in the destination.
* ### [BFFFO](./op/l_11.md)
    - Find First One in Bitfield.
* ### [BFCNT](./op/l_12.md)
    - Count number of set bits.

