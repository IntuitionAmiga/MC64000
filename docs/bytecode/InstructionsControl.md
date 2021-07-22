## [Documentation](../README.md) > [Bytecode Format](./README.md) > [Instruction Layout](./Instructions.md) > Control Group

The bytecode formats for the suppored control and flow instructions are documented here.

One byte is used for the Instruction Opcode:

* Opcodes without any operands are 1 byte.
* Opcodes that have operands place them in the following order:
    - Destination [Effective Address](EffectiveAddress.md).
    - Source [Effective Address](EffectiveAddress.md).
    - Branch displacements.
    - Other data.

___

### HCF

Halt and Call Function / Catch Fire.

General syntax:

        hcf <#N>

* Halts the interpreter and passes control back to the host environment.
* Extension byte 1 must be 0xFF or this opcode be interpreted as Halt and Cach Fire.
* N is the 8 bit index of a host native function to call. The host can therefore expose a maximum of 256 directly accessible entry points to the interpreter. What these do depends entirely on the host application.
* If D is not a recognised host function ID, the interpreter will halt with Unknown Host Call.

| Mnemonic | Bytecode | Ext 1 | Ext 2 |
| - | - | - | - | - | - |
| `hcf #<N>`| 0x00 | 0xFF | 0x*DD* |


___

### BRA

Branch.

        pc + d -> pc

General syntax:

        bra[.b] <#<D>|label>

* Branch distance is measured from the end of the instruction.
    - e.g. `bra #0` would theoretically branch to the next instruction.
    - Zero displacement is reserved for the unresolved forwards reference during assembly.
    - Negative displacements must be larger than the complete branch instruction.
* Short .b form requires the label to resolve to a displacement within -128 ... 127 bytes of the current Program Counter.
* Standard form requires the label to resolve to a displacement within -2147483648 ... 2147483647 bytes of the current Program Counter.

| Mnemonic | Bytecode | Ext 0 | Ext 1 | Ext 2 | Ext 3 |
| - | - | - | - | - | - |
| `bra.b <#D>`| 0x01 | 0x*DD* |
| `bra <#D>` | 0x02 | 0x*DD* | 0x*DD* | 0x*DD* | 0x*DD* |

___

### BSR

Branch to Subroutine.

        sp - 8 -> sp; pc -> (sp); pc + d -> pc

General syntax:

        bsr[.b] <#<D>|label>

* Branch distance is measured from the end of the instruction.
    - e.g. `bsr #0` would theoretically branch to the next instruction.
    - Zero displacement is reserved for the unresolved forwards reference during assembly.
    - Negative displacements must be larger than the complete branch instruction.
* Short .b form requires the label to resolve to a displacement within -128 ... 127 bytes of the current Program Counter.
* Standard form requires the label to resolve to a displacement within -2147483648 ... 2147483647 bytes of the current Program Counter.

| Mnemonic | Bytecode | Ext 0 | Ext 1 | Ext 2 | Ext 3 |
| - | - | - | - | - | - |
| `bsr.b #<D>`| 0x03 | 0x*DD* |
| `bsr #<D>` | 0x04 | 0x*DD* | 0x*DD* | 0x*DD* | 0x*DD* |

___

### JMP

Jump.

        <ea> -> pc

General syntax:

        jmp <ea>

* Effective Address cannot be register direct or integer immediate.

| Mnemonic | Bytecode | Ext 0 | ... |
| - | - | - | - |
| `jmp <ea>`| 0x05 | 0x*EA* | ... |

___

### JSR

Jump to Subroutine.

        sp - 8 -> sp; pc -> (sp); <ea> -> sp

General syntax:

        jsr <ea>

* Effective Address cannot be register direct or integer immediate.

| Mnemonic | Bytecode | Ext 0 | ... |
| - | - | - | - |
| `jsr <ea>`| 0x06 | 0x*EA* | ... |

___

### RTS

Return from Subroutine.

        (sp) -> pc; sp + 8 -> sp

General syntax:

        rts

| Mnemonic | Bytecode |
| - | - |
| `rts`| 0x07 |

___

### BIZ

Branch if operand is zero.

        if (<ea> == 0) then pc + d -> pc

