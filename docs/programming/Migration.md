## Programming > Migration

The following page gives information on how to migrate user model code from 680x0 to MC64K.

### General

* Be consistent in the use of register naming:
    - For convenience, you can use d0 ... d7 and a0 ... a7
    - Consider changing to the universal r0 ... r15 names


### Optimising

Until a JIT runtime exists, MC64K runs code _interpretively_. Long established rules for 680x0 code optimisation do not necessarily apply here:

* Every interpreted instruction has a significant fetch/decode cost:
    - This is typically the largest part of the execution time, actual operation performed is typically small.
    - Does not depend on the operation.
    - Does depend on the addressing modes in use.
* Effective Addresses are available for source and destination operands at the same time:
    - Fewer requirements to move operands into registers.
* Registe direct operands are not intrinsically faster than indirect:
    - MC64K registers are still memory locations.
    - Access time improvements are still possible from the simpler address calculation.

The general rules for optimisation can be summerised as:
* Optimise for fewer instructions.
    - Instruction decode overhead dominates.
* Where there is a choice between additional instructions or a more complex addressing mode, prefer the addressing mode.
    - An exception to this rule is where the same complex address would be evaluated repeatedly.
    - A single evaluation to a register temporary and reuse of the temporary is still faster.

### Examples

Consider the following contrived example in C:

```
                 a0            a1                    a2                d0
void sum_indexed(float64* dest, const float64* src, const uint32* idx, uint32 count) {
    while (count--) {
        int i = *idx++;
        dest[i] += src[i];
    }
}
```

An unoptimised version of this code in 68000/882 may look like this:

```
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

```
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

```
_sum_indexed:
    biz.l  d0, .done
.loop:
    move.l (a2)+, d0
    fadd.d (a1, d0.l * 8), (a0, d0.l * 8) ; dual wield EA modes
    dbnz   d0, .loop                      ; dbnz decrements uint32 to zero
.done:
    rts
```
