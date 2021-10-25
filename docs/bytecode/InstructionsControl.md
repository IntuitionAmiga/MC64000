## [Documentation](../README.md) > [Bytecode Format](./README.md) > [Instruction Layout](./Instructions.md) > Control Group

The bytecode formats for the suppored control and flow instructions are documented here.

One byte is used for the Instruction Opcode:

* Opcodes without any operands are 1 byte.
* Opcodes that have operands place them in the following order:
    - Destination [Effective Address](EffectiveAddress.md).
    - Source [Effective Address](EffectiveAddress.md).
    - Branch displacements.
    - Other data.

* ### [HCF](./op/c_01.md)
    - Halt and Call Function / Catch Fire.
* ### [BRA](./op/c_02.md)
    - Branch.
* ### [BSR](./op/c_03.md)
    - Branch to Subroutine.
* ### [JMP](./op/c_04.md)
    - Jump.
* ### [JSR](./op/c_05.md)
    - Jump to Subroutine.
* ### [RTS](./op/c_06.md)
    - Return from Subroutine.
* ### [BIZ](./op/c_07.md)
    - Branch if operand is zero.
* ### [BNZ](./op/c_08.md)
    - Branch if operand is not zero.
* ### [BMI](./op/c_09.md)
    - Branch if operand is minus.
* ### [BPL](./op/c_10.md)
    - Branch if operand is plus.
* ### [BLT](./op/c_11.md)
    - Branch if source operand is less than destination operand.
* ### [BLE](./op/c_12.md)
    - Branch if source operand is less than or equal to destination operand.
* ### [BEQ](./op/c_13.md)
    - Branch if source operand is equal to destination operand.
* ### [BGE](./op/c_14.md)
    - Branch if source operand is equal to or greater than destination operand.
* ### [BGT](./op/c_15.md)
    - Branch if source operand is greater than destination operand.
* ### [BNE](./op/c_16.md)
    - Branch if source operand is not equal to destination operand.
* ### [BBS](./op/c_17.md)
    - Branch if bit is set.
* ### [BBC](./op/c_18.md)
    - Branch if bit is clear.
* ### [DBNZ](./op/c_19.md)
    - Decrement counter and branch if not zero.
