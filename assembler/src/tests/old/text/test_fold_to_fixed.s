; Conditional branch always taken, short branch outcome
    biz.b  #0, #31
    biz.w  #0, #31
    biz.l  #0, #31
    biz.q  #0, #31
    bnz.b  #1, #31
    bnz.w  #1, #31
    bnz.l  #1, #31
    bnz.q  #1, #31
    bmi.b  #-1, #31
    bmi.w  #-1, #31
    bmi.l  #-1, #31
    bmi.q  #-1, #31
    bpl.b  #1, #31
    bpl.w  #1, #31
    bpl.l  #1, #31
    bpl.q  #1, #31
    fbiz.s  #0., #31
    fbiz.d  #0., #31
    fbnz.s  #1., #31
    fbnz.d  #1., #31
    fbmi.s  #-1., #31
    fbmi.d  #-1., #31
    fbpl.s  #1., #31
    fbpl.d  #1., #31
    bne.b #0, #1, #31
    bne.w #0, #1, #31
    bne.l #0, #1, #31
    bne.q #0, #1, #31
    blt.b #1, #2, #31
    blt.w #1, #2, #31
    blt.l #1, #2, #31
    blt.q #1, #2, #31
    ble.b #1, #1, #31
    ble.w #1, #1, #31
    ble.l #1, #1, #31
    ble.q #1, #1, #31
    beq.b #1, #1, #31
    beq.w #1, #1, #31
    beq.l #1, #1, #31
    beq.q #1, #1, #31
    bge.b #2, #2, #31
    bge.w #2, #2, #31
    bge.l #2, #2, #31
    bge.q #2, #2, #31
    bgt.b #3, #2, #31
    bgt.w #3, #2, #31
    bgt.l #3, #2, #31
    bgt.q #3, #2, #31
    fbne.s #0., #1., #31
    fbne.d #0., #1., #31
    fblt.s #1., #2., #31
    fblt.d #1., #2., #31
    fble.s #1., #1., #31
    fble.d #1., #1., #31
    fbeq.s #1., #1., #31
    fbeq.d #1., #1., #31
    fbge.s #3., #2., #31
    fbge.d #3., #2., #31
    fbgt.s #3., #2., #31
    fbgt.d #3., #2., #31
    beq.b r0, r0, #31
    beq.w r0, r0, #31
    beq.l r0, r0, #31
    beq.q r0, r0, #31
    beq.b (r0), (r0), #31
    beq.w (r0), (r0), #31
    beq.l (r0), (r0), #31
    beq.q (r0), (r0), #31
    beq.b 8(r0), 8(r0), #31
    beq.w 8(r0), 8(r0), #31
    beq.l 8(r0), 8(r0), #31
    beq.q 8(r0), 8(r0), #31
    beq.b (r0, d1.b), (r0, d1.b), #31
    beq.w (r0, d1.w * 2), (r0, d1.w * 2), #31
    beq.l (r0, d1.l * 4), (r0, d1.l * 4), #31
    beq.q (r0, d1.q * 8), (r0, d1.q * 8), #31
    beq.b 1(r0, d1.b), 1(r0, d1.b), #31
    beq.w 2(r0, d1.w * 2), 2(r0, d1.w * 2), #31
    beq.l 4(r0, d1.l * 4), 4(r0, d1.l * 4), #31
    beq.q 8(r0, d1.q * 8), 8(r0, d1.q * 8), #31
    beq.b 8(pc), 8(pc), #31
    beq.w 8(pc), 8(pc), #31
    beq.l 8(pc), 8(pc), #31
    beq.q 8(pc), 8(pc), #31
    beq.b (pc, d0.b), (pc, d0.b), #31
    beq.w (pc, d0.w * 2), (pc, d0.w * 2), #31
    beq.l (pc, d0.l * 4), (pc, d0.l * 4), #31
    beq.q (pc, d0.q * 8), (pc, d0.q * 8), #31
    beq.b 1(pc, d0.b), 1(pc, d0.b), #31
    beq.w 2(pc, d0.w * 2), 2(pc, d0.w * 2), #31
    beq.l 4(pc, d0.l * 4), 4(pc, d0.l * 4), #31
    beq.q 8(pc, d0.q * 8), 8(pc, d0.q * 8), #31
    fbeq.s fp0, fp0, #31
    fbeq.d fp0, fp0, #31
    fbeq.s (r0), (r0), #31
    fbeq.d (r0), (r0), #31
    fbeq.s 8(r0), 8(r0), #31
    fbeq.d 8(r0), 8(r0), #31
    fbeq.s (r0, d1.l * 4), (r0, d1.l * 4), #31
    fbeq.d (r0, d1.q * 8), (r0, d1.q * 8), #31
    fbeq.s 4(r0, d1.l * 4), 4(r0, d1.l * 4), #31
    fbeq.d 8(r0, d1.q * 8), 8(r0, d1.q * 8), #31
    fbeq.s 8(pc), 8(pc), #31
    fbeq.d 8(pc), 8(pc), #31
    fbeq.s (pc, d0.l * 4), (pc, d0.l * 4), #31
    fbeq.d (pc, d0.q * 8), (pc, d0.q * 8), #31
    fbeq.s 4(pc, d0.l * 4), 4(pc, d0.l * 4), #31
    fbeq.d 8(pc, d0.q * 8), 8(pc, d0.q * 8), #31

