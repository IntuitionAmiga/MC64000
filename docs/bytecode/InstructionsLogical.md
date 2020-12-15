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

Bitwise AND

        <ea(s)> & <ea(d)> -> <ea(d)>

`and.<b|w|l|q> <ea(s)>, <ea(d)>`

| Mnemonic | Bytecode | Ext 0 | ... | ... | ... |
| - | - | - | - | - | - |
| `and.b <ea(s)>, <ea(d)>` | 0x70 | 0xEA(d) | ... | 0xEA(s) | ... |
| `and.w <ea(s)>, <ea(d)>` | 0x71 | 0xEA(d) | ... | 0xEA(s) | ... |
| `and.l <ea(s)>, <ea(d)>` | 0x71 | 0xEA(d) | ... | 0xEA(s) | ... |
| `and.q <ea(s)>, <ea(d)>` | 0x73 | 0xEA(d) | ... | 0xEA(s) | ... |

___

### OR

Bitwise OR

        <ea(s)> | <ea(d)> -> <ea(d)>

`or.<b|w|l|q> <ea(s)>, <ea(d)>`

| Mnemonic | Bytecode | Ext 0 | ... | ... | ... |
| - | - | - | - | - | - |
| `or.b <ea(s)>, <ea(d)>` | 0x74 | 0xEA(d) | ... | 0xEA(s) | ... |
| `or.w <ea(s)>, <ea(d)>` | 0x75 | 0xEA(d) | ... | 0xEA(s) | ... |
| `or.l <ea(s)>, <ea(d)>` | 0x76 | 0xEA(d) | ... | 0xEA(s) | ... |
| `or.q <ea(s)>, <ea(d)>` | 0x77 | 0xEA(d) | ... | 0xEA(s) | ... |

___

### EOR

Bitwise Exclusive OR

        <ea(s)> ^ <ea(d)> -> <ea(d)>

`eor.<b|w|l|q> <ea(s)>, <ea(d)>`

| Mnemonic | Bytecode | Ext 0 | ... | ... | ... |
| - | - | - | - | - | - |
| `eor.b <ea(s)>, <ea(d)>` | 0x78 | 0xEA(d) | ... | 0xEA(s) | ... |
| `eor.w <ea(s)>, <ea(d)>` | 0x79 | 0xEA(d) | ... | 0xEA(s) | ... |
| `eor.l <ea(s)>, <ea(d)>` | 0x7A | 0xEA(d) | ... | 0xEA(s) | ... |
| `eor.q <ea(s)>, <ea(d)>` | 0x7B | 0xEA(d) | ... | 0xEA(s) | ... |

___

### NOT

Bitwise Complement

        ~<ea(s)> -> <ea(d)>

`not.<b|w|l|q> <ea(s)>, <ea(d)>`

| Mnemonic | Bytecode | Ext 0 | ... | ... | ... |
| - | - | - | - | - | - |
| `not.b <ea(s)>, <ea(d)>` | 0x7C | 0xEA(d) | ... | 0xEA(s) | ... |
| `not.w <ea(s)>, <ea(d)>` | 0x7D | 0xEA(d) | ... | 0xEA(s) | ... |
| `not.l <ea(s)>, <ea(d)>` | 0x7E | 0xEA(d) | ... | 0xEA(s) | ... |
| `not.q <ea(s)>, <ea(d)>` | 0x7F | 0xEA(d) | ... | 0xEA(s) | ... |

___

### LSL

Logical Shift Left

        <ea(d)> << <ea(s)> -> <ea(d)>

`lsl.<b|w|l|q> <ea(s)>, <ea(d)>`

* The source operand is accessed as a byte and modulo by the operation size.

| Mnemonic | Bytecode | Ext 0 | ... | ... | ... |
| - | - | - | - | - | - |
| `lsl.b <ea(s)>, <ea(d)>` | 0x80 | 0xEA(d) | ... | 0xEA(s) | ... |
| `lsl.w <ea(s)>, <ea(d)>` | 0x81 | 0xEA(d) | ... | 0xEA(s) | ... |
| `lsl.l <ea(s)>, <ea(d)>` | 0x82 | 0xEA(d) | ... | 0xEA(s) | ... |
| `lsl.q <ea(s)>, <ea(d)>` | 0x83 | 0xEA(d) | ... | 0xEA(s) | ... |

___

### LSR

Logical Shift Right

        <ea(d)> >> <ea(s)> -> <ea(d)>

`lsl.<b|w|l|q> <ea(s)>, <ea(d)>`

* The source operand is accessed as a byte and modulo by the operation size.

| Mnemonic | Bytecode | Ext 0 | ... | ... | ... |
| - | - | - | - | - | - |
| `lsr.b <ea(s)>, <ea(d)>` | 0x84 | 0xEA(d) | ... | 0xEA(s) | ... |
| `lsr.w <ea(s)>, <ea(d)>` | 0x85 | 0xEA(d) | ... | 0xEA(s) | ... |
| `lsr.l <ea(s)>, <ea(d)>` | 0x86 | 0xEA(d) | ... | 0xEA(s) | ... |
| `lsr.q <ea(s)>, <ea(d)>` | 0x87 | 0xEA(d) | ... | 0xEA(s) | ... |

___

### ROL

Rotate Left

        <ea(d)> <<@ <ea(s)> -> <ea(d)>

`rol.<b|w|l|q> <ea(s)>, <ea(d)>`

* The source operand is accessed as a byte and modulo by the operation size.

