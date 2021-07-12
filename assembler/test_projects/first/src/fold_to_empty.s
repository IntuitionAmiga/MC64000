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

    ; Just for fun
    @def ONE  1

fold_to_empty_source_matches_destination:
    ; These fold because the source and destination are the same
    exg    r0, r0
    fexg   fp1, fp1
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
    muls.b #ONE, r0
    muls.w #ONE, r1
    muls.l #ONE, r2
    muls.q #ONE, r3
    mulu.b #ONE, r4
    mulu.w #ONE, r5
    mulu.l #ONE, r6
    mulu.q #ONE, r7
    divs.b #ONE, r8
    divs.w #ONE, r9
    divs.l #ONE, r10
    divs.q #ONE, r11
    divu.b #ONE, r12
    divu.w #ONE, r13
    divu.l #ONE, r14
    divu.q #ONE, r15

    muls.b #ONE, (r0)
    muls.w #ONE, (r1)
    muls.l #ONE, (r2)
    muls.q #ONE, (r3)
    mulu.b #ONE, (r4)
    mulu.w #ONE, (r5)
    mulu.l #ONE, (r6)
    mulu.q #ONE, (r7)
    divs.b #ONE, (r8)
    divs.w #ONE, (r9)
    divs.l #ONE, (r10)
    divs.q #ONE, (r11)
    divu.b #ONE, (r12)
    divu.w #ONE, (r13)
    divu.l #ONE, (r14)
    divu.q #ONE, (r15)
    muls.b #ONE, 1(r0)
    muls.w #ONE, 2(r1)
    muls.l #ONE, 3(r2)
    muls.q #ONE, 8(r3)
    mulu.b #ONE, 1(r4)
    mulu.w #ONE, 2(r5)
    mulu.l #ONE, 4(r6)
    mulu.q #ONE, 8(r7)
    divs.b #ONE, 1(r8)
    divs.w #ONE, 2(r9)
    divs.l #ONE, 4(r10)
    divs.q #ONE, 9(r11)
    divu.b #ONE, 1(r12)
    divu.w #ONE, 2(r13)
    divu.l #ONE, 4(r14)

    @enable log_code_folds
    divu.q #ONE, 8(r15)
    @disable log_code_folds

    rts ; return will not fold - check the bytecode for the 0x07 return opcode

fold_to_empty_conditional_never_taken:
    biz.b  #ONE, .locally_never_get_here
    biz.w  #ONE, .locally_never_get_here
    biz.l  #ONE, .locally_never_get_here
    biz.q  #ONE, .locally_never_get_here
    bnz.b  #0, .locally_never_get_here
    bnz.w  #0, .locally_never_get_here
    bnz.l  #0, .locally_never_get_here
    bnz.q  #0, .locally_never_get_here
    bmi.b  #ONE, .locally_never_get_here
    bmi.w  #ONE, .locally_never_get_here
    bmi.l  #ONE, .locally_never_get_here
    bmi.q  #ONE, .locally_never_get_here
    bpl.b  #-1, .locally_never_get_here
    bpl.w  #-1, .locally_never_get_here
    bpl.l  #-1, .locally_never_get_here
    bpl.q  #-1, .locally_never_get_here
    fbiz.s #ONE., .locally_never_get_here
    fbiz.d #ONE., .locally_never_get_here
    fbnz.s #0., .locally_never_get_here
    fbnz.d #0., .locally_never_get_here
    fbmi.s #ONE., .locally_never_get_here
    fbmi.d #ONE., .locally_never_get_here
    fbpl.s #-1., .locally_never_get_here
    fbpl.d #-1., .locally_never_get_here
    bne.b  #ONE, #ONE, .locally_never_get_here
    bne.w  #ONE, #ONE, .locally_never_get_here
    bne.l  #ONE, #ONE, .locally_never_get_here
    bne.q  #ONE, #ONE, .locally_never_get_here
    blt.b  #ONE, #0, .locally_never_get_here
    blt.w  #ONE, #0, .locally_never_get_here
    blt.l  #ONE, #0, .locally_never_get_here
    blt.q  #ONE, #0, .locally_never_get_here
    ble.b  #ONE, #0, .locally_never_get_here
    ble.w  #ONE, #0, .locally_never_get_here
    ble.l  #ONE, #0, .locally_never_get_here
    ble.q  #ONE, #0, .locally_never_get_here
    beq.b  #ONE, #-1, .locally_never_get_here
    beq.w  #ONE, #-1, .locally_never_get_here
    beq.l  #ONE, #-1, .locally_never_get_here
    beq.q  #ONE, #-1, .locally_never_get_here
    bge.b  #ONE, #2, .locally_never_get_here
    bge.w  #ONE, #2, .locally_never_get_here
    bge.l  #ONE, #2, .locally_never_get_here
    bge.q  #ONE, #2, .locally_never_get_here
    bgt.b  #ONE, #2, .locally_never_get_here
    bgt.w  #ONE, #2, .locally_never_get_here
    bgt.l  #ONE, #2, .locally_never_get_here
    bgt.q  #ONE, #2, .locally_never_get_here
    fbne.s #ONE., #ONE., .locally_never_get_here
    fbne.d #ONE., #ONE., .locally_never_get_here
    fblt.s #ONE., #0., .locally_never_get_here
    fblt.d #ONE., #0., .locally_never_get_here
    fble.s #ONE., #0., .locally_never_get_here
    fble.d #ONE., #0., .locally_never_get_here
    fbeq.s #ONE., #-1., .locally_never_get_here
    fbeq.d #ONE., #-1., .locally_never_get_here
    fbge.s #ONE., #2., .locally_never_get_here
    fbge.d #ONE., #2., .locally_never_get_here
    fbgt.s #ONE., #2., .locally_never_get_here
    fbgt.d #ONE., #2., .locally_never_get_here

    rts ; return will not fold - check the bytecode for the 0x07 return opcode