; branch always taken, long branch outcome
    biz.b  #0, #32767
    biz.w  #0, #32767
    biz.l  #0, #32767
    biz.q  #0, #32767
    bnz.b  #1, #32767
    bnz.w  #1, #32767
    bnz.l  #1, #32767
    bnz.q  #1, #32767
    bmi.b  #-1, #32767
    bmi.w  #-1, #32767
    bmi.l  #-1, #32767
    bmi.q  #-1, #32767
    bpl.b  #1, #32767
    bpl.w  #1, #32767
    bpl.l  #1, #32767
    bpl.q  #1, #32767
    fbiz.s  #0., #32767
    fbiz.d  #0., #32767
    fbnz.s  #1., #32767
    fbnz.d  #1., #32767
    fbmi.s  #-1., #32767
    fbmi.d  #-1., #32767
    fbpl.s  #1., #32767
    fbpl.d  #1., #32767
    bne.b #0, #1, #32767
    bne.w #0, #1, #32767
    bne.l #0, #1, #32767
    bne.q #0, #1, #32767
    blt.b #1, #2, #32767
    blt.w #1, #2, #32767
    blt.l #1, #2, #32767
    blt.q #1, #2, #32767
    ble.b #1, #1, #32767
    ble.w #1, #1, #32767
    ble.l #1, #1, #32767
    ble.q #1, #1, #32767
    beq.b #1, #1, #32767
    beq.w #1, #1, #32767
    beq.l #1, #1, #32767
    beq.q #1, #1, #32767
    bge.b #2, #2, #32767
    bge.w #2, #2, #32767
    bge.l #2, #2, #32767
    bge.q #2, #2, #32767
    bgt.b #3, #2, #32767
    bgt.w #3, #2, #32767
    bgt.l #3, #2, #32767
    bgt.q #3, #2, #32767
    fbne.s #0., #1., #32767
    fbne.d #0., #1., #32767
    fblt.s #1., #2., #32767
    fblt.d #1., #2., #32767
    fble.s #1., #1., #32767
    fble.d #1., #1., #32767
    fbeq.s #1., #1., #32767
    fbeq.d #1., #1., #32767
    fbge.s #3., #2., #32767
    fbge.d #3., #2., #32767
    fbgt.s #3., #2., #32767
    fbgt.d #3., #2., #32767
    beq.b r0, r0, #32767
    beq.w r0, r0, #32767
    beq.l r0, r0, #32767
    beq.q r0, r0, #32767
    beq.b (r0), (r0), #32767
    beq.w (r0), (r0), #32767
    beq.l (r0), (r0), #32767
    beq.q (r0), (r0), #32767
    beq.b 8(r0), 8(r0), #32767
    beq.w 8(r0), 8(r0), #32767
    beq.l 8(r0), 8(r0), #32767
    beq.q 8(r0), 8(r0), #32767
    beq.b (r0, d1.b), (r0, d1.b), #32767
    beq.w (r0, d1.w * 2), (r0, d1.w * 2), #32767
    beq.l (r0, d1.l * 4), (r0, d1.l * 4), #32767
    beq.q (r0, d1.q * 8), (r0, d1.q * 8), #32767
    beq.b 1(r0, d1.b), 1(r0, d1.b), #32767
    beq.w 2(r0, d1.w * 2), 2(r0, d1.w * 2), #32767
    beq.l 4(r0, d1.l * 4), 4(r0, d1.l * 4), #32767
    beq.q 8(r0, d1.q * 8), 8(r0, d1.q * 8), #32767
    beq.b 8(pc), 8(pc), #32767
    beq.w 8(pc), 8(pc), #32767
    beq.l 8(pc), 8(pc), #32767
    beq.q 8(pc), 8(pc), #32767
    beq.b (pc, d0.b), (pc, d0.b), #32767
    beq.w (pc, d0.w * 2), (pc, d0.w * 2), #32767
    beq.l (pc, d0.l * 4), (pc, d0.l * 4), #32767
    beq.q (pc, d0.q * 8), (pc, d0.q * 8), #32767
    beq.b 1(pc, d0.b), 1(pc, d0.b), #32767
    beq.w 2(pc, d0.w * 2), 2(pc, d0.w * 2), #32767
    beq.l 4(pc, d0.l * 4), 4(pc, d0.l * 4), #32767
    beq.q 8(pc, d0.q * 8), 8(pc, d0.q * 8), #32767
    fbeq.s fp0, fp0, #32767
    fbeq.d fp0, fp0, #32767
    fbeq.s (r0), (r0), #32767
    fbeq.d (r0), (r0), #32767
    fbeq.s 8(r0), 8(r0), #32767
    fbeq.d 8(r0), 8(r0), #32767
    fbeq.s (r0, d1.l * 4), (r0, d1.l * 4), #32767
    fbeq.d (r0, d1.q * 8), (r0, d1.q * 8), #32767
    fbeq.s 4(r0, d1.l * 4), 4(r0, d1.l * 4), #32767
    fbeq.d 8(r0, d1.q * 8), 8(r0, d1.q * 8), #32767
    fbeq.s 8(pc), 8(pc), #32767
    fbeq.d 8(pc), 8(pc), #32767
    fbeq.s (pc, d0.l * 4), (pc, d0.l * 4), #32767
    fbeq.d (pc, d0.q * 8), (pc, d0.q * 8), #32767
    fbeq.s 4(pc, d0.l * 4), 4(pc, d0.l * 4), #32767
    fbeq.d 8(pc, d0.q * 8), 8(pc, d0.q * 8), #32767
