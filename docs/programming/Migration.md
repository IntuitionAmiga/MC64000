## [Documentation](../README.md) > [Programming](./README.md) > Migration

The following page gives information on how to migrate User Model code from 680x0 to MC64K.

### General

* Be consistent in the use of register naming:
    - For convenience, you can use d0 ... d7 and a0 ... a7.
    - Consider changing to the universal r0 ... r15 names.
* Replace extended arithmetic:
    - If possible, use _.q_ operands.

### Branching

The branching model in MC64K is significantly different than in 680x0. The primary motivation was to provide a simpler branching model and improve performance by not having to maintain a set of condition codes for the vitual machine. Consequently, except in some highly coincidental cases, any conditional branches will have to be reworked.

* Make note of the destination of the last operation before any conditional branch as the CCR will be set based on that value.
* Determine what needs to be compared:
   - MC64K compares a pair of operands directly or a single operand against zero.
   - Most 680x0 CCR tests are equivalent to a comparison against zero.

### Floating Point

The FPU in MC64K is somewhat simplified. In particular, integer operands are not supported for any operations.

The following class of operations are not possible:

```asm
    fadd.b d0, fp0
    fadd.w (a0), fp0
    fsub.l #1, fp0
```

MC64K provides casting operations in conjunction with a greater number of available registers to perform the above, albeit in more steps:

```asm
    fmoveb.d d0, fp8    ; register signed byte to double
    fadd.d   fp8, fp0
    fmove.w  (a0), fp8  ; indirect signed word to double
    fadd.d   fp8, fp0
    fmove.l  #1, fp8    ; signed immediate integer to double
    fsub.l   fp8, fp0
```

There is no support for extended precision or packed decimal data formats.

### Endianness

The 680x0 is a Big Endian processor series. The intended targets of MC64K are generally little endian. As MC64K is not an emulator and is aimed at x64/ARM it adopts a Little Endian memory model by default.

* Generally, code should not be accessing different sized elements at the same address, but beware of the following patterns of access:
    - move.l (a0), ...
    - move.w 2(a0), ...
    - move.b 3(a0), ...
* On the 680x0, these access a full 32-bit word, the 16-bit lower half and the least significant byte respectively.
* On a Little Endian MC64K machine, these share the same address:
    - move.l (a0), ...
    - move.w (a0), ...
    - move.b (a0), ...

### Optimising

Until a JIT runtime exists, MC64K runs code _interpretively_. Long established rules for 680x0 code optimisation do not necessarily apply here:

* Every interpreted instruction has a significant fetch/decode cost:
    - This is typically the largest part of the execution time, actual operation performed is typically small.
    - Does not significantly depend on the operation.
    - Does depend on the addressing modes in use.
* Effective Addresses are available for source and destination operands at the same time:
    - Fewer requirements to move operands into registers.
* Register direct operands are not intrinsically faster than indirect since both are really just memory locations. However:
    - A fast decode path exists for a set selected register to register instruction use cases.
    - These cases are automatically detected by the assembler and an alternative bytecode representation is emitted.
    - The alternative bytecode stores the packed register pair in a byte and the entire EA decode cycle is skipped at runtime.

The general rules for optimisation can be summerised as:
* Prefer fast path instructions where possible:
    - 32/64-bit Register to Register arithmetic/logic.
    - Register to Register Fast Path operations are up to 3-4x faster than general EA to EA.
* Optimise for fewer instructions.
    - Instruction/EA decode overhead dominates.
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
    move.l (a2)+, d1
    fadd.d (a1, d1.l * 8), (a0, d1.l * 8) ; dual wield EA modes
    dbnz   d0, .loop                      ; dbnz decrements uint32 to zero
.done:
    rts
```