General syntax:

        biz.<b|w|l|q> <ea>, <#<D>|label>
        fbiz.<s|d> <ea>, <#<D>|label>

* Branch distance is measured from the end of the instruction.
    - e.g. `biz.l r0, #0` would theoretically branch to the next instruction.
    - Zero displacement is reserved for the unresolved forwards reference during assembly.
    - Negative displacements must be larger than the complete branch instruction.

| Mnemonic | Bytecode | Ext 0 | ... | Ext N | Ext N+1 | Ext N+2 | Ext N+3 |
| - | - | - | - | - | - | - | - |
| `biz.b <ea>, #<D>` | 0x08 | 0x*EA* | ... | 0x*DD* | 0x*DD* | 0x*DD* | 0x*DD* |
| `biz.w <ea>, #<D>` | 0x09 | 0x*EA* | ... | 0x*DD* | 0x*DD* | 0x*DD* | 0x*DD* |
| `biz.l <ea>, #<D>` | 0x0A | 0x*EA* | ... | 0x*DD* | 0x*DD* | 0x*DD* | 0x*DD* |
| `biz.q <ea>, #<D>` | 0x0B | 0x*EA* | ... | 0x*DD* | 0x*DD* | 0x*DD* | 0x*DD* |
| `fbiz.s <ea>, #<D>` | 0x0C | 0x*EA* | ... | 0x*DD* | 0x*DD* | 0x*DD* | 0x*DD* |
| `fbiz.d <ea>, #<D>` | 0x0D | 0x*EA* | ... | 0x*DD* | 0x*DD* | 0x*DD* | 0x*DD* |

___

### BNZ

Branch if operand is not zero.

        if (<ea> != 0) then pc + d -> pc

General syntax:

        bnz.<b|w|l|q> <ea>, <#<D>|label>
        fbnz.<s|d> <ea>, <#<D>|label>

* Branch distance is measured from the end of the instruction.
    - e.g. `bnz.l r0, #0` would theoretically branch to the next instruction.
    - Zero displacement is reserved for the unresolved forwards reference during assembly.
    - Negative displacements must be larger than the complete branch instruction.

| Mnemonic | Bytecode | Ext 0 | ... | Ext N | Ext N+1 | Ext N+2 | Ext N+3 |
| - | - | - | - | - | - | - | - |
| `bnz.b <ea>, #<D>` | 0x0E | 0x*EA* | ... | 0x*DD* | 0x*DD* | 0x*DD* | 0x*DD* |
| `bnz.w <ea>, #<D>` | 0x0F | 0x*EA* | ... | 0x*DD* | 0x*DD* | 0x*DD* | 0x*DD* |
| `bnz.l <ea>, #<D>` | 0x10 | 0x*EA* | ... | 0x*DD* | 0x*DD* | 0x*DD* | 0x*DD* |
| `bnz.q <ea>, #<D>` | 0x11 | 0x*EA* | ... | 0x*DD* | 0x*DD* | 0x*DD* | 0x*DD* |
| `fbnz.s <ea>, #<D>` | 0x12 | 0x*EA* | ... | 0x*DD* | 0x*DD* | 0x*DD* | 0x*DD* |
| `fbnz.d <ea>, #<D>` | 0x13 | 0x*EA* | ... | 0x*DD* | 0x*DD* | 0x*DD* | 0x*DD* |

___

### BMI

Branch if operand is minus.

        if (<ea> < 0) then pc + d -> pc

General syntax:

        bmi.<b|w|l|q> <ea>, <#<D>|label>
        fbmi.<s|d> <ea>, <#<D>|label>

* Branch distance is measured from the end of the instruction.
    - e.g. `bmi.l r0, #0` would theoretically branch to the next instruction.
    - Zero displacement is reserved for the unresolved forwards reference during assembly.
    - Negative displacements must be larger than the complete branch instruction.

