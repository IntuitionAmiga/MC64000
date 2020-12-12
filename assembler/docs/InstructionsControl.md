## Bytecode Format > [Instruction Layout](Instructions.md) > Control Group

The bytecode formats for the suppored control and flow instructions are documented here.

One byte is used for the Instruction Opcode:

* Opcodes without any operands are 1 byte.
* Opcodes that have operands place them in the following order:
    - Destination [Effective Address](EffectiveAddress.md).
    - Source [Effective Address](EffectiveAddress.md).
    - Branch displacements.
    - Other data.

### Control Group

#### BKPT

Breakpoint

`bkpt #<N>`

| Mnemonic | Bytecode | Ext 0 |
| - | - | - |
| `bkpt #<N>`| 0x00 | 0xNN |


#### BRA

Branch

        pc + d -> pc

`bra[.b] <#<D>|label>`

* Branch distance is measured from the end of the instruction.
    - e.g. `bra. #1` branches to the next instruction.
* Short .b form requires the label to resolve to a displacement within -128 ... 127 bytes of the current Program Counter.
* Standard form requires the label to resolve to a displacement within -2147483648 ... 2147483647 bytes of the current Program Counter.

| Mnemonic | Bytecode | Ext 0 | Ext 1 | Ext 2 | Ext 3 |
| - | - | - | - | - | - |
| `bra.b #<D>`| 0x01 | 0xDD |
| `bra #<D>` | 0x02 | 0xDD | 0xDD | 0xDD | 0xDD |

#### BSR

Branch to Subroutine

        sp - 8 -> sp; pc -> (sp); pc + d -> pc

`bra[.b] <#<D>|label>`

* Branch distance is measured from the end of the instruction.
    - e.g. `bsr. #1` branches to the next instruction.
* Short .b form requires the label to resolve to a displacement within -128 ... 127 bytes of the current Program Counter.
* Standard form requires the label to resolve to a displacement within -2147483648 ... 2147483647 bytes of the current Program Counter.

| Mnemonic | Bytecode | Ext 0 | Ext 1 | Ext 2 | Ext 3 |
| - | - | - | - | - | - |
| `bsr.b #<D>`| 0x03 | 0xDD |
| `bsr #<D>` | 0x04 | 0xDD | 0xDD | 0xDD | 0xDD |

#### JMP

Jump

        <ea> -> pc

`jmp <ea>`

* Effective Address cannot be register direct or integer immediate.

| Mnemonic | Bytecode | Ext 0 | ... |
| - | - | - | - |
| `jmp <ea>`| 0x05 | 0xEA | ... |

#### JSR

Jump to Subroutine

        sp - 8 -> sp; pc -> (sp); <ea> -> sp

`jsr <ea>`

* Effective Address cannot be register direct or integer immediate.

| Mnemonic | Bytecode | Ext 0 | ... |
| - | - | - | - |
| `jsr <ea>`| 0x06 | 0xEA | ... |

#### RTS

Return from Subroutine

        (sp) -> pc; sp + 8 -> sp

| Mnemonic | Bytecode |
| - | - |
| `rts`| 0x07 |

#### BIZ

Branch if operand is zero

        if (<ea> == 0) then pc + d -> pc

`biz.<b|w|l|q> <ea>, <#<D>|label>`

`fbiz.<s|d> <ea>, <#<D>|label>`

| Mnemonic | Bytecode | Ext 0 | ... | Ext N | Ext N+1 | Ext N+2 | Ext N+3 |
| - | - | - | - | - | - | - | - |
| `biz.b <ea>, #<D>` | 0x08 | 0xEA | ... | 0xDD | 0xDD | 0xDD | 0xDD |
| `biz.w <ea>, #<D>` | 0x09 | 0xEA | ... | 0xDD | 0xDD | 0xDD | 0xDD |
| `biz.l <ea>, #<D>` | 0x0A | 0xEA | ... | 0xDD | 0xDD | 0xDD | 0xDD |
| `biz.q <ea>, #<D>` | 0x0B | 0xEA | ... | 0xDD | 0xDD | 0xDD | 0xDD |
| `fbiz.s <ea>, #<D>` | 0x0C | 0xEA | ... | 0xDD | 0xDD | 0xDD | 0xDD |
| `fbiz.d <ea>, #<D>` | 0x0D | 0xEA | ... | 0xDD | 0xDD | 0xDD | 0xDD |

#### BNZ

Branch if operand is not zero

        if (<ea> != 0) then pc + d -> pc

