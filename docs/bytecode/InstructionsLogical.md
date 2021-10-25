## [Documentation](../README.md) > [Bytecode Format](./README.md) > [Instruction Layout](./Instructions.md) > Logical Group

The bytecode formats for the suppored bitwise logical instructions are documented here.

One byte is used for the Instruction Opcode:

* Opcodes without any operands are 1 byte.
* Opcodes that have operands place them in the following order:
    - Destination [Effective Address](EffectiveAddress.md).
    - Source [Effective Address](EffectiveAddress.md).
    - Branch displacements.
    - Other data.
___

### AND

Bitwise AND.

        <ea(s)> & <ea(d)> -> <ea(d)>

General syntax:

        and.<b|w|l|q> <ea(s)>, <ea(d)>

| Mnemonic | Bytecode | Ext 0 | ... | ... | ... |
| - | - | - | - | - | - |
| `and.b <ea(s)>, <ea(d)>` | 0x72 | 0x*EA*(d) | ... | 0x*EA*(s) | ... |
| `and.w <ea(s)>, <ea(d)>` | 0x73 | 0x*EA*(d) | ... | 0x*EA*(s) | ... |
| `and.l <ea(s)>, <ea(d)>` | 0x74 | 0x*EA*(d) | ... | 0x*EA*(s) | ... |
| `and.q <ea(s)>, <ea(d)>` | 0x75 | 0x*EA*(d) | ... | 0x*EA*(s) | ... |

___

### OR

Bitwise OR.

        <ea(s)> | <ea(d)> -> <ea(d)>

General syntax:

        or.<b|w|l|q> <ea(s)>, <ea(d)>

| Mnemonic | Bytecode | Ext 0 | ... | ... | ... |
| - | - | - | - | - | - |
| `or.b <ea(s)>, <ea(d)>` | 0x76 | 0x*EA*(d) | ... | 0x*EA*(s) | ... |
| `or.w <ea(s)>, <ea(d)>` | 0x77 | 0x*EA*(d) | ... | 0x*EA*(s) | ... |
| `or.l <ea(s)>, <ea(d)>` | 0x78 | 0x*EA*(d) | ... | 0x*EA*(s) | ... |
| `or.q <ea(s)>, <ea(d)>` | 0x79 | 0x*EA*(d) | ... | 0x*EA*(s) | ... |

___

### EOR

Bitwise Exclusive OR.

        <ea(s)> ^ <ea(d)> -> <ea(d)>

`eor.<b|w|l|q> <ea(s)>, <ea(d)>`

| Mnemonic | Bytecode | Ext 0 | ... | ... | ... |
| - | - | - | - | - | - |
| `eor.b <ea(s)>, <ea(d)>` | 0x7A | 0x*EA*(d) | ... | 0x*EA*(s) | ... |
| `eor.w <ea(s)>, <ea(d)>` | 0x7B | 0x*EA*(d) | ... | 0x*EA*(s) | ... |
| `eor.l <ea(s)>, <ea(d)>` | 0x7C | 0x*EA*(d) | ... | 0x*EA*(s) | ... |
| `eor.q <ea(s)>, <ea(d)>` | 0x7D | 0x*EA*(d) | ... | 0x*EA*(s) | ... |

___

### NOT

Bitwise Complement.

        ~<ea(s)> -> <ea(d)>

General syntax:

        not.<b|w|l|q> <ea(s)>, <ea(d)>

| Mnemonic | Bytecode | Ext 0 | ... | ... | ... |
| - | - | - | - | - | - |
| `not.b <ea(s)>, <ea(d)>` | 0x7E | 0x*EA*(d) | ... | 0x*EA*(s) | ... |
| `not.w <ea(s)>, <ea(d)>` | 0x7F | 0x*EA*(d) | ... | 0x*EA*(s) | ... |
| `not.l <ea(s)>, <ea(d)>` | 0x80 | 0x*EA*(d) | ... | 0x*EA*(s) | ... |
| `not.q <ea(s)>, <ea(d)>` | 0x81 | 0x*EA*(d) | ... | 0x*EA*(s) | ... |

___

### LSL

Logical Shift Left.

        <ea(d)> << <ea(s)> -> <ea(d)>

General syntax:

        lsl.<b|w|l|q> <ea(s)>, <ea(d)>

* The source operand is accessed as a byte and modulo by the operation size.

| Mnemonic | Bytecode | Ext 0 | ... | ... | ... |
| - | - | - | - | - | - |
| `lsl.b <ea(s)>, <ea(d)>` | 0x82 | 0x*EA*(d) | ... | 0x*EA*(s) | ... |
| `lsl.w <ea(s)>, <ea(d)>` | 0x83 | 0x*EA*(d) | ... | 0x*EA*(s) | ... |
| `lsl.l <ea(s)>, <ea(d)>` | 0x84 | 0x*EA*(d) | ... | 0x*EA*(s) | ... |
| `lsl.q <ea(s)>, <ea(d)>` | 0x85 | 0x*EA*(d) | ... | 0x*EA*(s) | ... |

___

### LSR

Logical Shift Right.

        <ea(d)> >> <ea(s)> -> <ea(d)>

General syntax:

        lsl.<b|w|l|q> <ea(s)>, <ea(d)>

* The source operand is accessed as a byte and modulo by the operation size.