| Mnemonic | Bytecode | Ext 0 | ... | Ext N | Ext N+1 | Ext N+2 | Ext N+3 |
| - | - | - | - | - | - | - | - |
| `bmi.b <ea>, #<D>` | 0x14 | 0x*EA* | ... | 0x*DD* | 0x*DD* | 0x*DD* | 0x*DD* |
| `bmi.w <ea>, #<D>` | 0x15 | 0x*EA* | ... | 0x*DD* | 0x*DD* | 0x*DD* | 0x*DD* |
| `bmi.l <ea>, #<D>` | 0x16 | 0x*EA* | ... | 0x*DD* | 0x*DD* | 0x*DD* | 0x*DD* |
| `bmi.q <ea>, #<D>` | 0x17 | 0x*EA* | ... | 0x*DD* | 0x*DD* | 0x*DD* | 0x*DD* |
| `fbmi.s <ea>, #<D>` | 0x18 | 0x*EA* | ... | 0x*DD* | 0x*DD* | 0x*DD* | 0x*DD* |
| `fbmi.d <ea>, #<D>` | 0x19 | 0x*EA* | ... | 0x*DD* | 0x*DD* | 0x*DD* | 0x*DD* |

___

### BPL

Branch if operand is plus.

        if (<ea> > 0) then pc + d -> pc

General syntax:

        bpl.<b|w|l|q> <ea>, <#<D>|label>
        fbpl.<s|d> <ea>, <#<D>|label>

* Branch distance is measured from the end of the instruction.
    - e.g. `bpl.l r0, #0` would theoretically branch to the next instruction.
    - Zero displacement is reserved for the unresolved forwards reference during assembly.
    - Negative displacements must be larger than the complete branch instruction.

| Mnemonic | Bytecode | Ext 0 | ... | Ext N | Ext N+1 | Ext N+2 | Ext N+3 |
| - | - | - | - | - | - | - | - |
| `bpl.b <ea>, #<D>` | 0x1A | 0x*EA* | ... | 0x*DD* | 0x*DD* | 0x*DD* | 0x*DD* |
| `bpl.w <ea>, #<D>` | 0x1B | 0x*EA* | ... | 0x*DD* | 0x*DD* | 0x*DD* | 0x*DD* |
| `bpl.l <ea>, #<D>` | 0x1C | 0x*EA* | ... | 0x*DD* | 0x*DD* | 0x*DD* | 0x*DD* |
| `bpl.q <ea>, #<D>` | 0x1D | 0x*EA* | ... | 0x*DD* | 0x*DD* | 0x*DD* | 0x*DD* |
| `fbpl.s <ea>, #<D>` | 0x1E | 0x*EA* | ... | 0x*DD* | 0x*DD* | 0x*DD* | 0x*DD* |
| `fbpl.d <ea>, #<D>` | 0x1F | 0x*EA* | ... | 0x*DD* | 0x*DD* | 0x*DD* | 0x*DD* |

___

### BLT

Branch if source operand is less than destination operand.

        if (<ea(s)> < <ea(d)>) then pc + d -> pc

General syntax:

        blt.<b|w|l|q> <ea(s)>, <ea(d)> <#<D>|label>
        fblt.<s|d> <ea(s)>, <ea(d)>, <#<D>|label>

* Branch distance is measured from the end of the instruction.
    - e.g. `blt.l r0, r1, #0` would theoretically branch to the next instruction.
    - Zero displacement is reserved for the unresolved forwards reference during assembly.
    - Negative displacements must be larger than the complete branch instruction.

| Mnemonic | Bytecode | Ext 0 | ... | ... | ... | Ext N | Ext N+1 | Ext N+2 | Ext N+3 |
| - | - | - | - | - | - | - | - | - | - |
| `blt.b <ea(s)>, <ea(d)>, #<D>` | 0x20 | 0x*EA*(d) | ... | 0x*EA*(s) | ... | 0x*DD* | 0x*DD* | 0x*DD* | 0x*DD* |
| `blt.w <ea(s)>, <ea(d)>, #<D>` | 0x21 | 0x*EA*(d) | ... | 0x*EA*(s) | ... | 0x*DD* | 0x*DD* | 0x*DD* | 0x*DD* |
| `blt.l <ea(s)>, <ea(d)>, #<D>` | 0x22 | 0x*EA*(d) | ... | 0x*EA*(s) | ... | 0x*DD* | 0x*DD* | 0x*DD* | 0x*DD* |
| `blt.q <ea(s)>, <ea(d)>, #<D>` | 0x23 | 0x*EA*(d) | ... | 0x*EA*(s) | ... | 0x*DD* | 0x*DD* | 0x*DD* | 0x*DD* |
| `fblt.s <ea(s)>, <ea(d)>, #<D>` | 0x24 | 0x*EA*(d) | ... | 0x*EA*(s) | ... | 0x*DD* | 0x*DD* | 0x*DD* | 0x*DD* |
| `fblt.d <ea(s)>, <ea(d)>, #<D>` | 0x25 | 0x*EA*(d) | ... | 0x*EA*(s) | ... | 0x*DD* | 0x*DD* | 0x*DD* | 0x*DD* |

