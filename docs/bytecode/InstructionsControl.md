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
    - Halt and Call Function.
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
* ### [BIZ/FBIZ](./op/c_07.md)
    - Branch if operand is zero.
* ### [BNZ/FBNZ](./op/c_08.md)
    - Branch if operand is not zero.
* ### [BMI/FBMI](./op/c_09.md)
    - Branch if operand is minus.
* ### [BPL/FBPL](./op/c_10.md)
    - Branch if operand is plus.
* ### [BLT/BLO/FBLT](./op/c_11.md)
    - Branch if source operand is less than destination operand.
* ### [BLE/BLS/FBLE](./op/c_12.md)
    - Branch if source operand is less than or equal to destination operand.
* ### [BEQ/FBEQ](./op/c_13.md)
    - Branch if source operand is equal to destination operand.
* ### [BGE/BHS/FBGE](./op/c_14.md)
    - Branch if source operand is equal to or greater than destination operand.
* ### [BGT/BHI/FBGT](./op/c_15.md)
    - Branch if source operand is greater than destination operand.
* ### [BNE/FBNE](./op/c_16.md)
    - Branch if source operand is not equal to destination operand.
* ### [BBS](./op/c_17.md)
    - Branch if bit is set.
* ### [BBC](./op/c_18.md)
    - Branch if bit is clear.
* ### [DBNZ](./op/c_19.md)
    - Decrement counter and branch if not zero.