`bnz.<b|w|l|q> <ea>, <#<D>|label>`

`fbnz.<s|d> <ea>, <#<D>|label>`

| Mnemonic | Bytecode | Ext 0 | ... | Ext N | Ext N+1 | Ext N+2 | Ext N+3 |
| - | - | - | - | - | - | - | - |
| `bnz.b <ea>, #<D>` | 0x0E | 0xEA | ... | 0xDD | 0xDD | 0xDD | 0xDD |
| `bnz.w <ea>, #<D>` | 0x0F | 0xEA | ... | 0xDD | 0xDD | 0xDD | 0xDD |
| `bnz.l <ea>, #<D>` | 0x10 | 0xEA | ... | 0xDD | 0xDD | 0xDD | 0xDD |
| `bnz.q <ea>, #<D>` | 0x11 | 0xEA | ... | 0xDD | 0xDD | 0xDD | 0xDD |
| `fbnz.s <ea>, #<D>` | 0x12 | 0xEA | ... | 0xDD | 0xDD | 0xDD | 0xDD |
| `fbnz.d <ea>, #<D>` | 0x13 | 0xEA | ... | 0xDD | 0xDD | 0xDD | 0xDD |

#### BMI

Branch if operand is minus

        if (<ea> < 0) then pc + d -> pc

`bmi.<b|w|l|q> <ea>, <#<D>|label>`

`fbmi.<s|d> <ea>, <#<D>|label>`

| Mnemonic | Bytecode | Ext 0 | ... | Ext N | Ext N+1 | Ext N+2 | Ext N+3 |
| - | - | - | - | - | - | - | - |
| `bmi.b <ea>, #<D>` | 0x14 | 0xEA | ... | 0xDD | 0xDD | 0xDD | 0xDD |
| `bmi.w <ea>, #<D>` | 0x15 | 0xEA | ... | 0xDD | 0xDD | 0xDD | 0xDD |
| `bmi.l <ea>, #<D>` | 0x16 | 0xEA | ... | 0xDD | 0xDD | 0xDD | 0xDD |
| `bmi.q <ea>, #<D>` | 0x17 | 0xEA | ... | 0xDD | 0xDD | 0xDD | 0xDD |
| `fbmi.s <ea>, #<D>` | 0x18 | 0xEA | ... | 0xDD | 0xDD | 0xDD | 0xDD |
| `fbmi.d <ea>, #<D>` | 0x19 | 0xEA | ... | 0xDD | 0xDD | 0xDD | 0xDD |

#### BPL

Branch if operand is plus

        if (<ea> > 0) then pc + d -> pc

`bpl.<b|w|l|q> <ea>, <#<D>|label>`

`fbpl.<s|d> <ea>, <#<D>|label>`

| Mnemonic | Bytecode | Ext 0 | ... | Ext N | Ext N+1 | Ext N+2 | Ext N+3 |
| - | - | - | - | - | - | - | - |
| `bpl.b <ea>, #<D>` | 0x1A | 0xEA | ... | 0xDD | 0xDD | 0xDD | 0xDD |
| `bpl.w <ea>, #<D>` | 0x1B | 0xEA | ... | 0xDD | 0xDD | 0xDD | 0xDD |
| `bpl.l <ea>, #<D>` | 0x1C | 0xEA | ... | 0xDD | 0xDD | 0xDD | 0xDD |
| `bpl.q <ea>, #<D>` | 0x1D | 0xEA | ... | 0xDD | 0xDD | 0xDD | 0xDD |
| `fbpl.s <ea>, #<D>` | 0x1E | 0xEA | ... | 0xDD | 0xDD | 0xDD | 0xDD |
| `fbpl.d <ea>, #<D>` | 0x1F | 0xEA | ... | 0xDD | 0xDD | 0xDD | 0xDD |

#### BLT

Branch if source operand is less than destination operand

        if (<ea(s)> < <ea(d)>) then pc + d -> pc

`blt.<b|w|l|q> <ea(s)>, <ea(d)> <#<D>|label>`

`fblt.<s|d> <ea(s)>, <ea(d)>, <#<D>|label>`

