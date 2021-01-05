## [Documentation](../README.md) > [Bytecode Format](./README.md) > [Instruction Layout](./Instructions.md) > Data Movement Group

The bytecode formats for the suppored data movement and conversion instructions are documented here.

One byte is used for the Instruction Opcode:

* Opcodes without any operands are 1 byte.
* Opcodes that have operands place them in the following order:
    - Destination [Effective Address](EffectiveAddress.md).
    - Source [Effective Address](EffectiveAddress.md).
    - Branch displacements.
    - Other data.

___

### MOVE

Move data from source to destination

        <ea(s)> -> <ea(d)>

General syntax:

        move.<b|w|l|q> <ea(s)>, <ea(d)>

| Mnemonic | Bytecode | Ext 0 | ... | ... | ... |
| - | - | - | - | - | - |
| `move.b <ea(s)>, <ea(d)>` | 0x4D | 0x*EA*(d) | ... | 0x*EA*(s) | ... |
| `move.w <ea(s)>, <ea(d)>` | 0x4E | 0x*EA*(d) | ... | 0x*EA*(s) | ... |
| `move.l <ea(s)>, <ea(d)>` | 0x4F | 0x*EA*(d) | ... | 0x*EA*(s) | ... |
| `move.q <ea(s)>, <ea(d)>` | 0x50 | 0x*EA*(d) | ... | 0x*EA*(s) | ... |

___

### SAVEM

Save multiple registers

        register list -> <ea>


General syntax:

        savem <#<R>|register list>, <ea>

* Register list is 16-bit mask of saved registers. For each bit position, if the bit is set, the corresponding GPR is saved.
* Only register indirect pre/post inc/decrement Effective Address modes allowed.

| Mnemonic | Bytecode | Ext 0 | ... | Ext N | Ext N+1 |
| - | - | - | - | - | - |
| `savem #<R>, <ea>` | 0x51 | 0x*EA* | ... | 0x*RR* | 0x*RR* |

___

### LOADM

Load multiple registers

        <ea> -> register list

General syntax:

        loadm <ea>, <#<R>|register list>

* Register list is 16-bit mask of saved registers. For each bit position, if the bit is set, the corresponding GPR is loaded.
* Only register indirect pre/post inc/decrement Effective Address modes allowed.

| Mnemonic | Bytecode | Ext 0 | ... | Ext N | Ext N+1 |
| - | - | - | - | - | - |
| `loadm <ea>, #<R>` | 0x52 | 0x*EA* | ... | 0x*RR* | 0x*RR* |

___

### FMOVEB

Convert signed byte to floating point

        <ea(s)> -> <ea(d)>

General syntax:

        fmoveb.<s|d> <ea(s)>, <ea(d)>

* Operand size refers to destination.
* Source operand is interpreted as a signed 8-bit value and converted to floating point.

| Mnemonic | Bytecode | Ext 0 | ... | ... | ... |
| - | - | - | - | - | - |
| `fmoveb.s <ea(s)>, <ea(d)>` | 0x53 | 0x*EA*(d) | ... | 0x*EA*(s) | ... |
| `fmoveb.d <ea(s)>, <ea(d)>` | 0x54 | 0x*EA*(d) | ... | 0x*EA*(s) | ... |

___

### FMOVEW

Convert signed word to floating point

        <ea(s)> -> <ea(d)>

General syntax:

        fmovew.<s|d> <ea(s)>, <ea(d)>

* Operand size refers to destination.
* Source operand is interpreted as a signed 16-bit value and converted to floating point.

| Mnemonic | Bytecode | Ext 0 | ... | ... | ... |
| - | - | - | - | - | - |
| `fmovew.s <ea(s)>, <ea(d)>` | 0x55 | 0x*EA*(d) | ... | 0x*EA*(s) | ... |
| `fmovew.d <ea(s)>, <ea(d)>` | 0x56 | 0x*EA*(d) | ... | 0x*EA*(s) | ... |

___

### FMOVEL