| Mnemonic | Bytecode | Ext 0 | ... | ... | ... |
| - | - | - | - | - | - |
| `rol.b <ea(s)>, <ea(d)>` | 0x88 | 0xEA(d) | ... | 0xEA(s) | ... |
| `rol.w <ea(s)>, <ea(d)>` | 0x89 | 0xEA(d) | ... | 0xEA(s) | ... |
| `rol.l <ea(s)>, <ea(d)>` | 0x8A | 0xEA(d) | ... | 0xEA(s) | ... |
| `rol.q <ea(s)>, <ea(d)>` | 0x8B | 0xEA(d) | ... | 0xEA(s) | ... |

___

### ROR

Rotate Right

        <ea(d)> @>> <ea(s)> -> <ea(d)>

`ror.<b|w|l|q> <ea(s)>, <ea(d)>`

* The source operand is accessed as a byte and modulo by the operation size.

| Mnemonic | Bytecode | Ext 0 | ... | ... | ... |
| - | - | - | - | - | - |
| `ror.b <ea(s)>, <ea(d)>` | 0x8C | 0xEA(d) | ... | 0xEA(s) | ... |
| `ror.w <ea(s)>, <ea(d)>` | 0x8D | 0xEA(d) | ... | 0xEA(s) | ... |
| `ror.l <ea(s)>, <ea(d)>` | 0x8E | 0xEA(d) | ... | 0xEA(s) | ... |
| `ror.q <ea(s)>, <ea(d)>` | 0x8F | 0xEA(d) | ... | 0xEA(s) | ... |

___

### BCLR

Clear a bit in the destination

        0 -> <ea(d)>[ Bit:<ea(s)> ]

`bclr.<b|w|l|q> <ea(s)>, <ea(d)>`

* The source operand is accessed as a byte, modulo by the operation size and interpreted as a bit position.

| Mnemonic | Bytecode | Ext 0 | ... | ... | ... |
| - | - | - | - | - | - |
| `bclr.b <ea(s)>, <ea(d)>` | 0x90 | 0xEA(d) | ... | 0xEA(s) | ... |
| `bclr.w <ea(s)>, <ea(d)>` | 0x91 | 0xEA(d) | ... | 0xEA(s) | ... |
| `bclr.l <ea(s)>, <ea(d)>` | 0x92 | 0xEA(d) | ... | 0xEA(s) | ... |
| `bclr.q <ea(s)>, <ea(d)>` | 0x93 | 0xEA(d) | ... | 0xEA(s) | ... |

___

### BSET

Set a bit in the destination

        1 -> <ea(d)>[ Bit:<ea(s)> ]

`bset.<b|w|l|q> <ea(s)>, <ea(d)>`

* The source operand is accessed as a byte, modulo by the operation size and interpreted as a bit position.

| Mnemonic | Bytecode | Ext 0 | ... | ... | ... |
| - | - | - | - | - | - |
| `bset.b <ea(s)>, <ea(d)>` | 0x94 | 0xEA(d) | ... | 0xEA(s) | ... |
| `bset.w <ea(s)>, <ea(d)>` | 0x95 | 0xEA(d) | ... | 0xEA(s) | ... |
| `bset.l <ea(s)>, <ea(d)>` | 0x96 | 0xEA(d) | ... | 0xEA(s) | ... |
| `bset.q <ea(s)>, <ea(d)>` | 0x97 | 0xEA(d) | ... | 0xEA(s) | ... |

___

### BFCLR

Reserved

Clear Bitfield

        0 -> r<D>[ Bitfield ]

| Mnemonic | Bytecode | Ext 0 | ... | ... | ... |
| - | - | - | - | - | - |
| `<bfclr>` | 0x98 | 0xEA(d) | ... | 0xEA(s) | ... |

___

### BFSET

Reserved

Set Bitfield

        1 -> r<D>[ Bitfield ]

| Mnemonic | Bytecode | Ext 0 | ... | ... | ... |
| - | - | - | - | - | - |
| `<bfset>` | 0x99 | 0xEA(d) | ... | 0xEA(s) | ... |

___

### BFEXTS

Reserved

Extract Bitfield as Signed

| Mnemonic | Bytecode | Ext 0 | ... | ... | ... |
| - | - | - | - | - | - |
| `<bfexts>` | 0x9A | 0xEA(d) | ... | 0xEA(s) | ... |

Reserved

___

### BFEXTU

Extract Bitfield as Unsigned

| Mnemonic | Bytecode | Ext 0 | ... | ... | ... |
| - | - | - | - | - | - |
| `<bfextu>` | 0x9B | 0xEA(d) | ... | 0xEA(s) | ... |

___

### BFINS

Reserved

Insert Bitfield

| Mnemonic | Bytecode | Ext 0 | ... | ... | ... |
| - | - | - | - | - | - |
| `<bfins>` | 0x9C | 0xEA(d) | ... | 0xEA(s) | ... |

___

### BFFFO

Reserved

Find First One in Bitfield

| Mnemonic | Bytecode | Ext 0 | ... | ... | ... |
| - | - | - | - | - | - |
| `<bfffo>` | 0x9D | 0xEA(d) | ... | 0xEA(s) | ... |

___

### BFCNT

Reserved

Count number of set bits

        pop_count(<ea(s)>) -> <ea(d)>

`bfcnt <ea(s)>, <ea(d)>`

| Mnemonic | Bytecode | Ext 0 | ... | ... | ... |
| - | - | - | - | - | - |
| `bfcnt.b <ea(s)>, <ea(d)>` | 0x9E | 0xEA(d) | ... | 0xEA(s) | ... |

___

### BFXXX

Reserved

| Mnemonic | Bytecode | Ext 0 | ... | ... | ... |
| - | - | - | - | - | - |
| `<bfxxx>` | 0x9F | 0xEA(d) | ... | 0xEA(s) | ... |
