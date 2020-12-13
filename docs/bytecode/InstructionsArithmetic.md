## Bytecode Format > [Instruction Layout](Instructions.md) > Arithmetic Group

The bytecode formats for the suppored arithmetic instructions are documented here.

One byte is used for the Instruction Opcode:

* Opcodes without any operands are 1 byte.
* Opcodes that have operands place them in the following order:
    - Destination [Effective Address](EffectiveAddress.md).
    - Source [Effective Address](EffectiveAddress.md).
    - Branch displacements.
    - Other data.