fold_to_empty_conditional_always_taken_to_next_position:
    biz.b  #0, #0
    biz.w  #0, #0
    biz.l  #0, #0
    biz.q  #0, #0
    bnz.b  #ONE, #0
    bnz.w  #ONE, #0
    bnz.l  #ONE, #0
    bnz.q  #ONE, #0
    bmi.b  #-1, #0
    bmi.w  #-1, #0
    bmi.l  #-1, #0
    bmi.q  #-1, #0
    bpl.b  #ONE, #0
    bpl.w  #ONE, #0
    bpl.l  #ONE, #0
    bpl.q  #ONE, #0
    fbiz.s  #0., #0
    fbiz.d  #0., #0
    fbnz.s  #ONE., #0
    fbnz.d  #ONE., #0
    fbmi.s  #-1., #0
    fbmi.d  #-1., #0
    fbpl.s  #ONE., #0
    fbpl.d  #ONE., #0
    bne.b #0, #ONE, #0
    bne.w #0, #ONE, #0
    bne.l #0, #ONE, #0
    bne.q #0, #ONE, #0
    blt.b #ONE, #2, #0
    blt.w #ONE, #2, #0
    blt.l #ONE, #2, #0
    blt.q #ONE, #2, #0
    ble.b #ONE, #ONE, #0
    ble.w #ONE, #ONE, #0
    ble.l #ONE, #ONE, #0
    ble.q #ONE, #ONE, #0
    beq.b #ONE, #ONE, #0
    beq.w #ONE, #ONE, #0
    beq.l #ONE, #ONE, #0
    beq.q #ONE, #ONE, #0
    bge.b #2, #2, #0
    bge.w #2, #2, #0
    bge.l #2, #2, #0
    bge.q #2, #2, #0
    bgt.b #3, #2, #0
    bgt.w #3, #2, #0
    bgt.l #3, #2, #0
    bgt.q #3, #2, #0
    fbne.s #0., #ONE., #0
    fbne.d #0., #ONE., #0
    fblt.s #ONE., #2., #0
    fblt.d #ONE., #2., #0
    fble.s #ONE., #ONE., #0
    fble.d #ONE., #ONE., #0
    fbeq.s #ONE., #ONE., #0
    fbeq.d #ONE., #ONE., #0

    @enable log_code_folds
    fbge.s F_PI, #2., #0
    fbge.d F_PI, #2., #0

    @disable log_code_folds
    fbgt.s F_PI, #2., #0
    fbgt.d F_PI, #2., #0
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
