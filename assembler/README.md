## Assembler

This subdirectory contains PHP7.4 sources for the MC64000 bytecode assembler (WIP)

## Programming Model

## Syntax Overview

### Register Names

The MC64000 proposes 16 64-bit general purpose registers (TBC). These registers can be used in both integer and address mode contexts and support all addressing modes:

* r0, r1, r2 ... r15
    - r15 is used as the the stack pointer.

To assist migration of assembly sources from existing 680x0 projects, the following aliases are also permitted:

* d0, d1, d2 ... d7
    - These are aliases of the general purpose r0, r1, r2 ... r7.
    - These names are permitted as register direct operands and as indexes.
    - These names are not permitted as register indirect operands, e.g. `(d0)` is not considered valid syntax, whereas `(r0)` is legal despite being the same register.
    - This is to preserve the semantic meaning of the original code where it is not legal to use a data register as the base register for an address operation.

* a0, a1, a2 ... a7
    - These are aliases of the general purpose registers r8, r9, r10 ... r15
    - These names are permitted as register indirect operands.
    - The convention ensures that a7 is regarded as the stack pointer.

The specification further proposes 16 floating point registers (TBC). These registers are only available to floating point operations:

* fp0, fp1, fp2 ... fp15
    - Only supports IEEE754 64-bit precision
    - Register to Register operations will always be 64 bit.


### Operand Sizes

The following operand size identifiers are used as prefixes for mnemonics and effective address index registers:

* b (8-bit integer)
* w (16-bit integer)
* l (32-bit integer)
* q (64-bit integer)
* s (32-bit float)
* d (64-bit float)

### Addressing Modes

The following subset of 68020 inspired addressing modes are supported for MC64000.

#### Integer Immediate

An integer value is embedded in the bytecode stream.

* _Valid for source operands only._
* _Signed decimal and unsigned hexadecimal representations supported_.
* _Operation size determines valid range for integer and number of bytes embedded._
 
Examples:

* \#0
* \#-128
* \#127
* \#+15
* \#0x12AF
* \#0xfffffffe



#### Register Direct

The contents of a register are accessed directly.

* _For operations on integers, the operation size deterines which bits of the register are affected._
* _For operations on floats, all 64-bits of the register are affected regardless of the operation size._

Examples:

* r0
* d1
* a2
* fp3


#### Register Indirect

The contents of an integer register are used as the address of an operand.

* _All bits of the register are used to define the address._
* _Not allowed for aliases d0 - d7 or pc._

Examples:

* (r1)
* (a0)


#### Register Indirect Post Increment

The contents of an integer register are used as the address of an operand. The address is incremented byte the operation size after dereferencing.

* _All bits of the register are used to define the address._
* _Not allowed for aliases d0 - d7 or pc._

Examples:

* (r2)+
* (a1)+

#### Register Indirect Post Decrement

The contents of an integer register are used as the address of an operand. The address is decremented by the operation size after dereferencing.

* _All bits of the register are used to define the address._
* _Not defined for 680x0._
* _Not allowed for aliases d0 - d7 or pc._

Examples:

* (r3)-
* (a2)-


#### Register Indirect Pre Increment

The contents of an integer register are used as the address of an operand. The address is incremented by the operation size before dereferencing.

* _All bits of the register are used to define the address._
* _Not allowed for aliases d0 - d7 or pc._

Examples:

* +(r4)
* +(a3)

#### Register Indirect Pre Decrement

The contents of an integer register are used as the address of an operand. The address is decremented by the operation size before dereferencing.

* _All bits of the register are used to define the address._
* _Not defined for 680x0._
* _Not allowed for aliases d0 - d7 or pc._

Examples:

* -(r5)
* -(a4)

#### Register Indirect with Signed 32-bit Displacement

The contents of an integer register are used as the address of an operand. The address is offset by the displacement before dereferencing.

* _All bits of the register are used to define the address._
* _Not allowed for aliases d0 - d7._
* _Indirection based on Program Counter is valid for source operands only._

Examples:

* Older 680x0 style:
    - 16(r0)
    - -32(r1)
    - +12(a0)
    - 32(pc)
    - -128(pc)
    - 0xF0(a1)
* Newer 680x0 style:
    - (32, r7)
    - (-16, a2)
    - (0xc0, r3)
    - (256, pc)


#### Register Indirect with Index

The contents of an integer register are used as the address of an operand. The address is offset by an index stored in another register before dereferencing.

* _All bits of the register are used to define the address._
* _Only the lower order bits of the index register are used for index sizes less than 64-bit._
* _Not allowed for aliases d0 - d7._
* _Indirection based on Program Counter is valid for source operands only._

Examples:

* (r0, r1.b)
* (a0, d1.w)
* (r1, r3.l)
* (r1, r4.q)
* (pc, r0.l)


#### Register Indirect with Index and Signed 32-bit Displacement

The contents of an integer register are used as the address of an operand. The address is first offset by the displacement then further offset by an index stored in another register before dereferencing.

* _All bits of the register are used to define the address._
* _Only the lower order bits of the index register are used for index sizes less than 64-bit._
* _Not allowed for aliases d0 - d7._
* _Indirection based on Program Counter is valid for source operands only._

Examples:

* Older 680x0 style:
    - 32(r0, d1.b)
    - -64(a1, d0.w)
    - 0xFFFE(r2, r3.l)
* Newer 680x0 style:
    - -128(pc, r0.q)
    - (-32, r7, r0.w)
    - (0xfc, r0, r1.l)


#### Register Indirect with 16/32/64-bit Scaled Index

The contents of an integer register are used as the address of an operand. The address is offset by an index stored in another register before dereferencing. The index value is premultiplied by the scale factor.

* _All bits of the register are used to define the address._
* _Only the lower order bits of the index register are used for index sizes less than 64-bit._
* _Scale factor must be 2, 4, or 8._
* _Not allowed for aliases d0 - d7._
* _Indirection based on Program Counter is valid for source operands only._

Examples:

* (a0, d0.w * 2)
* (r7, r0.l * 4)
* (pc, r1.q * 8)


#### Register Indirect with 16/32/64-bit Scaled Index and Signed 32-bit Displacement

The contents of an integer register are used as the address of an operand. The address is first offset by the displacement then further offset by an index stored in another register before dereferencing. The index value is premultiplied by the scale factor.

* _All bits of the register are used to define the address._
* _Only the lower order bits of the index register are used for index sizes less than 64-bit._
* _Scale factor must be 2, 4, or 8._
* _Not allowed for aliases d0 - d7._
* _Indirection based on Program Counter is valid for source operands only._

Examples:

* Older 680x0 style:
    - 32(r0, d1.b * 2)
    - -64(a1, d0.w * 4)
    - 0xFFFE(r2, r3.l * 8)
* Newer 680x0 style:
    - (-128, pc, r0.q * 2)
    - (-32, r7, r0.w * 4)
    - (0xfc, r0, r1.l * 8)

#### Absolute Address

An unsigned integer value is embedded in the bytecode stream. This is interpreted as an absolute memory location.

* _Included for completeness._
* _Size defines the number of bytes used to encode the address._
* _Address is expanded to 64-bits_

Examples:

* (4096).w
* (0x0001beef).l
* (8589934592).q