| Mnemonic | Bytecode | Ext 0 | ... | ... | ... | Ext N | Ext N+1 | Ext N+2 | Ext N+3 |
| - | - | - | - | - | - | - | - | - | - |
| `blt.b <ea(s)>, <ea(d)>, #<D>` | 0x20 | 0xEA(d) | ... | 0xEA(s) | ... | 0xDD | 0xDD | 0xDD | 0xDD |
| `blt.w <ea(s)>, <ea(d)>, #<D>` | 0x21 | 0xEA(d) | ... | 0xEA(s) | ... | 0xDD | 0xDD | 0xDD | 0xDD |
| `blt.l <ea(s)>, <ea(d)>, #<D>` | 0x22 | 0xEA(d) | ... | 0xEA(s) | ... | 0xDD | 0xDD | 0xDD | 0xDD |
| `blt.q <ea(s)>, <ea(d)>, #<D>` | 0x23 | 0xEA(d) | ... | 0xEA(s) | ... | 0xDD | 0xDD | 0xDD | 0xDD |
| `fblt.s <ea(s)>, <ea(d)>, #<D>` | 0x24 | 0xEA(d) | ... | 0xEA(s) | ... | 0xDD | 0xDD | 0xDD | 0xDD |
| `fblt.d <ea(s)>, <ea(d)>, #<D>` | 0x25 | 0xEA(d) | ... | 0xEA(s) | ... | 0xDD | 0xDD | 0xDD | 0xDD |

#### BLE

Branch if source operand is less than or equal to destination operand

        if (<ea(s)> <= <ea(d)>) then pc + d -> pc

`ble.<b|w|l|q> <ea(s)>, <ea(d)> <#<D>|label>`

`fble.<s|d> <ea(s)>, <ea(d)>, <#<D>|label>`

| Mnemonic | Bytecode | Ext 0 | ... | ... | ... | Ext N | Ext N+1 | Ext N+2 | Ext N+3 |
| - | - | - | - | - | - | - | - | - | - |
| `ble.b <ea(s)>, <ea(d)>, #<D>` | 0x26 | 0xEA(d) | ... | 0xEA(s) | ... | 0xDD | 0xDD | 0xDD | 0xDD |
| `ble.w <ea(s)>, <ea(d)>, #<D>` | 0x27 | 0xEA(d) | ... | 0xEA(s) | ... | 0xDD | 0xDD | 0xDD | 0xDD |
| `ble.l <ea(s)>, <ea(d)>, #<D>` | 0x28 | 0xEA(d) | ... | 0xEA(s) | ... | 0xDD | 0xDD | 0xDD | 0xDD |
| `ble.q <ea(s)>, <ea(d)>, #<D>` | 0x29 | 0xEA(d) | ... | 0xEA(s) | ... | 0xDD | 0xDD | 0xDD | 0xDD |
| `fble.s <ea(s)>, <ea(d)>, #<D>` | 0x2A | 0xEA(d) | ... | 0xEA(s) | ... | 0xDD | 0xDD | 0xDD | 0xDD |
| `fble.d <ea(s)>, <ea(d)>, #<D>` | 0x2B | 0xEA(d) | ... | 0xEA(s) | ... | 0xDD | 0xDD | 0xDD | 0xDD |

#### BEQ

Branch if source operand is equal to destination operand

        if (<ea(s)> == <ea(d)>) then pc + d -> pc

`beq.<b|w|l|q> <ea(s)>, <ea(d)> <#<D>|label>`

`fbeq.<s|d> <ea(s)>, <ea(d)>, <#<D>|label>`

| Mnemonic | Bytecode | Ext 0 | ... | ... | ... | Ext N | Ext N+1 | Ext N+2 | Ext N+3 |
| - | - | - | - | - | - | - | - | - | - |
| `beq.b <ea(s)>, <ea(d)>, #<D>` | 0x2C | 0xEA(d) | ... | 0xEA(s) | ... | 0xDD | 0xDD | 0xDD | 0xDD |
| `beq.w <ea(s)>, <ea(d)>, #<D>` | 0x2D | 0xEA(d) | ... | 0xEA(s) | ... | 0xDD | 0xDD | 0xDD | 0xDD |
| `beq.l <ea(s)>, <ea(d)>, #<D>` | 0x2E | 0xEA(d) | ... | 0xEA(s) | ... | 0xDD | 0xDD | 0xDD | 0xDD |
| `beq.q <ea(s)>, <ea(d)>, #<D>` | 0x2F | 0xEA(d) | ... | 0xEA(s) | ... | 0xDD | 0xDD | 0xDD | 0xDD |
| `fbeq.s <ea(s)>, <ea(d)>, #<D>` | 0x30 | 0xEA(d) | ... | 0xEA(s) | ... | 0xDD | 0xDD | 0xDD | 0xDD |
| `fbeq.d <ea(s)>, <ea(d)>, #<D>` | 0x31 | 0xEA(d) | ... | 0xEA(s) | ... | 0xDD | 0xDD | 0xDD | 0xDD |

