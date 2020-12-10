## Assembler

This subdirectory contains PHP7.4 sources for the MC64000 bytecode assembler (WIP)

### Register Names

The MC64000 proposes 16 64-bit general purpose registers (TBC). These registers can be used in both integer and address mode contexts and support all addressing modes:

- r0, r1, r2 ... r15
- - r15 is used as the the stack pointer.

To assist migration of assembly sources from existing 680x0 projects, the following aliases are also permitted:

- d0, d1, d2 ... d7
- - These are aliases of the general purpose r0, r1, r2 ... r7.
- - These names are permitted as register direct operands and as indexes.
- - These names are not permitted as register indirect operands, e.g. `(d0)` is not considered valid syntax, whereas `(r0)` is.

- a0, a1, a2 ... a7
- - These are aliases of the general purpose registers r8, r9, r10 ... r15
- - These names are permitted as register indirect operands.
- - The convention ensures that a7 is regarded as the stack pointer.

The specification further proposes 16 floating point registers (TBC). These registers are only available to floating point operations:

- fp0, fp1, fp2 ... fp15
- - Only supports IEEE754 64-bit precision
- - Register to Register operations will always be 64 bit.


### Operand Sizes

The following operand size identifiers are used:

- b (8-bit integer)
- w (16-bit integer)
- l (32-bit integer)
- q (64-bit integer)
- s (32-bit float)
- d (64-bit float)

### Addressing Modes

The following subset of 68020 style addressing modes are available for most instruction operands (examples of each are non-exhaustive):

- Integer Literal:
- - \#0
- - \#-128
- - \#127
- - \#+15
- - \#0x12AF
- - \#0xfffffffe
- Register Direct:
- - r0
- - d1
- - a2
- - fp3
- Register Indirect:
- - (r1)
- - (a0)
- Register Indirect Post Increment:
- - (r2)+
- - (a1)+
- Register Indirect Post Decrement:
- - (r3)-
- - (a2)-
- Register Indirect Pre  Increment:
- - +(r4)
- - +(a3)
- Register Indirect Pre  Decrement:
- - -(r5)
- - -(a4)
- Register Indirect with Signed Displacement:
- - Older 680x0 style:
- - - 16(r0)
- - - -32(r1)
- - - +12(a0)
- - - 32(pc)
- - - -128(pc)
- - 0xF0(a1)
- - Newer 680x0 style:
- - - (32, r7)
- - - (-16, a2)
- - - (0xc0, r3)
- - - (256, pc)
- Register Indirect with Index
- - (r0, r1.b)
- - (a0, d1.w)
- - (r1, r3.l)
- - (r1, r4.q)
- - (pc, r0.l)
- Register Indirect with Index and Signed Displacement:
- - Older 680x0 style:
- - - 32(r0, d1.b)
- - - -64(a1, d0.w)
- - - 0xFFFE(r2, r3.l)
- - Newer 680x0 style:
- - - -128(pc, r0.q)
- - - (-32, r7, r0.w)
- - - (0xfc, r0, r1.l)
- Register Indirect with Scaled Index:
- - (a0, d0.w * 2)
- - (r7, r0.l * 4)
- - (pc, r1.q * 8)
- Register Indirect with Scaled Index and Signed Displacement:
- - Older 680x0 style:
- - - 32(r0, d1.b * 2)
- - - -64(a1, d0.w * 4)
- - - 0xFFFE(r2, r3.l * 8)
- - Newer 680x0 style:
- - - (-128, pc, r0.q * 2)
- - - (-32, r7, r0.w * 4)
- - - (0xfc, r0, r1.l * 8)
