## Bytecode Format > Instruction Layout

The bytecode formats for the suppored instructions are documented here.

One byte is used for the Instruction Opcode:

* Opcodes without any operands are 1 byte.
* Opcodes that have operands place them in the following order:
    - Destination [Effective Address](EffectiveAddress.md).
    - Source [Effective Address](EffectiveAddress.md)].
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

Unconditional Branch

`bra[.b] <#<D>|label>`

* Branch distance is measured from the end of the instructions.
    - e.g. `bra. #1` executes the next instruction.
* Short .b form requires the label to be within -128 ... 127 bytes of the current Program Counter.
* Standard form requires the lable to be within -2147483648 ... 2147483647 bytes of the current Program Counter.

| Mnemonic | Bytecode | Ext 0 | Ext 1 | Ext 2 | Ext 3 |
| - | - | - | - | - | - |
| `bra.b <#<D>|label>`| 0x01 | 0xDD |
| `bra <#<D>|label>` | 0x02 | 0xDD | 0xDD | 0xDD | 0xDD |

#### BSR

Branch to Subroutine

`bra[.b] <#<D>|label>`

* Branch distance is measured from the end of the instructions.
    - e.g. `bra. #1` executes the next instruction.
* Short .b form requires the label to be within -128 ... 127 bytes of the current Program Counter.
* Standard form requires the lable to be within -2147483648 ... 2147483647 bytes of the current Program Counter.

| Mnemonic | Bytecode | Ext 0 | Ext 1 | Ext 2 | Ext 3 |
| - | - | - | - | - | - |
| `bsr.b <label>`| 0x03 | 0xDD |
| `bsr <label>` | 0x04 | 0xDD | 0xDD | 0xDD | 0xDD |

