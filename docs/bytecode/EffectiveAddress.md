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

### General Purpose Register Direct

The contents of the register are used as the operand data.

General syntax: `r<N>`

* Allowed register names: r0 ... r15, d0 ... d7, a0 ... a7, sp
* Operation size determines which bits of the register are used.

| Mode | Bytecode |
| - | - |
| `r0` | 0x00 |
| `r1` | 0x01 |
| ... | ... |
| `r15` | 0x0F |

___

### General Purpose Register Indirect

The contents of the register are used as the address of the operand data in memory.

General syntax: `(r<N>)`

* Allowed register names: r0 ... r15, a0 ... a7, sp
* All bits of the register are used.

| Mode | Bytecode |
| - | - |
| `(r0)` | 0x10 |
| `(r1)` | 0x11 |
| ... | ... |
| `(r15)` | 0x1F |

___

### General Purpose Register Indirect, Post Increment

The contents of the register are used as the address of the operand data in memory. The contents of the register are then incremented by the operation size.

General syntax: `(r<N>)+`

* Allowed register names: r0 ... r15, a0 ... a7, sp
* All bits of the register are used.

| Mode | Bytecode |
| - | - |
| `(r0)+` | 0x20 |
| `(r1)+` | 0x21 |
| ... | ... |
| `(r15)+` | 0x2F |

___

### General Purpose Register Indirect, Post Decrement

The contents of the register are used as the address of the operand data in memory. The contents of the register are then decremented by the operation size.

General syntax: `(r<N>)-`

* Allowed register names: r0 ... r15, a0 ... a7, sp
* All bits of the register are used.

| Mode | Bytecode |
| - | - |
| `(r0)-` | 0x30 |
| `(r1)-` | 0x31 |
| ... | ... |
| `(r15)-` | 0x3F |

___

### General Purpose Register Indirect, Pre Increment

The contents of the register are incremented by the operation size. The contents of the register are then used as the address of the operand data in memory.

General syntax: `+(r<N>)`

* Allowed register names: r0 ... r15, a0 ... a7, sp
* All bits of the register are used.

| Mode | Bytecode |
| - | - |
| `+(r0)` | 0x40 |
| `+(r1)` | 0x41 |
| ... | ... |
| `+(r15)` | 0x4F |

___

### General Purpose Register Indirect, Pre Decrement

The contents of the register are decremented by the operation size. The contents of the register are then used as the address of the operand data in memory.

General syntax: `-(r<N>)`

* Allowed register names: r0 ... r15, a0 ... a7, sp
* All bits of the register are used.

| Mode | Bytecode |
| - | - |
| `-(r0)` | 0x50 |
| `-(r1)` | 0x51 |
| ... | ... |
| `-(r15)` | 0x5F |

___

### General Purpose Register Indirect with Displacement

The contents of the register, plus the signed 32-bit displacement are used as the address of the operand data in memory.

General syntax: `<D>(r<N>)` or `(<D>, r<N>)`

Examples:

        16(r0)
        (16, a0)
        -10(sp)
        (-10, a1)
        0xFF00(r2)
        (0xFC00, a2)

* Allowed register names: r0 ... r15, a0 ... a7, sp
* All bits of the register are used.
* D = -2147483648 ... 2147483647

| Mode | Bytecode | Ext 0 | Ext 1  | Ext 2 | Ext 3 |
| - | - | - | - | - | - |
| `(r0)` | 0x60 | 0x*DD* | 0x*DD* | 0x*DD* | 0x*DD* |
| `(r1)` | 0x61 | 0x*DD* | 0x*DD* | 0x*DD* | 0x*DD* |
| ... | ... | ... | ... | ... | ... |
| `(r15)` | 0x6F | 0x*DD* | 0x*DD* | 0x*DD* | 0x*DD* |

___

### Floating Point Register Direct

The contents of the register are used as the operand data. This mode is only available for floating point operations.

General syntax: `fp<N>`

* Allowed register names: fp0 ... fp15