Convert signed long to floating point

        <ea(s)> -> <ea(d)>

General syntax:

        fmovel.<s|d> <ea(s)>, <ea(d)>

* Operand size refers to destination.
* Source operand is interpreted as a signed 32-bit value and converted to floating point.
* Loss of precision occurs when destination is 32-bit float.

| Mnemonic | Bytecode | Ext 0 | ... | ... | ... |
| - | - | - | - | - | - |
| `fmovel.s <ea(s)>, <ea(d)>` | 0x57 | 0x*EA*(d) | ... | 0x*EA*(s) | ... |
| `fmovel.d <ea(s)>, <ea(d)>` | 0x58 | 0x*EA*(d) | ... | 0x*EA*(s) | ... |

___

### FMOVEQ

Convert signed quad to floating point

        <ea(s)> -> <ea(d)>

General syntax:

        fmoveq.<s|d> <ea(s)>, <ea(d)>

* Operand size refers to destination.
* Source operand is interpreted as a signed 64-bit value and converted to floating point.
* Loss of precision occurs.

| Mnemonic | Bytecode | Ext 0 | ... | ... | ... |
| - | - | - | - | - | - |
| `fmoveq.s <ea(s)>, <ea(d)>` | 0x59 | 0x*EA*(d) | ... | 0x*EA*(s) | ... |
| `fmoveq.d <ea(s)>, <ea(d)>` | 0x5A | 0x*EA*(d) | ... | 0x*EA*(s) | ... |

___

### FMOVES

Convert a single precision to other format

        <ea(s)> -> <ea(d)>

General syntax:

        fmoves.<l|q|d> <ea(s)>, <ea(d)>

* Operand size refers to destination.
* Source operand is interpreted as a 32-bit floating point value and converted to target format.
* Loss of precision occurs for long and quad target.

| Mnemonic | Bytecode | Ext 0 | ... | ... | ... |
| - | - | - | - | - | - |
| `fmoves.l <ea(s)>, <ea(d)>` | 0x5B | 0x*EA*(d) | ... | 0x*EA*(s) | ... |
| `fmoves.q <ea(s)>, <ea(d)>` | 0x5C | 0x*EA*(d) | ... | 0x*EA*(s) | ... |
| `fmoves.d <ea(s)>, <ea(d)>` | 0x5D | 0x*EA*(d) | ... | 0x*EA*(s) | ... |

___

### FMOVED

Convert a double precision to other format

        <ea(s)> -> <ea(d)>

General syntax:

        fmoved.<l|q|s> <ea(s)>, <ea(d)>

* Operand size refers to destination.
* Source operand is interpreted as a 64-bit floating point value and converted to target format.
* Loss of precision occurs.

| Mnemonic | Bytecode | Ext 0 | ... | ... | ... |
| - | - | - | - | - | - |
| `fmoved.l <ea(s)>, <ea(d)>` | 0x5E | 0x*EA*(d) | ... | 0x*EA*(s) | ... |
| `fmoved.q <ea(s)>, <ea(d)>` | 0x5F | 0x*EA*(d) | ... | 0x*EA*(s) | ... |
| `fmoved.s <ea(s)>, <ea(d)>` | 0x60 | 0x*EA*(d) | ... | 0x*EA*(s) | ... |

___

### FSAVEM

Save multiple floating point registers

        register list -> <ea>

General syntax:

        fsavem <#<R>|register list>, <ea>

* Register list is 16-bit mask of saved registers. For each bit position, if the bit is set, the corresponding FPR is saved.
* Only register indirect pre/post inc/decrement Effective Address modes allowed.

| Mnemonic | Bytecode | Ext 0 | ... | Ext N | Ext N+1 |
| - | - | - | - | - | - |
| `fsavem #<R>, <ea>` | 0x61 | 0x*EA* | ... | 0x*RR* | 0x*RR* |

___

### FLOADM

Load multiple floating point registers

        <ea> -> register list

General syntax:

        floadm <ea>, <#<R>|register list>

