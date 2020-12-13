## Bytecode Format > [Instruction Layout](Instructions.md) > Logical Group

The bytecode formats for the suppored bitwise logical instructions are documented here.

One byte is used for the Instruction Opcode:

* Opcodes without any operands are 1 byte.
* Opcodes that have operands place them in the following order:
    - Destination [Effective Address](EffectiveAddress.md).
    - Source [Effective Address](EffectiveAddress.md).
    - Branch displacements.
    - Other data.

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

### BCLR

Clear a bit in the destination

        0 -> <ea(d)>[ Bit:<ea(s)> ]

`ror.<b|w|l|q> <ea(s)>, <ea(d)>`

* The source operand is accessed as a byte, modulo by the operation size and interpreted as a bit position.

| Mnemonic | Bytecode | Ext 0 | ... | ... | ... |
| - | - | - | - | - | - |
| `bclr.b <ea(s)>, <ea(d)>` | 0x90 | 0xEA(d) | ... | 0xEA(s) | ... |
| `bclr.w <ea(s)>, <ea(d)>` | 0x91 | 0xEA(d) | ... | 0xEA(s) | ... |
| `bclr.l <ea(s)>, <ea(d)>` | 0x92 | 0xEA(d) | ... | 0xEA(s) | ... |
| `bclr.q <ea(s)>, <ea(d)>` | 0x93 | 0xEA(d) | ... | 0xEA(s) | ... |

### BSET

Set a bit in the destination

        1 -> <ea(d)>[ Bit:<ea(s)> ]

`ror.<b|w|l|q> <ea(s)>, <ea(d)>`

* The source operand is accessed as a byte, modulo by the operation size and interpreted as a bit position.

| Mnemonic | Bytecode | Ext 0 | ... | ... | ... |
| - | - | - | - | - | - |
| `bclr.b <ea(s)>, <ea(d)>` | 0x94 | 0xEA(d) | ... | 0xEA(s) | ... |
| `bclr.w <ea(s)>, <ea(d)>` | 0x95 | 0xEA(d) | ... | 0xEA(s) | ... |
| `bclr.l <ea(s)>, <ea(d)>` | 0x96 | 0xEA(d) | ... | 0xEA(s) | ... |
| `bclr.q <ea(s)>, <ea(d)>` | 0x97 | 0xEA(d) | ... | 0xEA(s) | ... |


### BFCLR

Clear Bitfield

        0 -> r<D>[ Bitfield ]

| Mnemonic | Bytecode | Ext 0 | ... | ... | ... |
| - | - | - | - | - | - |
| `` | 0x98 | 0xEA(d) | ... | 0xEA(s) | ... |

### BFSET

Set Bitfield

        1 -> r<D>[ Bitfield ]

| Mnemonic | Bytecode | Ext 0 | ... | ... | ... |
| - | - | - | - | - | - |
| `` | 0x99 | 0xEA(d) | ... | 0xEA(s) | ... |

### BFEXTS

Extract Bitfield as Signed

### BFEXTU

Extract Bitfield as Unsigned

### BFINS

Insert Bitfield

### BFFFO

Find First One in Bitfield

### BFCNT

Count number of set bits

        POPCOUNT(<ea(s)> -> <ea(d)>

`bfcnt <ea(s)>, <ea(d)>`

| Mnemonic | Bytecode | Ext 0 | ... | ... | ... |
| - | - | - | - | - | - |
| `bcnt.b <ea(s)>, <ea(d)>` | 0x9E | 0xEA(d) | ... | 0xEA(s) | ... |