| Mode | Bytecode |
| - | - |
| `fp0` | 0x70 |
| `fp1` | 0x71 |
| ... | ... |
| `fp15` | 0x7F |

___

### General Purpose Register Indirect with (Scaled) Index

The contents of the register, plus an optionally scaled index value taken from a second register are used as the address of the operand in memory. The index size and scale factor are selectable.

General syntax: `(r<A>, r<I>.<b|w|l|q> [* <2|4|8>])`

Examples:

        (r0, r1.b)
        (a0, d1.q * 2)
        (sp, d5.l* 8)
        (r0, a0.q * 4)

* Allowed base register names: r0 ... r15, a0 ... a7, sp
* Allowed index register names: r0 ... r15, d0 ... d7, a0 ... a7, sp
* All bits of the base register are used.
* For .b, .w and .l sized indexes, the register fragment is treated as a signed value:
    - For a .b index, a register value of 0x00000000000000FF is interpreted as -1, not 255.

| Mode | Bytecode | Ext 0 |
| - | - | - |
| `(r<A>, r<I>.b)` | 0x80 | 0x*AI* |
| `(r<A>, r<I>.w)` | 0x81 | 0x*AI* |
| `(r<A>, r<I>.l)` | 0x82 | 0x*AI* |
| `(r<A>, r<I>.q)` | 0x83 | 0x*AI* |
| `(r<A>, r<I>.b * 2)` | 0x84 | 0x*AI* |
| `(r<A>, r<I>.w * 2)` | 0x85 | 0x*AI* |
| `(r<A>, r<I>.l * 2)` | 0x86 | 0x*AI* |
| `(r<A>, r<I>.q * 2)` | 0x87 | 0x*AI* |
| `(r<A>, r<I>.b * 4)` | 0x88 | 0x*AI* |
| `(r<A>, r<I>.w * 4)` | 0x89 | 0x*AI* |
| `(r<A>, r<I>.l * 4)` | 0x8A | 0x*AI* |
| `(r<A>, r<I>.q * 4)` | 0x8B | 0x*AI* |
| `(r<A>, r<I>.b * 8)` | 0x8C | 0x*AI* |
| `(r<A>, r<I>.w * 8)` | 0x8D | 0x*AI* |
| `(r<A>, r<I>.l * 8)` | 0x8E | 0x*AI* |
| `(r<A>, r<I>.q * 8)` | 0x8F | 0x*AI* |

___

### General Purpose Register Indirect with (Scaled) Index and Displacement

The contents of the register, plus an optionally scaled index value taken from a second register, plus the signed 32-bit displacement are used as the address of the operand in memory. The index size and scale factor are selectable.

General syntax: `<D>(r<A>, r<I>.<b|w|l|q> [ * <2|4|8>])` or `(<D>, r<A>, r<I>.<b|w|l|q> [ * <2|4|8>])`

Examples:

        10(r0, r1.w)
        -5(a0, d1.l * 2)
        0xFFFE(sp, r5.b * 8)
        (-20, r0, a0.l * 4)

* Allowed base register names: r0 ... r15, a0 ... a7, sp
* Allowed index register names: r0 ... r15, d0 ... d7, a0 ... a7, sp
* All bits of the base register are used.
* For .b, .w and .l sized indexes, the register fragment is treated as a signed value:
    - For a .b index, a register value of 0x00000000000000FF is interpreted as -1, not 255.
* D = -2147483648 ... 2147483647

