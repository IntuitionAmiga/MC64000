## Bytecode Format > [Instruction Layout](Instructions.md) > Data Movement Group

The bytecode formats for the suppored data movement and conversion instructions are documented here.

One byte is used for the Instruction Opcode:

* Opcodes without any operands are 1 byte.
* Opcodes that have operands place them in the following order:
    - Destination [Effective Address](EffectiveAddress.md).
    - Source [Effective Address](EffectiveAddress.md).
    - Branch displacements.
    - Other data.

### Data Movement

#### MOVE

Move data from source to destination

        <ea(s)> -> <ea(d)>

`move.<b|w|l|q> <ea(s)>, <ea(d)>`

| Mnemonic | Bytecode | Ext 0 | ... | ... | ... |
| - | - | - | - | - | - |
| `move.b <ea(s)>, <ea(d)>` | 0x4D | 0xEA(d) | ... | 0xEA(s) | ... |
| `move.w <ea(s)>, <ea(d)>` | 0x4E | 0xEA(d) | ... | 0xEA(s) | ... |
| `move.l <ea(s)>, <ea(d)>` | 0x4F | 0xEA(d) | ... | 0xEA(s) | ... |
| `move.q <ea(s)>, <ea(d)>` | 0x50 | 0xEA(d) | ... | 0xEA(s) | ... |

#### SAVEM

Save multiple registers

        register list -> <ea>

`savem <#<R>|register list>, <ea>`

* Register list is 16-bit mask of saved registers. For each bit position, if the bit is set, the corresponding GPR is saved.
* Only register indirect pre/post inc/decrement Effective Address modes allowed.

| Mnemonic | Bytecode | Ext 0 | ... | Ext N | Ext N+1 |
| - | - | - | - | - | - |
| `savem #<R>, <ea>` | 0x51 | 0xEA | ... | 0xRR | 0xRR |

#### LOADM

Load multiple registers

        <ea> -> register list

`loadm <ea>, <#<R>|register list>`

* Register list is 16-bit mask of saved registers. For each bit position, if the bit is set, the corresponding GPR is loaded.
* Only register indirect pre/post inc/decrement Effective Address modes allowed.

| Mnemonic | Bytecode | Ext 0 | ... | Ext N | Ext N+1 |
| - | - | - | - | - | - |
| `loadm <ea>, #<R>` | 0x52 | 0xEA | ... | 0xRR | 0xRR |

#### FMOVEB

Convert signed byte to floating point

        <ea(s)> -> <ea(d)>

`fmoveb.<s|d> <ea(s)>, <ea(d)>`

* Operand size refers to destination.
* Source operand is interpreted as a signed 8-bit value and converted to floating point.

| Mnemonic | Bytecode | Ext 0 | ... | ... | ... |
| - | - | - | - | - | - |
| `fmoveb.s <ea(s)>, <ea(d)>` | 0x53 | 0xEA(d) | ... | 0xEA(s) | ... |
| `fmoveb.d <ea(s)>, <ea(d)>` | 0x54 | 0xEA(d) | ... | 0xEA(s) | ... |

#### FMOVEW

Convert signed word to floating point

        <ea(s)> -> <ea(d)>

`fmovew.<s|d> <ea(s)>, <ea(d)>`

* Operand size refers to destination.
* Source operand is interpreted as a signed 16-bit value and converted to floating point.

| Mnemonic | Bytecode | Ext 0 | ... | ... | ... |
| - | - | - | - | - | - |
| `fmovew.s <ea(s)>, <ea(d)>` | 0x55 | 0xEA(d) | ... | 0xEA(s) | ... |
| `fmovew.d <ea(s)>, <ea(d)>` | 0x56 | 0xEA(d) | ... | 0xEA(s) | ... |


#### FMOVEL

Convert signed long to floating point

        <ea(s)> -> <ea(d)>

`fmovel.<s|d> <ea(s)>, <ea(d)>`

* Operand size refers to destination.
* Source operand is interpreted as a signed 32-bit value and converted to floating point.
* Loss of precision occurs when destination is 32-bit float.

| Mnemonic | Bytecode | Ext 0 | ... | ... | ... |
| - | - | - | - | - | - |
| `fmovel.s <ea(s)>, <ea(d)>` | 0x57 | 0xEA(d) | ... | 0xEA(s) | ... |
| `fmovel.d <ea(s)>, <ea(d)>` | 0x58 | 0xEA(d) | ... | 0xEA(s) | ... |

#### FMOVEQ

Convert signed quad to floating point

        <ea(s)> -> <ea(d)>

`fmoveq.<s|d> <ea(s)>, <ea(d)>`

* Operand size refers to destination.
* Source operand is interpreted as a signed 64-bit value and converted to floating point.
* Loss of precision occurs.

| Mnemonic | Bytecode | Ext 0 | ... | ... | ... |
| - | - | - | - | - | - |
| `fmoveq.s <ea(s)>, <ea(d)>` | 0x59 | 0xEA(d) | ... | 0xEA(s) | ... |
| `fmoveq.d <ea(s)>, <ea(d)>` | 0x5A | 0xEA(d) | ... | 0xEA(s) | ... |


#### FMOVES

Convert a single precision to other format

        <ea(s)> -> <ea(d)>