#### BGE

Branch if source operand is equal to or greater than destination operand

        if (<ea(s)> >= <ea(d)>) then pc + d -> pc

`bge.<b|w|l|q> <ea(s)>, <ea(d)> <#<D>|label>`

`fbge.<s|d> <ea(s)>, <ea(d)>, <#<D>|label>`

| Mnemonic | Bytecode | Ext 0 | ... | ... | ... | Ext N | Ext N+1 | Ext N+2 | Ext N+3 |
| - | - | - | - | - | - | - | - | - | - |
| `bge.b <ea(s)>, <ea(d)>, #<D>` | 0x32 | 0xEA(d) | ... | 0xEA(s) | ... | 0xDD | 0xDD | 0xDD | 0xDD |
| `bge.w <ea(s)>, <ea(d)>, #<D>` | 0x33 | 0xEA(d) | ... | 0xEA(s) | ... | 0xDD | 0xDD | 0xDD | 0xDD |
| `bge.l <ea(s)>, <ea(d)>, #<D>` | 0x34 | 0xEA(d) | ... | 0xEA(s) | ... | 0xDD | 0xDD | 0xDD | 0xDD |
| `bge.q <ea(s)>, <ea(d)>, #<D>` | 0x35 | 0xEA(d) | ... | 0xEA(s) | ... | 0xDD | 0xDD | 0xDD | 0xDD |
| `fbge.s <ea(s)>, <ea(d)>, #<D>` | 0x36 | 0xEA(d) | ... | 0xEA(s) | ... | 0xDD | 0xDD | 0xDD | 0xDD |
| `fbge.d <ea(s)>, <ea(d)>, #<D>` | 0x37 | 0xEA(d) | ... | 0xEA(s) | ... | 0xDD | 0xDD | 0xDD | 0xDD |

#### BGT

Branch if source operand is greater than destination operand

        if (<ea(s)> > <ea(d)>) then pc + d -> pc

`bgt.<b|w|l|q> <ea(s)>, <ea(d)> <#<D>|label>`

`fbgt.<s|d> <ea(s)>, <ea(d)>, <#<D>|label>`

| Mnemonic | Bytecode | Ext 0 | ... | ... | ... | Ext N | Ext N+1 | Ext N+2 | Ext N+3 |
| - | - | - | - | - | - | - | - | - | - |
| `bgt.b <ea(s)>, <ea(d)>, #<D>` | 0x38 | 0xEA(d) | ... | 0xEA(s) | ... | 0xDD | 0xDD | 0xDD | 0xDD |
| `bgt.w <ea(s)>, <ea(d)>, #<D>` | 0x39 | 0xEA(d) | ... | 0xEA(s) | ... | 0xDD | 0xDD | 0xDD | 0xDD |
| `bgt.l <ea(s)>, <ea(d)>, #<D>` | 0x3A | 0xEA(d) | ... | 0xEA(s) | ... | 0xDD | 0xDD | 0xDD | 0xDD |
| `bgt.q <ea(s)>, <ea(d)>, #<D>` | 0x3B | 0xEA(d) | ... | 0xEA(s) | ... | 0xDD | 0xDD | 0xDD | 0xDD |
| `fbgt.s <ea(s)>, <ea(d)>, #<D>` | 0x3C | 0xEA(d) | ... | 0xEA(s) | ... | 0xDD | 0xDD | 0xDD | 0xDD |
| `fbgt.d <ea(s)>, <ea(d)>, #<D>` | 0x3D | 0xEA(d) | ... | 0xEA(s) | ... | 0xDD | 0xDD | 0xDD | 0xDD |

#### BNE

Branch if source operand is not equal to destination operand

        if (<ea(s)> != <ea(d)>) then pc + d -> pc

`bne.<b|w|l|q> <ea(s)>, <ea(d)> <#<D>|label>`

`fbne.<s|d> <ea(s)>, <ea(d)>, <#<D>|label>`