| Mode | Bytecode | Ext 0 | Ext 1  | Ext 2 | Ext 3 | Ext 4 |
| - | - | - | - | - | - | - |
| `<D>(r<A>, r<I>.b)` | 0x90 | 0x*AI* | 0x*DD* | 0x*DD* | 0x*DD* | 0x*DD* |
| `<D>(r<A>, r<I>.w)` | 0x91 | 0x*AI* | 0x*DD* | 0x*DD* | 0x*DD* | 0x*DD* |
| `<D>(r<A>, r<I>.l)` | 0x92 | 0x*AI* | 0x*DD* | 0x*DD* | 0x*DD* | 0x*DD* |
| `<D>(r<A>, r<I>.q)` | 0x93 | 0x*AI* | 0x*DD* | 0x*DD* | 0x*DD* | 0x*DD* |
| `<D>(r<A>, r<I>.b * 2)` | 0x94 | 0x*AI* | 0x*DD* | 0x*DD* | 0x*DD* | 0x*DD* |
| `<D>(r<A>, r<I>.w * 2)` | 0x95 | 0x*AI* | 0x*DD* | 0x*DD* | 0x*DD* | 0x*DD* |
| `<D>(r<A>, r<I>.l * 2)` | 0x96 | 0x*AI* | 0x*DD* | 0x*DD* | 0x*DD* | 0x*DD* |
| `<D>(r<A>, r<I>.q * 2)` | 0x97 | 0x*AI* | 0x*DD* | 0x*DD* | 0x*DD* | 0x*DD* |
| `<D>(r<A>, r<I>.b * 4)` | 0x98 | 0x*AI* | 0x*DD* | 0x*DD* | 0x*DD* | 0x*DD* |
| `<D>(r<A>, r<I>.w * 4)` | 0x99 | 0x*AI* | 0x*DD* | 0x*DD* | 0x*DD* | 0x*DD* |
| `<D>(r<A>, r<I>.l * 4)` | 0x9A | 0x*AI* | 0x*DD* | 0x*DD* | 0x*DD* | 0x*DD* |
| `<D>(r<A>, r<I>.q * 4)` | 0x9B | 0x*AI* | 0x*DD* | 0x*DD* | 0x*DD* | 0x*DD* |
| `<D>(r<A>, r<I>.b * 8)` | 0x9C | 0x*AI* | 0x*DD* | 0x*DD* | 0x*DD* | 0x*DD* |
| `<D>(r<A>, r<I>.w * 8)` | 0x9D | 0x*AI* | 0x*DD* | 0x*DD* | 0x*DD* | 0x*DD* |
| `<D>(r<A>, r<I>.l * 8)` | 0x9E | 0x*AI* | 0x*DD* | 0x*DD* | 0x*DD* | 0x*DD* |
| `<D>(r<A>, r<I>.q * 8)` | 0x9F | 0x*AI* | 0x*DD* | 0x*DD* | 0x*DD* | 0x*DD* |
___

### Program Counter Indirect with (Scaled) Index

The contents of the program counter, plus an optionally scaled index value taken from a second register are used as the address of the operand in memory. The index size and scale factor are selectable.

General syntax: `(pc, r<I>.<b|w|l|q> [* <2|4|8>])`

Examples:

        (pc, r1.w)
        (pc, d1.l * 2)
        (pc, r5.b * 8)
        (pc, a0.l * 4)

* Allowed index register names: r0 ... r15, d0 ... d7, a0 ... a7, sp
* All bits of the base register are used.
* For .b, .w and .l sized indexes, the register fragment is treated as a signed value:
    - For a .b index, a register value of 0x00000000000000FF is interpreted as -1, not 255.
* Cannot be used for destination operands.

| Mode | Bytecode | Ext 0 |
| - | - | - |
| `(pc, r<I>.b)` | 0xA0 | 0x0*I* |
| `(pc, r<I>.w)` | 0xA1 | 0x0*I* |
| `(pc, r<I>.l)` | 0xA2 | 0x0*I* |
| `(pc, r<I>.q)` | 0xA3 | 0x0*I* |
| `(pc, r<I>.b * 2)` | 0xA4 | 0x0*I* |
| `(pc, r<I>.w * 2)` | 0xA5 | 0x0*I* |
| `(pc, r<I>.l * 2)` | 0xA6 | 0x0*I* |
| `(pc, r<I>.q * 2)` | 0xA7 | 0x0*I* |
| `(pc, r<I>.b * 4)` | 0xA8 | 0x0*I* |
| `(pc, r<I>.w * 4)` | 0xA9 | 0x0*I* |
| `(pc, r<I>.l * 4)` | 0xAA | 0x0*I* |
| `(pc, r<I>.q * 4)` | 0xAB | 0x0*I* |
| `(pc, r<I>.b * 8)` | 0xAC | 0x0*I* |
| `(pc, r<I>.w * 8)` | 0xAD | 0x0*I* |
| `(pc, r<I>.l * 8)` | 0xAE | 0x0*I* |
| `(pc, r<I>.q * 8)` | 0xAF | 0x0*I* |