| Mnemonic | Bytecode | Ext 0 | ... | ... | ... |
| - | - | - | - | - | - |
| `lsr.b <ea(s)>, <ea(d)>` | 0x86 | 0x*EA*(d) | ... | 0x*EA*(s) | ... |
| `lsr.w <ea(s)>, <ea(d)>` | 0x87 | 0x*EA*(d) | ... | 0x*EA*(s) | ... |
| `lsr.l <ea(s)>, <ea(d)>` | 0x88 | 0x*EA*(d) | ... | 0x*EA*(s) | ... |
| `lsr.q <ea(s)>, <ea(d)>` | 0x89 | 0x*EA*(d) | ... | 0x*EA*(s) | ... |

___

### ROL

Rotate Left.

        <ea(d)> <<@ <ea(s)> -> <ea(d)>

General syntax:

        rol.<b|w|l|q> <ea(s)>, <ea(d)>

* The source operand is accessed as a byte and modulo by the operation size.

| Mnemonic | Bytecode | Ext 0 | ... | ... | ... |
| - | - | - | - | - | - |
| `rol.b <ea(s)>, <ea(d)>` | 0x8A | 0x*EA*(d) | ... | 0x*EA*(s) | ... |
| `rol.w <ea(s)>, <ea(d)>` | 0x8B | 0x*EA*(d) | ... | 0x*EA*(s) | ... |
| `rol.l <ea(s)>, <ea(d)>` | 0x8C | 0x*EA*(d) | ... | 0x*EA*(s) | ... |
| `rol.q <ea(s)>, <ea(d)>` | 0x8D | 0x*EA*(d) | ... | 0x*EA*(s) | ... |

___

### ROR

Rotate Right.

        <ea(d)> @>> <ea(s)> -> <ea(d)>

General syntax:

        ror.<b|w|l|q> <ea(s)>, <ea(d)>

* The source operand is accessed as a byte and modulo by the operation size.

| Mnemonic | Bytecode | Ext 0 | ... | ... | ... |
| - | - | - | - | - | - |
| `ror.b <ea(s)>, <ea(d)>` | 0x8E | 0x*EA*(d) | ... | 0x*EA*(s) | ... |
| `ror.w <ea(s)>, <ea(d)>` | 0x8F | 0x*EA*(d) | ... | 0x*EA*(s) | ... |
| `ror.l <ea(s)>, <ea(d)>` | 0x90 | 0x*EA*(d) | ... | 0x*EA*(s) | ... |
| `ror.q <ea(s)>, <ea(d)>` | 0x91 | 0x*EA*(d) | ... | 0x*EA*(s) | ... |

___

### BCLR

Clear a bit in the destination.

        0 -> <ea(d)>[ Bit:<ea(s)> ]

General syntax:

        bclr.<b|w|l|q> <ea(s)>, <ea(d)>

* The source operand is accessed as a byte, modulo by the operation size and interpreted as a bit position.

| Mnemonic | Bytecode | Ext 0 | ... | ... | ... |
| - | - | - | - | - | - |
| `bclr.b <ea(s)>, <ea(d)>` | 0x92 | 0x*EA*(d) | ... | 0x*EA*(s) | ... |
| `bclr.w <ea(s)>, <ea(d)>` | 0x93 | 0x*EA*(d) | ... | 0x*EA*(s) | ... |
| `bclr.l <ea(s)>, <ea(d)>` | 0x94 | 0x*EA*(d) | ... | 0x*EA*(s) | ... |
| `bclr.q <ea(s)>, <ea(d)>` | 0x95 | 0x*EA*(d) | ... | 0x*EA*(s) | ... |

___

### BSET

Set a bit in the destination.

        1 -> <ea(d)>[ Bit:<ea(s)> ]

General syntax:

        bset.<b|w|l|q> <ea(s)>, <ea(d)>

* The source operand is accessed as a byte, modulo by the operation size and interpreted as a bit position.

| Mnemonic | Bytecode | Ext 0 | ... | ... | ... |
| - | - | - | - | - | - |
| `bset.b <ea(s)>, <ea(d)>` | 0x96 | 0x*EA*(d) | ... | 0x*EA*(s) | ... |
| `bset.w <ea(s)>, <ea(d)>` | 0x97 | 0x*EA*(d) | ... | 0x*EA*(s) | ... |
| `bset.l <ea(s)>, <ea(d)>` | 0x98 | 0x*EA*(d) | ... | 0x*EA*(s) | ... |
| `bset.q <ea(s)>, <ea(d)>` | 0x99 | 0x*EA*(d) | ... | 0x*EA*(s) | ... |

___

### BFFFO

Find First One in Bitfield.

General syntax:

| Mnemonic | Bytecode | Ext 0 | ... | ... | ... |
| - | - | - | - | - | - |
| `<bfffo>` | 0x9A | 0x*EA*(d) | ... | 0x*EA*(s) | ... |

___

### BFCNT

Count number of set bits.

        pop_count(<ea(s)>) -> <ea(d)>

General syntax:

        bfcnt <ea(s)>, <ea(d)>

| Mnemonic | Bytecode | Ext 0 | ... | ... | ... |
| - | - | - | - | - | - |
| `bfcnt <ea(s)>, <ea(d)>` | 0x9B | 0x*EA*(d) | ... | 0x*EA*(s) | ... |

___


