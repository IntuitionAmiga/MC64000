;  888b     d888  .d8888b.   .d8888b.      d8888  888    d8P
;  8888b   d8888 d88P  Y88b d88P  Y88b    d8P888  888   d8P
;  88888b.d88888 888    888 888          d8P 888  888  d8P
;  888Y88888P888 888        888d888b.   d8P  888  888d88K
;  888 Y888P 888 888        888P "Y88b d88   888  8888888b
;  888  Y8P  888 888    888 888    888 8888888888 888  Y88b
;  888   "   888 Y88b  d88P Y88b  d88P       888  888   Y88b
;  888       888  "Y8888P"   "Y8888P"        888  888    Y88b
;
;   - 64-bit 680x0-inspired Virtual Machine and assembler -
;
; My first project - fold_to_empty.s
;
; Functions that are full of fold-to-empty operations

.locally_never_get_here:
    rts

fold_to_empty_source_matches_destination:
    ; These fold because the source and destination are the same
    exg    r0, r0
    fexg  fp1, fp1
    move.b d0, d0
    move.w d0, d0
    move.l d0, d0
    move.q d0, d0
    move.b (r0, d1.b), (r0, d1.b)
    move.w (r0, d1.w * 2), (r0, d1.w * 2)
    move.l (r0, d1.l * 4), (r0, d1.l * 4)
    move.q (r0, d1.q * 8), (r0, d1.q * 8)
    move.b 1(r0, d1.b), 1(r0, d1.b)
    move.w 2(r0, d1.w * 2), 2(r0, d1.w * 2)
    move.l 4(r0, d1.l * 4), 4(r0, d1.l * 4)
    move.q 8(r0, d1.q * 8), 8(r0, d1.q * 8)
    rts ; return will not fold - check the bytecode for the 0x07 return opcode

fold_to_empty_immediate_zero:
    ; These fold because the immediate zero source operand results in no change
    or.b   #0, r0
    or.w   #0, r1
    or.l   #0, r2
    or.q   #0, r3
    add.b  #0, r4
    add.w  #0, r5
    add.l  #0, r6
    add.q  #0, r7
    sub.b  #0, r8
    sub.w  #0, r9
    sub.l  #0, r10
    sub.q  #0, r11
    or.b   #0, (r0)
    or.w   #0, (r1)
    or.l   #0, (r2)
    or.q   #0, (r3)
    add.b  #0, (r4)
    add.w  #0, (r5)
    add.l  #0, (r6)
    add.q  #0, (r7)
    sub.b  #0, (r8)
    sub.w  #0, (r9)
    sub.l  #0, (r10)
    sub.q  #0, (r11)
    or.b   #0, 1(r0)
    or.w   #0, 2(r1)
    or.l   #0, 4(r2)
    or.q   #0, 8(r3)
    add.b  #0, 1(r4)
    add.w  #0, 2(r5)
    add.l  #0, 4(r6)
    add.q  #0, 8(r7)
    sub.b  #0, 1(r8)
    sub.w  #0, 2(r9)
    sub.l  #0, 4(r10)
    sub.q  #0, 8(r11)

    rts ; return will not fold - check the bytecode for the 0x07 return opcode

fold_to_empty_immediate_one:
    muls.b #1, r0
    muls.w #1, r1
    muls.l #1, r2
    muls.q #1, r3
    mulu.b #1, r4
    mulu.w #1, r5
    mulu.l #1, r6
    mulu.q #1, r7
    divs.b #1, r8
    divs.w #1, r9
    divs.l #1, r10
    divs.q #1, r11
    divu.b #1, r12
    divu.w #1, r13
    divu.l #1, r14
    divu.q #1, r15

    muls.b #1, (r0)
    muls.w #1, (r1)
    muls.l #1, (r2)
    muls.q #1, (r3)
    mulu.b #1, (r4)
    mulu.w #1, (r5)
    mulu.l #1, (r6)
    mulu.q #1, (r7)
    divs.b #1, (r8)
    divs.w #1, (r9)
    divs.l #1, (r10)
    divs.q #1, (r11)
    divu.b #1, (r12)
    divu.w #1, (r13)
    divu.l #1, (r14)
    divu.q #1, (r15)
    muls.b #1, 1(r0)
    muls.w #1, 2(r1)
    muls.l #1, 3(r2)
    muls.q #1, 8(r3)
    mulu.b #1, 1(r4)
    mulu.w #1, 2(r5)
    mulu.l #1, 4(r6)
    mulu.q #1, 8(r7)
    divs.b #1, 1(r8)
    divs.w #1, 2(r9)
    divs.l #1, 4(r10)
    divs.q #1, 9(r11)
    divu.b #1, 1(r12)
    divu.w #1, 2(r13)
    divu.l #1, 4(r14)
    divu.q #1, 8(r15)

    rts ; return will not fold - check the bytecode for the 0x07 return opcode