___

### Program Counter Indirect with (Scaled) Index and Displacement

The contents of the program counter, plus an optionally scaled index value taken from a second register, plus the signed 32-bit displacement are used as the address of the operand in memory. The index size and scale factor are selectable.

General syntax: `<D>(pc, r<I>.<b|w|l|q> [* <2|4|8>])` or `(<D>, pc, r<I>.<b|w|l|q> [* <2|4|8>])`

Examples:

        8(pc, r1.w)
        -4(pc, d1.l * 2)
        (32, pc, r5.b * 8)
        (-10, pc, a0.l * 4)

* Allowed index register names: r0 ... r15, d0 ... d7, a0 ... a7, sp
* All bits of the base register are used.
* For .b, .w and .l sized indexes, the register fragment is treated as a signed value:
    - For a .b index, a register value of 0x00000000000000FF is interpreted as -1, not 255.
* D = -2147483648 ... 2147483647
* Cannot be used for destination operands.

| Mode | Bytecode | Ext 0 | Ext 1  | Ext 2 | Ext 3 | Ext 4 |
| - | - | - | - | - | - | - |
| `<D>(pc, r<I>.b)` | 0xB0 | 0x0*I* | 0x*DD* | 0x*DD* | 0x*DD* | 0x*DD* |
| `<D>(pc, r<I>.w)` | 0xB1 | 0x0*I* | 0x*DD* | 0x*DD* | 0x*DD* | 0x*DD* |
| `<D>(pc, r<I>.l)` | 0xB2 | 0x0*I* | 0x*DD* | 0x*DD* | 0x*DD* | 0x*DD* |
| `<D>(pc, r<I>.q)` | 0xB3 | 0x0*I* | 0x*DD* | 0x*DD* | 0x*DD* | 0x*DD* |
| `<D>(pc, r<I>.b * 2)` | 0xB4 | 0x0*I* | 0x*DD* | 0x*DD* | 0x*DD* | 0x*DD* |
| `<D>(pc, r<I>.w * 2)` | 0xB5 | 0x0*I* | 0x*DD* | 0x*DD* | 0x*DD* | 0x*DD* |
| `<D>(pc, r<I>.l * 2)` | 0xB6 | 0x0*I* | 0x*DD* | 0x*DD* | 0x*DD* | 0x*DD* |
| `<D>(pc, r<I>.q * 2)` | 0xB7 | 0x0*I* | 0x*DD* | 0x*DD* | 0x*DD* | 0x*DD* |
| `<D>(pc, r<I>.b * 4)` | 0xB8 | 0x0*I* | 0x*DD* | 0x*DD* | 0x*DD* | 0x*DD* |
| `<D>(pc, r<I>.w * 4)` | 0xB9 | 0x0*I* | 0x*DD* | 0x*DD* | 0x*DD* | 0x*DD* |
| `<D>(pc, r<I>.l * 4)` | 0xBA | 0x0*I* | 0x*DD* | 0x*DD* | 0x*DD* | 0x*DD* |
| `<D>(pc, r<I>.q * 4)` | 0xBB | 0x0*I* | 0x*DD* | 0x*DD* | 0x*DD* | 0x*DD* |
| `<D>(pc, r<I>.b * 8)` | 0xBC | 0x0*I* | 0x*DD* | 0x*DD* | 0x*DD* | 0x*DD* |
| `<D>(pc, r<I>.w * 8)` | 0xBD | 0x0*I* | 0x*DD* | 0x*DD* | 0x*DD* | 0x*DD* |
| `<D>(pc, r<I>.l * 8)` | 0xBE | 0x0*I* | 0x*DD* | 0x*DD* | 0x*DD* | 0x*DD* |
| `<D>(pc, r<I>.q * 8)` | 0xBF | 0x0*I* | 0x*DD* | 0x*DD* | 0x*DD* | 0x*DD* |