___

### BLE

Branch if source operand is less than or equal to destination operand.

        if (<ea(s)> <= <ea(d)>) then pc + d -> pc

General syntax:

        ble.<b|w|l|q> <ea(s)>, <ea(d)> <#<D>|label>
        fble.<s|d> <ea(s)>, <ea(d)>, <#<D>|label>

* Branch distance is measured from the end of the instruction.
    - e.g. `ble.l r0, r1, #0` would theoretically branch to the next instruction.
    - Zero displacement is reserved for the unresolved forwards reference during assembly.
    - Negative displacements must be larger than the complete branch instruction.

| Mnemonic | Bytecode | Ext 0 | ... | ... | ... | Ext N | Ext N+1 | Ext N+2 | Ext N+3 |
| - | - | - | - | - | - | - | - | - | - |
| `ble.b <ea(s)>, <ea(d)>, #<D>` | 0x26 | 0x*EA*(d) | ... | 0x*EA*(s) | ... | 0x*DD* | 0x*DD* | 0x*DD* | 0x*DD* |
| `ble.w <ea(s)>, <ea(d)>, #<D>` | 0x27 | 0x*EA*(d) | ... | 0x*EA*(s) | ... | 0x*DD* | 0x*DD* | 0x*DD* | 0x*DD* |
| `ble.l <ea(s)>, <ea(d)>, #<D>` | 0x28 | 0x*EA*(d) | ... | 0x*EA*(s) | ... | 0x*DD* | 0x*DD* | 0x*DD* | 0x*DD* |
| `ble.q <ea(s)>, <ea(d)>, #<D>` | 0x29 | 0x*EA*(d) | ... | 0x*EA*(s) | ... | 0x*DD* | 0x*DD* | 0x*DD* | 0x*DD* |
| `fble.s <ea(s)>, <ea(d)>, #<D>` | 0x2A | 0x*EA*(d) | ... | 0x*EA*(s) | ... | 0x*DD* | 0x*DD* | 0x*DD* | 0x*DD* |
| `fble.d <ea(s)>, <ea(d)>, #<D>` | 0x2B | 0x*EA*(d) | ... | 0x*EA*(s) | ... | 0x*DD* | 0x*DD* | 0x*DD* | 0x*DD* |

___

### BEQ

Branch if source operand is equal to destination operand.

        if (<ea(s)> == <ea(d)>) then pc + d -> pc

General syntax:

        beq.<b|w|l|q> <ea(s)>, <ea(d)> <#<D>|label>
        fbeq.<s|d> <ea(s)>, <ea(d)>, <#<D>|label>

* Branch distance is measured from the end of the instruction.
    - e.g. `beq.l r0, r1, #0` would theoretically branch to the next instruction.
    - Zero displacement is reserved for the unresolved forwards reference during assembly.
    - Negative displacements must be larger than the complete branch instruction.

