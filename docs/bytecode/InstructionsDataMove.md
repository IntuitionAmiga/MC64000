## [Documentation](../README.md) > [Bytecode Format](./README.md) > [Instruction Layout](./Instructions.md) > Data Movement Group

The bytecode formats for the suppored data movement and conversion instructions are documented here.

One byte is used for the Instruction Opcode:

* Opcodes without any operands are 1 byte.
* Opcodes that have operands place them in the following order:
    - Destination [Effective Address](EffectiveAddress.md).
    - Source [Effective Address](EffectiveAddress.md).
    - Branch displacements.
    - Other data.

* ### [MOVE](./op/d_01.md)
    - Move Data.
* ### [SAVEM](./op/d_02.md)
    - Save Multiple Registers.
* ### [LOADM](./op/d_03.md)
    - Load Multiple Registers.
* ### [FMOVEB](./op/d_04.md)
    - Convert signed byte to floating point.
* ### [FMOVEW](./op/d_05.md)
    - Convert signed word to floating point.
* ### [FMOVEL](./op/d_06.md)
    - Convert signed long to floating point.
* ### [FMOVEQ](./op/d_07.md)
    - Convert signed long to floating point.
* ### [FMOVES](./op/d_08.md)
    - Convert from single precision floating point.
* ### [FMOVEQ](./op/d_09.md)
    - Convert from double precision floating point.
* ### [MOVE](./op/d_10.md)
    - Move Floating Point Data.
* ### [FINFO](./op/d_11.md)
    - Floating point data information.
* ### [CLR](./op/d_12.md)
    - Clear a location.
* ### [EXG](./op/d_13.md)
    - Exchange GPR.
* ### [FEXG](./op/d_14.md)
    - Exchange FPR.
* ### [SWAP](./op/d_15.md)
    - Swap GPR fragments.
* ### [LINK](./op/d_16.md)
    - Link and allocate.
* ### [UNLK](./op/d_17.md)
    - Unlink.
* ### [LEA](./op/d_18.md)
    - Load Effective Address.
* ### [PEA](./op/d_19.md)
    - Push Effective Address.
* ### [SIZ/FSIZ](./op/d_20.md)
    - Set if operand is zero.
* ### [SNZ/FSNZ](./op/d_21.md)
    - Set if operand is not zero.
* ### [SMI/FSMI](./op/d_22.md)
    - Set if operand is minus.
* ### [SPL/FSPL](./op/d_23.md)
    - Set if operand is plus.
* ### [SLT/SLO/FSLT](./op/d_24.md)
    - Set if source operand is less than destination operand.
* ### [SLE/SLS/FSLT](./op/d_25.md)
    - Set if source operand is less than or equal to destination operand.
* ### [SEQ/FSEQ](./op/d_26.md)
    - Set if source operand is equal to destination operand.
* ### [SGE/SHS/FSGE](./op/d_27.md)
    - Set if source operand is equal to or greater than destination operand.
* ### [SGT/SHI/FSGT](./op/d_28.md)
    - Set if source operand is greater than destination operand.
* ### [SNE/FSNE](./op/d_29.md)
    - Set if source operand is not equal to destination operand.
* ### [SBS](./op/d_30.md)
    - Set if bit is set.
* ### [SBC](./op/d_31.md)
    - Set if bit is clear.