fold_to_empty_conditional_never_taken:
    biz.b  #1, .locally_never_get_here
    biz.w  #1, .locally_never_get_here
    biz.l  #1, .locally_never_get_here
    biz.q  #1, .locally_never_get_here
    bnz.b  #0, .locally_never_get_here
    bnz.w  #0, .locally_never_get_here
    bnz.l  #0, .locally_never_get_here
    bnz.q  #0, .locally_never_get_here
    bmi.b  #1, .locally_never_get_here
    bmi.w  #1, .locally_never_get_here
    bmi.l  #1, .locally_never_get_here
    bmi.q  #1, .locally_never_get_here
    bpl.b  #-1, .locally_never_get_here
    bpl.w  #-1, .locally_never_get_here
    bpl.l  #-1, .locally_never_get_here
    bpl.q  #-1, .locally_never_get_here
    fbiz.s  #1., .locally_never_get_here
    fbiz.d  #1., .locally_never_get_here
    fbnz.s  #0., .locally_never_get_here
    fbnz.d  #0., .locally_never_get_here
    fbmi.s  #1., .locally_never_get_here
    fbmi.d  #1., .locally_never_get_here
    fbpl.s  #-1., .locally_never_get_here
    fbpl.d  #-1., .locally_never_get_here
    bne.b #1, #1, .locally_never_get_here
    bne.w #1, #1, .locally_never_get_here
    bne.l #1, #1, .locally_never_get_here
    bne.q #1, #1, .locally_never_get_here
    blt.b #1, #0, .locally_never_get_here
    blt.w #1, #0, .locally_never_get_here
    blt.l #1, #0, .locally_never_get_here
    blt.q #1, #0, .locally_never_get_here
    ble.b #1, #0, .locally_never_get_here
    ble.w #1, #0, .locally_never_get_here
    ble.l #1, #0, .locally_never_get_here
    ble.q #1, #0, .locally_never_get_here
    beq.b #1, #-1, .locally_never_get_here
    beq.w #1, #-1, .locally_never_get_here
    beq.l #1, #-1, .locally_never_get_here
    beq.q #1, #-1, .locally_never_get_here
    bge.b #1, #2, .locally_never_get_here
    bge.w #1, #2, .locally_never_get_here
    bge.l #1, #2, .locally_never_get_here
    bge.q #1, #2, .locally_never_get_here
    bgt.b #1, #2, .locally_never_get_here
    bgt.w #1, #2, .locally_never_get_here
    bgt.l #1, #2, .locally_never_get_here
    bgt.q #1, #2, .locally_never_get_here
    fbne.s #1., #1., .locally_never_get_here
    fbne.d #1., #1., .locally_never_get_here
    fblt.s #1., #0., .locally_never_get_here
    fblt.d #1., #0., .locally_never_get_here
    fble.s #1., #0., .locally_never_get_here
    fble.d #1., #0., .locally_never_get_here
    fbeq.s #1., #-1., .locally_never_get_here
    fbeq.d #1., #-1., .locally_never_get_here
    fbge.s #1., #2., .locally_never_get_here
    fbge.d #1., #2., .locally_never_get_here
    fbgt.s #1., #2., .locally_never_get_here
    fbgt.d #1., #2., .locally_never_get_here

    rts ; return will not fold - check the bytecode for the 0x07 return opcode

