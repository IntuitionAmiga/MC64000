## [Documentation](../README.md) > [Bytecode Format](./README.md) > [Instruction Layout](./Instructions.md) > Arithmetic Group

The bytecode formats for the suppored arithmetic instructions are documented here.

One byte is used for the Instruction Opcode:

* Opcodes without any operands are 1 byte.
* Opcodes that have operands place them in the following order:
    - Destination [Effective Address](EffectiveAddress.md).
    - Source [Effective Address](EffectiveAddress.md).
    - Branch displacements.
    - Other data.

* ### [EXTB](./op/a_01.md)
    - Sign Extend Byte.
* ### [EXTW](./op/a_02.md)
    - Sign Extend Word.
* ### [EXTL](./op/a_03.md)
    - Sign Extend Long.
* ### [ASL](./op/a_04.md)
    - Arithmetic Shift Left.
* ### [ASR](./op/a_05.md)
    - Arithmetic Shift Right.
* ### [ADD](./op/a_06.md)
    - Add source to destination.
* ### [SUB](./op/a_07.md)
    - Subtract source from destination.
* ### [NEG](./op/a_08.md)
    - Negate Source to Destination.
* ### [MULS](./op/a_09.md)
    - Integer Signed Multiply.
* ### [MULU](./op/a_10.md)
    - Integer Unsigned Multiply.
* ### [FMUL](./op/a_11.md)
    - Floating Point Multiply.
* ### [DIVS](./op/a_12.md)
    - Integer Signed Divide.
* ### [MODS](./op/a_13.md)
    - Integer Signed Modulus.
* ### [DIVU](./op/a_14.md)
    - Integer Unsigned Divide.
* ### [MODU](./op/a_15.md)
    - Integer Unsigned Modulus.
* ### [FDIV](./op/a_16.md)
    - Floating Point Divide.
* ### [FMOD](./op/a_17.md)
    - Floating Point Modulus.
* ### [FABS](./op/a_18.md)
    - Floating Point Absolute.
* ### [FSQRT](./op/a_19.md)
    - Floating Point Square Root.
* ### [FACOS](./op/a_20.md)
    - Floating Point Arccosine.
* ### [FASIN](./op/a_21.md)
    - Floating Point Arcsine.
* ### [FATAN](./op/a_22.md)
    - Floating Point Arctangent.
* ### [FCOS](./op/a_23.md)
    - Floating Point Cosine.
* ### [FSIN](./op/a_24.md)
    - Floating Point Sine.
* ### [FSINCOS](./op/a_25.md)
    - Floating Point Sine and Cosine.
* ### [FTAN](./op/a_26.md)
    - Floating Point Tangent.
* ### [FETOX](./op/a_27.md)
    - Floating Point e to x.
* ### [FLOGN](./op/a_28.md)
    - Floating Point Natural Logarithm.
* ### [FLOG2](./op/a_29.md)
    - Floating Point Base-2 Logarithm.
* ### [FTWOTOX](./op/a_30.md)
    - Floating Point 2 to x