___

### Program Indirect Counter with Displacement

The contents of the program counter, plus the signed 32-bit displacement are used as the address of the operand data in memory.

General syntax: `<D>(pc)` or `(<D>, pc)`

Examples:

        8(pc)
        0xA(pc)
        (10, pc)

* D = -2147483648 ... 2147483647
* Cannot be used for destination operands.

| Mode | Bytecode | Ext 0 | Ext 1  | Ext 2 | Ext 3 |
| - | - | - | - | - | - |
| `<D>(pc)` | 0xC0 | 0x*DD* | 0x*DD* | 0x*DD* | 0x*DD* |

___

### Integer Immediate

An signed immediate integer value is encoded into the instruction stream.

General syntax: `#<D>`

* The number of bytes for the immediate varies depending on the magnitude of D:
    - D = 0 ... 8: value is encoded directly in the EA byte.
    - D = -128 ... 127: value is encoded in 1 extension byte.
    - D = -32768 ... 32767: value is encoded in 2 extension bytes.
    - D = -2147483648 ... 2147483647: value is encoded in 4 extension bytes.
    - D = -9223372036854775808 ... 9223372036854775807: value is encoded in 8 extension bytes.
* For operations involving word sizes larger than the encoded size of D, D is sign extended.
* Cannot be used for destination operands.
* Cannot use values of D that are larger than the corresponding operation size.

| Mode | Bytecode | Ext 0 | Ext 1 | ... | Ext (size-1) |
| - | - | - | - | - | - |
| `#0` | 0xC1 | N/A | ... | ... | ... |
| `#1` | 0xC2 | N/A | ... | ... | ... |
| `#2` | 0xC3 | N/A | ... | ... | ... |
| `#3` | 0xC4 | N/A | ... | ... | ... |
| `#4` | 0xC5 | N/A | ... | ... | ... |
| `#5` | 0xC6 | N/A | ... | ... | ... |
| `#6` | 0xC7 | N/A | ... | ... | ... |
| `#7` | 0xC8 | N/A | ... | ... | ... |
| `#8` | 0xC9 | N/A | ... | ... | ... |
| `#<D.b>` | 0xCA | 0x*DD* | N/A | ... | ... |
| `#<D.w>` | 0xCB | 0x*DD* | 0x*DD* | N/A | ... |
| `#<D.l>` | 0xCC | 0x*DD* | 0x*DD* | ... | 0x*DD* |
| `#<D.q>` | 0xCD | 0x*DD* | 0x*DD* | ... | 0x*DD* |

___

### Float Immediate

A floating point value is encoded into the instruction stream.

General syntax: `#<F.>`

Examples:

        #0.
        #-1.
        #0.5
        #1.e10
        #1.25e-10
        #0.005

* Floating point immediates are differentiated from integer immediates by the presence of the decimal separator.
* Instruction size determines actual datatype used.
* Only available for floating point operations.

| Mode | Bytecode | Ext 0 | ... | Ext (size-1) |
| - | - | - | - | - |
| `#<F.s>` | 0xCE | 0x*FF* | ... | 0x*FF* |
| `#<F.d>` | 0xCF | 0x*FF* | ... | 0x*FF* |

___

### Same As Destination

The fully evaluated destination operand address is used for the source operand in memory.

* Cannot be used for destination operands.
* Used where the Source Effective Address mode is the same as the Destination Effective Address mode and that mode does not have any increment/decrement behaviour.
* Bytecode / decode logic optimisation.

| Mode | Bytecode |
| - | - |
|   | 0xD0 |