fold_to_empry_conditional_always_taken_to_next_position:
    biz.b  #0, #0
    biz.w  #0, #0
    biz.l  #0, #0
    biz.q  #0, #0
    bnz.b  #1, #0
    bnz.w  #1, #0
    bnz.l  #1, #0
    bnz.q  #1, #0
    bmi.b  #-1, #0
    bmi.w  #-1, #0
    bmi.l  #-1, #0
    bmi.q  #-1, #0
    bpl.b  #1, #0
    bpl.w  #1, #0
    bpl.l  #1, #0
    bpl.q  #1, #0
    fbiz.s  #0., #0
    fbiz.d  #0., #0
    fbnz.s  #1., #0
    fbnz.d  #1., #0
    fbmi.s  #-1., #0
    fbmi.d  #-1., #0
    fbpl.s  #1., #0
    fbpl.d  #1., #0
    bne.b #0, #1, #0
    bne.w #0, #1, #0
    bne.l #0, #1, #0
    bne.q #0, #1, #0
    blt.b #1, #2, #0
    blt.w #1, #2, #0
    blt.l #1, #2, #0
    blt.q #1, #2, #0
    ble.b #1, #1, #0
    ble.w #1, #1, #0
    ble.l #1, #1, #0
    ble.q #1, #1, #0
    beq.b #1, #1, #0
    beq.w #1, #1, #0
    beq.l #1, #1, #0
    beq.q #1, #1, #0
    bge.b #2, #2, #0
    bge.w #2, #2, #0
    bge.l #2, #2, #0
    bge.q #2, #2, #0
    bgt.b #3, #2, #0
    bgt.w #3, #2, #0
    bgt.l #3, #2, #0
    bgt.q #3, #2, #0
    fbne.s #0., #1., #0
    fbne.d #0., #1., #0
    fblt.s #1., #2., #0
    fblt.d #1., #2., #0
    fble.s #1., #1., #0
    fble.d #1., #1., #0
    fbeq.s #1., #1., #0
    fbeq.d #1., #1., #0
    fbge.s #3., #2., #0
    fbge.d #3., #2., #0
    fbgt.s #3., #2., #0
    fbgt.d #3., #2., #0
    beq.b r0, r0, #0
    beq.w r0, r0, #0
    beq.l r0, r0, #0
    beq.q r0, r0, #0
    beq.b (r0), (r0), #0
    beq.w (r0), (r0), #0
    beq.l (r0), (r0), #0
    beq.q (r0), (r0), #0
    beq.b 8(r0), 8(r0), #0
    beq.w 8(r0), 8(r0), #0
    beq.l 8(r0), 8(r0), #0
    beq.q 8(r0), 8(r0), #0
    beq.b (r0, d1.b), (r0, d1.b), #0
    beq.w (r0, d1.w * 2), (r0, d1.w * 2), #0
    beq.l (r0, d1.l * 4), (r0, d1.l * 4), #0
    beq.q (r0, d1.q * 8), (r0, d1.q * 8), #0
    beq.b 1(r0, d1.b), 1(r0, d1.b), #0
    beq.w 2(r0, d1.w * 2), 2(r0, d1.w * 2), #0
    beq.l 4(r0, d1.l * 4), 4(r0, d1.l * 4), #0
    beq.q 8(r0, d1.q * 8), 8(r0, d1.q * 8), #0
    beq.b 8(pc), 8(pc), #0
    beq.w 8(pc), 8(pc), #0
    beq.l 8(pc), 8(pc), #0
    beq.q 8(pc), 8(pc), #0
    beq.b (pc, d0.b), (pc, d0.b), #0
    beq.w (pc, d0.w * 2), (pc, d0.w * 2), #0
    beq.l (pc, d0.l * 4), (pc, d0.l * 4), #0
    beq.q (pc, d0.q * 8), (pc, d0.q * 8), #0
    beq.b 1(pc, d0.b), 1(pc, d0.b), #0
    beq.w 2(pc, d0.w * 2), 2(pc, d0.w * 2), #0
    beq.l 4(pc, d0.l * 4), 4(pc, d0.l * 4), #0
    beq.q 8(pc, d0.q * 8), 8(pc, d0.q * 8), #0
    fbeq.s fp0, fp0, #0
    fbeq.d fp0, fp0, #0
    fbeq.s (r0), (r0), #0
    fbeq.d (r0), (r0), #0
    fbeq.s 8(r0), 8(r0), #0
    fbeq.d 8(r0), 8(r0), #0
    fbeq.s (r0, d1.l * 4), (r0, d1.l * 4), #0
    fbeq.d (r0, d1.q * 8), (r0, d1.q * 8), #0
    fbeq.s 4(r0, d1.l * 4), 4(r0, d1.l * 4), #0
    fbeq.d 8(r0, d1.q * 8), 8(r0, d1.q * 8), #0
    fbeq.s 8(pc), 8(pc), #0
    fbeq.d 8(pc), 8(pc), #0
    fbeq.s (pc, d0.l * 4), (pc, d0.l * 4), #0
    fbeq.d (pc, d0.q * 8), (pc, d0.q * 8), #0
    fbeq.s 4(pc, d0.l * 4), 4(pc, d0.l * 4), #0
    fbeq.d 8(pc, d0.q * 8), 8(pc, d0.q * 8), #0

    rts ; return will not fold - check the bytecode for the 0x07 return opcode