| Mnemonic | Bytecode | Ext 0 | ... | ... | ... | Ext N | Ext N+1 | Ext N+2 | Ext N+3 |
| - | - | - | - | - | - | - | - | - | - |
| `beq.b <ea(s)>, <ea(d)>, #<D>` | 0x2C | 0x*EA*(d) | ... | 0x*EA*(s) | ... | 0x*DD* | 0x*DD* | 0x*DD* | 0x*DD* |
| `beq.w <ea(s)>, <ea(d)>, #<D>` | 0x2D | 0x*EA*(d) | ... | 0x*EA*(s) | ... | 0x*DD* | 0x*DD* | 0x*DD* | 0x*DD* |
| `beq.l <ea(s)>, <ea(d)>, #<D>` | 0x2E | 0x*EA*(d) | ... | 0x*EA*(s) | ... | 0x*DD* | 0x*DD* | 0x*DD* | 0x*DD* |
| `beq.q <ea(s)>, <ea(d)>, #<D>` | 0x2F | 0x*EA*(d) | ... | 0x*EA*(s) | ... | 0x*DD* | 0x*DD* | 0x*DD* | 0x*DD* |
| `fbeq.s <ea(s)>, <ea(d)>, #<D>` | 0x30 | 0x*EA*(d) | ... | 0x*EA*(s) | ... | 0x*DD* | 0x*DD* | 0x*DD* | 0x*DD* |
| `fbeq.d <ea(s)>, <ea(d)>, #<D>` | 0x31 | 0x*EA*(d) | ... | 0x*EA*(s) | ... | 0x*DD* | 0x*DD* | 0x*DD* | 0x*DD* |

___

### BGE

Branch if source operand is equal to or greater than destination operand.

        if (<ea(s)> >= <ea(d)>) then pc + d -> pc

General syntax:

        bge.<b|w|l|q> <ea(s)>, <ea(d)> <#<D>|label>
        fbge.<s|d> <ea(s)>, <ea(d)>, <#<D>|label>

* Branch distance is measured from the end of the instruction.
    - e.g. `bge.l r0, r1, #0` would theoretically branch to the next instruction.
    - Zero displacement is reserved for the unresolved forwards reference during assembly.
    - Negative displacements must be larger than the complete branch instruction.

| Mnemonic | Bytecode | Ext 0 | ... | ... | ... | Ext N | Ext N+1 | Ext N+2 | Ext N+3 |
| - | - | - | - | - | - | - | - | - | - |
| `bge.b <ea(s)>, <ea(d)>, #<D>` | 0x32 | 0x*EA*(d) | ... | 0x*EA*(s) | ... | 0x*DD* | 0x*DD* | 0x*DD* | 0x*DD* |
| `bge.w <ea(s)>, <ea(d)>, #<D>` | 0x33 | 0x*EA*(d) | ... | 0x*EA*(s) | ... | 0x*DD* | 0x*DD* | 0x*DD* | 0x*DD* |
| `bge.l <ea(s)>, <ea(d)>, #<D>` | 0x34 | 0x*EA*(d) | ... | 0x*EA*(s) | ... | 0x*DD* | 0x*DD* | 0x*DD* | 0x*DD* |
| `bge.q <ea(s)>, <ea(d)>, #<D>` | 0x35 | 0x*EA*(d) | ... | 0x*EA*(s) | ... | 0x*DD* | 0x*DD* | 0x*DD* | 0x*DD* |
| `fbge.s <ea(s)>, <ea(d)>, #<D>` | 0x36 | 0x*EA*(d) | ... | 0x*EA*(s) | ... | 0x*DD* | 0x*DD* | 0x*DD* | 0x*DD* |
| `fbge.d <ea(s)>, <ea(d)>, #<D>` | 0x37 | 0x*EA*(d) | ... | 0x*EA*(s) | ... | 0x*DD* | 0x*DD* | 0x*DD* | 0x*DD* |

___

### BGT

Branch if source operand is greater than destination operand.

        if (<ea(s)> > <ea(d)>) then pc + d -> pc

General syntax:

        bgt.<b|w|l|q> <ea(s)>, <ea(d)> <#<D>|label>
        fbgt.<s|d> <ea(s)>, <ea(d)>, <#<D>|label>

* Branch distance is measured from the end of the instruction.
    - e.g. `bgt.l r0, r1, #0` would theoretically branch to the next instruction.
    - Zero displacement is reserved for the unresolved forwards reference during assembly.
    - Negative displacements must be larger than the complete branch instruction.

