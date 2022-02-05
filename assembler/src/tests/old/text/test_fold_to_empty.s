; All of these examples should result in no bytecode being emitted

; Source and destination operands are the same
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

; Immediate source of zero has no effect
    or.b   #0, r0
    or.w   #0, r0
    or.l   #0, r0
    or.q   #0, r0
    add.b  #0, r0
    add.w  #0, r0
    add.l  #0, r0
    add.q  #0, r0
    sub.b  #0, r0
    sub.w  #0, r0
    sub.l  #0, r0
    sub.q  #0, r0
    or.b   #0, (r0)
    or.w   #0, (r0)
    or.l   #0, (r0)
    or.q   #0, (r0)
    add.b  #0, (r0)
    add.w  #0, (r0)
    add.l  #0, (r0)
    add.q  #0, (r0)
    sub.b  #0, (r0)
    sub.w  #0, (r0)
    sub.l  #0, (r0)
    sub.q  #0, (r0)

; Immediate source of 1 has no effect
    muls.b #1, r0
    muls.w #1, r0
    muls.l #1, r0
    muls.q #1, r0
    mulu.b #1, r0
    mulu.w #1, r0
    mulu.l #1, r0
    mulu.q #1, r0
    divs.b #1, r0
    divs.w #1, r0
    divs.l #1, r0
    divs.q #1, r0
    divu.b #1, r0
    divu.w #1, r0
    divu.l #1, r0
    divu.q #1, r0

; Conditional branch never taken
    biz.b  #1, #16
    biz.w  #1, #16
    biz.l  #1, #16
    biz.q  #1, #16
    bnz.b  #0, #16
    bnz.w  #0, #16
    bnz.l  #0, #16
    bnz.q  #0, #16
    bmi.b  #1, #16
    bmi.w  #1, #16
    bmi.l  #1, #16
    bmi.q  #1, #16
    bpl.b  #-1, #16
    bpl.w  #-1, #16
    bpl.l  #-1, #16
    bpl.q  #-1, #16
    fbiz.s  #1., #16
    fbiz.d  #1., #16
    fbnz.s  #0., #16
    fbnz.d  #0., #16
    fbmi.s  #1., #16
    fbmi.d  #1., #16
    fbpl.s  #-1., #16
    fbpl.d  #-1., #16
    bne.b #1, #1, #16
    bne.w #1, #1, #16
    bne.l #1, #1, #16
    bne.q #1, #1, #16
    blt.b #1, #0, #16
    blt.w #1, #0, #16
    blt.l #1, #0, #16
    blt.q #1, #0, #16
    ble.b #1, #0, #16
    ble.w #1, #0, #16
    ble.l #1, #0, #16
    ble.q #1, #0, #16
    beq.b #1, #-1, #16
    beq.w #1, #-1, #16
    beq.l #1, #-1, #16
    beq.q #1, #-1, #16
    bge.b #1, #2, #16
    bge.w #1, #2, #16
    bge.l #1, #2, #16
    bge.q #1, #2, #16
    bgt.b #1, #2, #16
    bgt.w #1, #2, #16
    bgt.l #1, #2, #16
    bgt.q #1, #2, #16
    fbne.s #1., #1., #16
    fbne.d #1., #1., #16
    fblt.s #1., #0., #16
    fblt.d #1., #0., #16
    fble.s #1., #0., #16
    fble.d #1., #0., #16
    fbeq.s #1., #-1., #16
    fbeq.d #1., #-1., #16
    fbge.s #1., #2., #16
    fbge.d #1., #2., #16
    fbgt.s #1., #2., #16
    fbgt.d #1., #2., #16

    bne.b r0, r0, #16
    bne.w r0, r0, #16
    bne.l r0, r0, #16
    bne.q r0, r0, #16
    bne.b (r0), (r0), #16
    bne.w (r0), (r0), #16
    bne.l (r0), (r0), #16
    bne.q (r0), (r0), #16
    bne.b 8(r0), 8(r0), #16
    bne.w 8(r0), 8(r0), #16
    bne.l 8(r0), 8(r0), #16
    bne.q 8(r0), 8(r0), #16
    bne.b (r0, d1.b), (r0, d1.b), #16
    bne.w (r0, d1.w * 2), (r0, d1.w * 2), #16
    bne.l (r0, d1.l * 4), (r0, d1.l * 4), #16
    bne.q (r0, d1.q * 8), (r0, d1.q * 8), #16
    bne.b 1(r0, d1.b), 1(r0, d1.b), #16
    bne.w 2(r0, d1.w * 2), 2(r0, d1.w * 2), #16
    bne.l 4(r0, d1.l * 4), 4(r0, d1.l * 4), #16
    bne.q 8(r0, d1.q * 8), 8(r0, d1.q * 8), #16
    bne.b 8(pc), 8(pc), #16
    bne.w 8(pc), 8(pc), #16
    bne.l 8(pc), 8(pc), #16
    bne.q 8(pc), 8(pc), #16
    bne.b (pc, d0.b), (pc, d0.b), #16
    bne.w (pc, d0.w * 2), (pc, d0.w * 2), #16
    bne.l (pc, d0.l * 4), (pc, d0.l * 4), #16
    bne.q (pc, d0.q * 8), (pc, d0.q * 8), #16
    bne.b 1(pc, d0.b), 1(pc, d0.b), #16
    bne.w 2(pc, d0.w * 2), 2(pc, d0.w * 2), #16
    bne.l 4(pc, d0.l * 4), 4(pc, d0.l * 4), #16
    bne.q 8(pc, d0.q * 8), 8(pc, d0.q * 8), #16
    fbne.s fp0, fp0, #16
    fbne.d fp0, fp0, #16
    fbne.s (r0), (r0), #16
    fbne.d (r0), (r0), #16
    fbne.s 8(r0), 8(r0), #16
    fbne.d 8(r0), 8(r0), #16
    fbne.s (r0, d1.l * 4), (r0, d1.l * 4), #16
    fbne.d (r0, d1.q * 8), (r0, d1.q * 8), #16
    fbne.s 4(r0, d1.l * 4), 4(r0, d1.l * 4), #16
    fbne.d 8(r0, d1.q * 8), 8(r0, d1.q * 8), #16
    fbne.s 8(pc), 8(pc), #16
    fbne.d 8(pc), 8(pc), #16
    fbne.s (pc, d0.l * 4), (pc, d0.l * 4), #16
    fbne.d (pc, d0.q * 8), (pc, d0.q * 8), #16
    fbne.s 4(pc, d0.l * 4), 4(pc, d0.l * 4), #16
    fbne.d 8(pc, d0.q * 8), 8(pc, d0.q * 8), #16

; Conditional branch always taken but target has no effect
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