| Mnemonic | Bytecode | Ext 0 | ... | ... | ... | Ext N | Ext N+1 | Ext N+2 | Ext N+3 |
| - | - | - | - | - | - | - | - | - | - |
| `bne.b <ea(s)>, <ea(d)>, #<D>` | 0x3E | 0xEA(d) | ... | 0xEA(s) | ... | 0xDD | 0xDD | 0xDD | 0xDD |
| `bne.w <ea(s)>, <ea(d)>, #<D>` | 0x3F | 0xEA(d) | ... | 0xEA(s) | ... | 0xDD | 0xDD | 0xDD | 0xDD |
| `bne.l <ea(s)>, <ea(d)>, #<D>` | 0x40 | 0xEA(d) | ... | 0xEA(s) | ... | 0xDD | 0xDD | 0xDD | 0xDD |
| `bne.q <ea(s)>, <ea(d)>, #<D>` | 0x41 | 0xEA(d) | ... | 0xEA(s) | ... | 0xDD | 0xDD | 0xDD | 0xDD |
| `fbne.s <ea(s)>, <ea(d)>, #<D>` | 0x42 | 0xEA(d) | ... | 0xEA(s) | ... | 0xDD | 0xDD | 0xDD | 0xDD |
| `fbne.d <ea(s)>, <ea(d)>, #<D>` | 0x43 | 0xEA(d) | ... | 0xEA(s) | ... | 0xDD | 0xDD | 0xDD | 0xDD |


#### BBS

Branch if bit is set

        if ( ((1 << <ea(s)>) & <ea(d)>) != 0 ) then pc + d -> pc

`bbs.<b|w|l|q> <ea(s)>, <ea(d)>`

* All possible integer Effective Address modes are valid for source and destination.
* Source value is accessed as a byte, modulo by the operation size.

| Mnemonic | Bytecode | Ext 0 | ... | ... | ... | Ext N | Ext N+1 | Ext N+2 | Ext N+3 |
| - | - | - | - | - | - | - | - | - | - |
| `bbs.b <ea(s)>, <ea(d)>, #<D>` | 0x44 | 0xEA(d) | ... | 0xEA(s) | ... | 0xDD | 0xDD | 0xDD | 0xDD |
| `bbs.w <ea(s)>, <ea(d)>, #<D>` | 0x45 | 0xEA(d) | ... | 0xEA(s) | ... | 0xDD | 0xDD | 0xDD | 0xDD |
| `bbs.l <ea(s)>, <ea(d)>, #<D>` | 0x46 | 0xEA(d) | ... | 0xEA(s) | ... | 0xDD | 0xDD | 0xDD | 0xDD |
| `bbs.q <ea(s)>, <ea(d)>, #<D>` | 0x47 | 0xEA(d) | ... | 0xEA(s) | ... | 0xDD | 0xDD | 0xDD | 0xDD |

#### BBC

Branch if bit is clear

        if ( ((1 << <ea(s)>) & <ea(d)>) == 0 ) then pc + d -> pc

`bbc.<b|w|l|q> <ea(s)>, <ea(d)>`

* All possible integer Effective Address modes are valid for source and destination.
* Source value is accessed as a byte, modulo by the operation size.

| Mnemonic | Bytecode | Ext 0 | ... | ... | ... | Ext N | Ext N+1 | Ext N+2 | Ext N+3 |
| - | - | - | - | - | - | - | - | - | - |
| `bbc.b <ea(s)>, <ea(d)>, #<D>` | 0x48 | 0xEA(d) | ... | 0xEA(s) | ... | 0xDD | 0xDD | 0xDD | 0xDD |
| `bbc.w <ea(s)>, <ea(d)>, #<D>` | 0x49 | 0xEA(d) | ... | 0xEA(s) | ... | 0xDD | 0xDD | 0xDD | 0xDD |
| `bbc.l <ea(s)>, <ea(d)>, #<D>` | 0x4A | 0xEA(d) | ... | 0xEA(s) | ... | 0xDD | 0xDD | 0xDD | 0xDD |
| `bbc.q <ea(s)>, <ea(d)>, #<D>` | 0x4B | 0xEA(d) | ... | 0xEA(s) | ... | 0xDD | 0xDD | 0xDD | 0xDD |

#### DBNZ

Decrement counter and branch if not zero

        <ea> - 1 -> <ea>; if (<ea> == 0) then pc + d -> pc

`dbnz <ea>, <#<D>|label>`

* Effective address is accessed as 32-bit unsigned counter.

| Mnemonic | Bytecode | Ext 0 | ... | Ext N | Ext N+1 | Ext N+2 | Ext N+3 |
| - | - | - | - | - | - | - | - |
| `dbnz <ea>, #<D>` | 0x4C | 0xEA | ... | 0xDD | 0xDD | 0xDD | 0xDD |