| Mnemonic | Bytecode | Ext 0 | ... | ... | ... | Ext N | Ext N+1 | Ext N+2 | Ext N+3 |
| - | - | - | - | - | - | - | - | - | - |
| `bgt.b <ea(s)>, <ea(d)>, #<D>` | 0x38 | 0x*EA*(d) | ... | 0x*EA*(s) | ... | 0x*DD* | 0x*DD* | 0x*DD* | 0x*DD* |
| `bgt.w <ea(s)>, <ea(d)>, #<D>` | 0x39 | 0x*EA*(d) | ... | 0x*EA*(s) | ... | 0x*DD* | 0x*DD* | 0x*DD* | 0x*DD* |
| `bgt.l <ea(s)>, <ea(d)>, #<D>` | 0x3A | 0x*EA*(d) | ... | 0x*EA*(s) | ... | 0x*DD* | 0x*DD* | 0x*DD* | 0x*DD* |
| `bgt.q <ea(s)>, <ea(d)>, #<D>` | 0x3B | 0x*EA*(d) | ... | 0x*EA*(s) | ... | 0x*DD* | 0x*DD* | 0x*DD* | 0x*DD* |
| `fbgt.s <ea(s)>, <ea(d)>, #<D>` | 0x3C | 0x*EA*(d) | ... | 0x*EA*(s) | ... | 0x*DD* | 0x*DD* | 0x*DD* | 0x*DD* |
| `fbgt.d <ea(s)>, <ea(d)>, #<D>` | 0x3D | 0x*EA*(d) | ... | 0x*EA*(s) | ... | 0x*DD* | 0x*DD* | 0x*DD* | 0x*DD* |

___

### BNE

Branch if source operand is not equal to destination operand.

        if (<ea(s)> != <ea(d)>) then pc + d -> pc

General syntax:

        bne.<b|w|l|q> <ea(s)>, <ea(d)> <#<D>|label>
        fbne.<s|d> <ea(s)>, <ea(d)>, <#<D>|label>

* Branch distance is measured from the end of the instruction.
    - e.g. `bne.l r0, r1, #0` would theoretically branch to the next instruction.
    - Zero displacement is reserved for the unresolved forwards reference during assembly.
    - Negative displacements must be larger than the complete branch instruction.

| Mnemonic | Bytecode | Ext 0 | ... | ... | ... | Ext N | Ext N+1 | Ext N+2 | Ext N+3 |
| - | - | - | - | - | - | - | - | - | - |
| `bne.b <ea(s)>, <ea(d)>, #<D>` | 0x3E | 0x*EA*(d) | ... | 0x*EA*(s) | ... | 0x*DD* | 0x*DD* | 0x*DD* | 0x*DD* |
| `bne.w <ea(s)>, <ea(d)>, #<D>` | 0x3F | 0x*EA*(d) | ... | 0x*EA*(s) | ... | 0x*DD* | 0x*DD* | 0x*DD* | 0x*DD* |
| `bne.l <ea(s)>, <ea(d)>, #<D>` | 0x40 | 0x*EA*(d) | ... | 0x*EA*(s) | ... | 0x*DD* | 0x*DD* | 0x*DD* | 0x*DD* |
| `bne.q <ea(s)>, <ea(d)>, #<D>` | 0x41 | 0x*EA*(d) | ... | 0x*EA*(s) | ... | 0x*DD* | 0x*DD* | 0x*DD* | 0x*DD* |
| `fbne.s <ea(s)>, <ea(d)>, #<D>` | 0x42 | 0x*EA*(d) | ... | 0x*EA*(s) | ... | 0x*DD* | 0x*DD* | 0x*DD* | 0x*DD* |
| `fbne.d <ea(s)>, <ea(d)>, #<D>` | 0x43 | 0x*EA*(d) | ... | 0x*EA*(s) | ... | 0x*DD* | 0x*DD* | 0x*DD* | 0x*DD* |

___

### BBS

Branch if bit is set.

        if ( ((1 << <ea(s)>) & <ea(d)>) != 0 ) then pc + d -> pc

General syntax:

        bbs.<b|w|l|q> <ea(s)>, <ea(d)>

