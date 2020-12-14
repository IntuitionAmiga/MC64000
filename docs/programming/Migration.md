## [Programming](./README.md) > Migration

The following page gives information on how to migrate User Model code from 680x0 to MC64K.

### General

* Be consistent in the use of register naming:
    - For convenience, you can use d0 ... d7 and a0 ... a7.
    - Consider changing to the universal r0 ... r15 names.
* Replace extended arithmetic:
    - If possible, use _.q_ operands.
* Rework conditional code:
    - Most 680x0 operations directly set the Condition Code Register
    - Make note of the destination of the last operation before any conditional branch as the CCR will be set based on that value.
    - MC64K only supports branching based on direct comparisons of operands or an operand against zero.

### Optimising

Until a JIT runtime exists, MC64K runs code _interpretively_. Long established rules for 680x0 code optimisation do not necessarily apply here:

* Every interpreted instruction has a significant fetch/decode cost:
    - This is typically the largest part of the execution time, actual operation performed is typically small.
    - Does not depend on the operation.
    - Does depend on the addressing modes in use.
* Effective Addresses are available for source and destination operands at the same time:
    - Fewer requirements to move operands into registers.
* Register direct operands are not intrinsically faster than indirect:
    - MC64K registers are still memory locations.
    - Access time improvements are still possible due to simpler address calculations.

The general rules for optimisation can be summerised as:
* Optimise for fewer instructions.
    - Instruction decode overhead dominates.
* Where there is a choice between additional instructions or a more complex addressing mode, prefer the addressing mode.
    - An exception to this rule is where the same complex address would be evaluated repeatedly.
    - A single evaluation to a register temporary and reuse of the temporary is still faster.
* Where a complex address mode is duplicated for both source and destination operands, the assembler will emit an optimised addressing mode for the source that re-uses the calculation for the destination.
    - Does not apply to pre/post increment/decrement modes due to the side effects of modifying the register.

### Examples

Consider the following contrived example in C:

```c

/**
 * @param float64*       dest  a0
 * @param const float64* src   a1
 * @param const uint32*  idx   a2
 * @param uint32         count d0
 * @return void
 */
void sum_indexed(float64* dest, const float64* src, const uint32* idx, uint32 count) {
    while (count--) {
        int i = *idx++;
        dest[i] += src[i];
    }
}
```

An unoptimised version of this code in 68000/882 may look like this:

```asm

; a0 float64*       dest
; a1 const float64* src
; a2 const uint32*  idx
; d0 uint32         count

_sum_indexed:
    tst.l   d0               ; beware zero count
    beq     .done
.loop:
    move.l  (a2)+, d1
    lsl.l   #3, d1           ; sizeof float64
    fmove.d (a0, d1.l), fp0
    fadd.d  (a1, d1.l), fp0  ; destination must be register
    fmove.d fp0, (a0, d1.l)
    sub.l   #1, d0           ; db<cc> only for 16 bit counters
    bne     .loop
.done
    rts
```
Using some 020+ scaled indexed addressing modes, one instruction can be saved:

```asm

; a0 float64*       dest
; a1 const float64* src
; a2 const uint32*  idx
; d0 uint32         count
_sum_indexed:
    tst.l   d0
    beq     .done
.loop:
    move.l  (a2)+, d1
    fmove.d (a0, d1.l * 8), fp0
    fadd.d  (a1, d1.l * 8), fp0
    fmove.d fp0, (a0, d1.l)
    sub.l   #1, d0
    bne     .loop
.done
    rts
```

For the MC64K, the equivalent code becomes:

```asm

; a0 float64*       dest
; a1 const float64* src
; a2 const uint32*  idx
; d0 uint32         count

_sum_indexed:
    biz.l  d0, .done                      ; branch if zero
.loop:
    move.l (a2)+, d0
    fadd.d (a1, d0.l * 8), (a0, d0.l * 8) ; dual wield EA modes
    dbnz   d0, .loop                      ; dbnz decrements uint32 to zero
.done:
    rts
```
