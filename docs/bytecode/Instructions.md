[Documentation](../README.md) > [Bytecode Format](./README.md)

## Instruction Layout

Instructions are tokenised into a simple bytecode format that can be read and processed linearly by the interpreter:

* The first byte encodes the operation to perform.
    - Operand size is not explicitly encoded, rather each size variation of an operation is represented by a different opcode byte.
    - Based on the operation, there may be implied additional parameters.
* If the operation has a single operand, the next byte encodes the effective address mode for that operand.
    - If the operand has additional parameters, e.g. a literal value or displacement, it is encoded into the next _N_ bytes.
* If the operation has two operands, the next byte encodes the effective address for the that operand.
    - If the operand has additional parameters, e.g. a literal value or displacement, it is encoded into the next _N_ bytes.
    - Generally the second operand is considered to be the source operand for any dyadic operation.
* If the operation has any additional parameter data, e.g. a branch displacement, register mask, etc. it is encoded into the next _N_ bytes.

The key benefits of this layout are:

* Simple lookup for operation/size.
* Destination EA first:
    - Allows trivial implementation of "assign operate" behaviour.
    - Allows a shortcut EA for source when the source EA is the same as the destination and has no special side effects.
* Branch displacement last allows for a consistent measure of displacement _from the end of the instruction_, i.e. a displacement of 0 being the location of the _next_ instruction.

### Examples

A consequence of the design is that instruction lengths can vary significantly. The shortest instructions are a single byte, while the longest can be as many as 21. Some examples are:

| Size | Example | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 | 11 | 12 | 13 | 14 | 15 | 16 | 17 | 18 | 19 | 20 |
| - | - | - | - | - | - | - | - | - | - | - | - | - | - | - | - | - | - | - | - | - | - | - |
| 1 | `ret` | RET | . | . | . | . | . | . | . | . | . | . | . | . | . | . | . | . | . | . | . |. |
| 2 | `clr.l (a0)+` | CLR_L | R8_IND_POST_INC | . | . | . | . | . | . | . | . | . | . | . | . | . | . | . | . | . | . | . |
| 3 | `add.w d0, d1` | ADD_W | R1_DIR | R0_DIR | . | . | . | . | . | . | . | . | . | . | . | . | . | . | . | . | . | . |
| 5 | `bra label` | BRA | *l0* | *l1* | *l2* | *l3* | . | . | . | . | . | . | . | . | . | . | . | . | . | . | . | . |
| 6 | `biz.q (a0), label` | BMC | IEQ_Q | R8_IND | *l0*  | *l1* | *l2*  | *l3* | . | . | . | . | . | . | . | . | . | . | . | . | . | . |
| 7 | `bnz.l $ABCD(a0), label` | BMC | INE_L | R8_IND_DSP | 0xCD | 0xAB | 0x00 | 0x00 | *l0*  | *l1* | *l2*  | *l3* | . | . | . | . | . | . | . | . | . | . |
| 21 | `fbgt.d #PI, $ABCD(r8, r1.w*8), label` | BDC | FGT_D | REG_IND_IDXQ_8_DSP | 8:1 | 0xCD | 0xAB | 0x00 | 0x00 | FLT_IMM_DOUBLE | *PI* | *PI* | *PI* | *PI* | *PI* | *PI* | *PI* | *PI* | *l0*  | *l1* | *l2* | *l3* |

### Fast Path Instructions

Most of the commonly used arithmetic and logical instructions tend to be dyadic operations on register operands. With the standard encoding scheme these require 3 bytes, as per the example in the table above. At runtime, the destination and source operands are decoded individually and there is marginal gain from the operand being in a register. As the MC64K design has 16 general purpose and 16 floating point registers, an alternative encoding is provided that aims to allow faster runtime execution of register to register operations for selected instructions by encoding them into 2 bytes:

* The first byte contains the alternative opcode for the fast path instruction variant.
    - A selection of 32 and 64 bit arithmetic, logical and movement operations are available.
* The next byte contains the source and destination register pair.
    - The destination register number is encoded in the lower nybble.
    - The source register number is encoded in the upper nybble.

| Example | 0 | 1 |
| - | - | - |
| `add.q r2, r3` | R2R_ADD_Q | 2:3 |
| `fmul.d fp5, fp7` | R2R_FMUL_D | 5:7 |

This alternative encoding allows the regular EA decode cycle to be skipped entirely and the required register pair locations extracted directly, offering significant performance gains.

### Reference

The bytecode layouts for the suppored instructions are documented here. Instructions are broken down into the following logical groups:

* [Control Group](./InstructionsControl.md)
* [Data Movement Group](./InstructionsDataMove.md)
* [Logical Group](./InstructionsLogical.md)
* [Arithmetic Group](./InstructionsArithmetic.md)