* Branch distance is measured from the end of the instruction.
    - e.g. `bbs.l r0, r1, #0` would theoretically branch to the next instruction.
    - Zero displacement is reserved for the unresolved forwards reference during assembly.
    - Negative displacements must be larger than the complete branch instruction.
* All possible integer Effective Address modes are valid for source and destination.
* Source value is accessed as a byte, modulo by the operation size.

| Mnemonic | Bytecode | Ext 0 | ... | ... | ... | Ext N | Ext N+1 | Ext N+2 | Ext N+3 |
| - | - | - | - | - | - | - | - | - | - |
| `bbs.b <ea(s)>, <ea(d)>, #<D>` | 0x44 | 0x*EA*(d) | ... | 0x*EA*(s) | ... | 0x*DD* | 0x*DD* | 0x*DD* | 0x*DD* |
| `bbs.w <ea(s)>, <ea(d)>, #<D>` | 0x45 | 0x*EA*(d) | ... | 0x*EA*(s) | ... | 0x*DD* | 0x*DD* | 0x*DD* | 0x*DD* |
| `bbs.l <ea(s)>, <ea(d)>, #<D>` | 0x46 | 0x*EA*(d) | ... | 0x*EA*(s) | ... | 0x*DD* | 0x*DD* | 0x*DD* | 0x*DD* |
| `bbs.q <ea(s)>, <ea(d)>, #<D>` | 0x47 | 0x*EA*(d) | ... | 0x*EA*(s) | ... | 0x*DD* | 0x*DD* | 0x*DD* | 0x*DD* |

___

### BBC

Branch if bit is clear.

        if ( ((1 << <ea(s)>) & <ea(d)>) == 0 ) then pc + d -> pc

General syntax:

        bbc.<b|w|l|q> <ea(s)>, <ea(d)>

* Branch distance is measured from the end of the instruction.
    - e.g. `bbc.l r0, r1, #0` would theoretically branch to the next instruction.
    - Zero displacement is reserved for the unresolved forwards reference during assembly.
    - Negative displacements must be larger than the complete branch instruction.
* All possible integer Effective Address modes are valid for source and destination.
* Source value is accessed as a byte, modulo by the operation size.

| Mnemonic | Bytecode | Ext 0 | ... | ... | ... | Ext N | Ext N+1 | Ext N+2 | Ext N+3 |
| - | - | - | - | - | - | - | - | - | - |
| `bbc.b <ea(s)>, <ea(d)>, #<D>` | 0x48 | 0x*EA*(d) | ... | 0x*EA*(s) | ... | 0x*DD* | 0x*DD* | 0x*DD* | 0x*DD* |
| `bbc.w <ea(s)>, <ea(d)>, #<D>` | 0x49 | 0x*EA*(d) | ... | 0x*EA*(s) | ... | 0x*DD* | 0x*DD* | 0x*DD* | 0x*DD* |
| `bbc.l <ea(s)>, <ea(d)>, #<D>` | 0x4A | 0x*EA*(d) | ... | 0x*EA*(s) | ... | 0x*DD* | 0x*DD* | 0x*DD* | 0x*DD* |
| `bbc.q <ea(s)>, <ea(d)>, #<D>` | 0x4B | 0x*EA*(d) | ... | 0x*EA*(s) | ... | 0x*DD* | 0x*DD* | 0x*DD* | 0x*DD* |

___

### DBNZ

Decrement counter and branch if not zero.

        <ea> - 1 -> <ea>; if (<ea> == 0) then pc + d -> pc

General syntax:

        dbnz <ea>, <#<D>|label>

* Branch distance is measured from the end of the instruction.
    - e.g. `dbnz r0, #0` would theoretically branch to the next instruction.
    - Zero displacement is reserved for the unresolved forwards reference during assembly.
    - Negative displacements must be larger than the complete branch instruction.
* Effective address is accessed as 32-bit unsigned counter.

| Mnemonic | Bytecode | Ext 0 | ... | Ext N | Ext N+1 | Ext N+2 | Ext N+3 |
| - | - | - | - | - | - | - | - |
| `dbnz <ea>, #<D>` | 0x4C | 0x*EA* | ... | 0x*DD* | 0x*DD* | 0x*DD* | 0x*DD* |