`fmoves.<l|q|d> <ea(s)>, <ea(d)>`

* Operand size refers to destination.
* Source operand is interpreted as a 32-bit floating point value and converted to target format.
* Loss of precision occurs for long and quad target.

| Mnemonic | Bytecode | Ext 0 | ... | ... | ... |
| - | - | - | - | - | - |
| `fmoves.l <ea(s)>, <ea(d)>` | 0x5B | 0xEA(d) | ... | 0xEA(s) | ... |
| `fmoves.q <ea(s)>, <ea(d)>` | 0x5C | 0xEA(d) | ... | 0xEA(s) | ... |
| `fmoves.d <ea(s)>, <ea(d)>` | 0x5D | 0xEA(d) | ... | 0xEA(s) | ... |

#### FMOVED

Convert a double precision to other format

        <ea(s)> -> <ea(d)>

`fmoved.<l|q|s> <ea(s)>, <ea(d)>`

* Operand size refers to destination.
* Source operand is interpreted as a 64-bit floating point value and converted to target format.
* Loss of precision occurs.

| Mnemonic | Bytecode | Ext 0 | ... | ... | ... |
| - | - | - | - | - | - |
| `fmoved.l <ea(s)>, <ea(d)>` | 0x5E | 0xEA(d) | ... | 0xEA(s) | ... |
| `fmoved.q <ea(s)>, <ea(d)>` | 0x5F | 0xEA(d) | ... | 0xEA(s) | ... |
| `fmoved.s <ea(s)>, <ea(d)>` | 0x60 | 0xEA(d) | ... | 0xEA(s) | ... |

#### FSAVEM

Save multiple floating point registers

        register list -> <ea>

`fsavem <#<R>|register list>, <ea>,`

* Register list is 16-bit mask of saved registers. For each bit position, if the bit is set, the corresponding FPR is saved.
* Only register indirect pre/post inc/decrement Effective Address modes allowed.

| Mnemonic | Bytecode | Ext 0 | ... | Ext N | Ext N+1 |
| - | - | - | - | - | - |
| `fsavem #<R>, <ea>` | 0x61 | 0xEA | ... | 0xRR | 0xRR |

#### FLOADM

Load multiple floating point registers

        <ea> -> register list

`floadm <ea>, <#<R>|register list>`

* Register list is 16-bit mask of saved registers. For each bit position, if the bit is set, the corresponding FPR is loaded.
* Only register indirect pre/post inc/decrement Effective Address modes allowed.

| Mnemonic | Bytecode | Ext 0 | ... | Ext N | Ext N+1 |
| - | - | - | - | - | - |
| `floadm <ea>, #<R>` | 0x62 | 0xEA | ... | 0xRR | 0xRR |


#### CLR

Clear a location

        0 -> <ea>

`clr.<b|w|l|q> <ea>`

* All bits are set to zero.

| Mnemonic | Bytecode | Ext 0 | ... |
| - | - | - | - |
| `clr.b <ea>`| 0x63 | 0xEA | ... |
| `clr.w <ea>`| 0x64 | 0xEA | ... |
| `clr.l <ea>`| 0x65 | 0xEA | ... |
| `clr.q <ea>`| 0x66 | 0xEA | ... |

#### EXG

Exchange GPR

        r<D> <-> r<S>

`exg r<S>, r<D>`

* General Purpose Register contents are exchanged.

| Mnemonic | Bytecode | Ext 0 |
| - | - | - |
| `exg <S>, r<D>`| 0x67 | 0x(S)(D) |

#### FEXG

Exchange FPR

        fp<D> <-> fp<S>

`fexg fp<S>, fp<D>`

* Floating Point Register contents are exchanged.

| Mnemonic | Bytecode | Ext 0 |
| - | - | - |
| `fexg fp<S>, fp<D>`| 0x68 | 0x(S)(D) |


#### SWAP

Swap register fragments

        r<N>[31:16] <-> r<N>[15:0]
        r<N>[63:32] <-> r<N>[31:0]

`swap[.l] r<N>`

* By default, only the lower 32-bits of the register are affected swapping bits 31:16 are with bits 15:0
* The .l variant affects the full register, swapping bits 63:32 with bits 31:0

| Mnemonic | Bytecode | Ext 0 |
| - | - | - |
| `swap r<N>`| 0x69 | 0x0(N) |
| `swap.l r<N>` | 0x6A | 0x0(N) |

#### LINK

Link and Allocate

        sp - 8 -> sp; r<N> -> (sp)
        sp -> r<N>; sp + d -> sp

`link r<N>, #<D>`

* Displacement should be negative to allocate stack space.

| Mnemonic | Bytecode | Ext 0 | Ext 1 | Ext 2 | Ext 3 | Ext 4 |
| - | - | - | - | - | - | - |
| `link r<N>, #<D>`| 0x6B | 0x0(N) | 0xDD | 0xDD | 0xDD | 0xDD |


#### UNLK

Unlink

        r<N> -> sp; (sp) -> r<N>; sp + 8 -> sp

`unlk rN>`

| Mnemonic | Bytecode | Ext 0 |
| - | - | - |
| `unlk r<N>`| 0x6C | 0x0(N) |
