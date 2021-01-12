; All of these examples should result in no bytecode being emitted

; Source and destination operands are the same
    exg    r0, r0
    fexg  fp1, fp1
    move.b d0, d0
    move.w d0, d0
    move.l d0, d0
    move.q d0, d0

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