* Register list is 16-bit mask of saved registers. For each bit position, if the bit is set, the corresponding FPR is loaded.
* Only register indirect pre/post inc/decrement Effective Address modes allowed.

| Mnemonic | Bytecode | Ext 0 | ... | Ext N | Ext N+1 |
| - | - | - | - | - | - |
| `floadm <ea>, #<R>` | 0x62 | 0x*EA* | ... | 0x*RR* | 0x*RR* |

___

### CLR

Clear a location

        0 -> <ea>

General syntax:
        clr.<b|w|l|q> <ea>

* All bits are set to zero.

| Mnemonic | Bytecode | Ext 0 | ... |
| - | - | - | - |
| `clr.b <ea>`| 0x63 | 0x*EA* | ... |
| `clr.w <ea>`| 0x64 | 0x*EA* | ... |
| `clr.l <ea>`| 0x65 | 0x*EA* | ... |
| `clr.q <ea>`| 0x66 | 0x*EA* | ... |

___

### EXG

Exchange GPR

        r<S> <-> r<D>

General syntax:

        exg r<S>, r<D>

* General Purpose Register contents are exchanged.

| Mnemonic | Bytecode | Ext 0 |
| - | - | - |
| `exg <S>, r<D>`| 0x67 | 0x*SD* |

___

### FEXG

Exchange FPR

        fp<S> <-> fp<D>

`fexg fp<S>, fp<D>`

* Floating Point Register contents are exchanged.

| Mnemonic | Bytecode | Ext 0 |
| - | - | - |
| `fexg fp<S>, fp<D>`| 0x68 | 0x*SD* |

___

### SWAP

Swap register fragments

        r<N>[31:16] <-> r<N>[15:0]
        r<N>[31:24, 23:16, 15:8, 7:0] <-> r<N>[7:0, 15:8, 23:16, 31:24]
        r<N>[63:56, 55:48, ... 15:8, 7:0] <-> r<N>[7:0, 15:8, ... 55:48, 63:46]

General syntax:

        swap[.<l|q>] r<N>

* The unsized variant replicates the original 16-bit word swap of the 6800x0.
* The .l variant performs a 32-bit byteswap of the lower half of the register.
* The .q variant performs a 64-bit byteswap of the entire register.

| Mnemonic | Bytecode | Ext 0 |
| - | - | - |
| `swap r<N>`| 0x69 | 0x0*N* |
| `swap.l r<N>` | 0x6A | 0x0*N* |
| `swap.q r<N>` | 0x6B | 0x0*N* |

___

### LINK

Link and Allocate

        sp - 8 -> sp; r<N> -> (sp)
        sp -> r<N>; sp + d -> sp

General syntax:

        link r<N>, #<D>

* Displacement should be negative to allocate stack space.

| Mnemonic | Bytecode | Ext 0 | Ext 1 | Ext 2 | Ext 3 | Ext 4 |
| - | - | - | - | - | - | - |
| `link r<N>, #<D>`| 0x6C | 0x0*N* | 0x*DD* | 0x*DD* | 0x*DD* | 0x*DD* |

___

### UNLK

Unlink

        r<N> -> sp; (sp) -> r<N>; sp + 8 -> sp

General syntax:

        unlk <rN>

| Mnemonic | Bytecode | Ext 0 |
| - | - | - |
| `unlk r<N>`| 0x6D | 0x0*N* |

___

### LEA

Load Effective Address

        '<ea> -> r<N>'

General syntax:

        lea <ea>, r<N>

| Mnemonic | Bytecode | Ext 0 | Ext 1 | ... |
| - | - | - | - | - |
| `lea <ea>, r<N>`| 0x6E | 0x0*N* | 0x*EA* | ...|

___

### PEA

Push Effective Address

        sp - 8 -> sp; <ea> -> (sp)

General syntax:

        pea <ea>

| Mnemonic | Bytecode | Ext 0 | ... |
| - | - | - | - |
| `pea <ea>`| 0x6F | 0x*EA* | ...|
