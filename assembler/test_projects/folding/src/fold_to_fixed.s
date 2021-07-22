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
; My first project - fold_to_fixed.s
;
; Functions that are full of fold-to-fixed-branch operations

.locally_always_get_here:
    rts

fold_to_fixed_conditional_always_taken:
; Conditional branch always taken, short branch outcome to local label
    biz.b  #0, .locally_always_get_here
    biz.w  #0, .locally_always_get_here
    biz.l  #0, .locally_always_get_here
    biz.q  #0, .locally_always_get_here
    bnz.b  #1, .locally_always_get_here
    bnz.w  #1, .locally_always_get_here
    bnz.l  #1, .locally_always_get_here
    bnz.q  #1, .locally_always_get_here
    bmi.b  #-1, .locally_always_get_here
    bmi.w  #-1, .locally_always_get_here
    bmi.l  #-1, .locally_always_get_here
    bmi.q  #-1, .locally_always_get_here
    bpl.b  #1, .locally_always_get_here
    bpl.w  #1, .locally_always_get_here
    bpl.l  #1, .locally_always_get_here
    bpl.q  #1, .locally_always_get_here
    fbiz.s  #0., .locally_always_get_here
    fbiz.d  #0., .locally_always_get_here
    fbnz.s  #1., .locally_always_get_here
    fbnz.d  #1., .locally_always_get_here
    fbmi.s  #-1., .locally_always_get_here
    fbmi.d  #-1., .locally_always_get_here
    fbpl.s  #1., .locally_always_get_here
    fbpl.d  #1., .locally_always_get_here
    bne.b #0, #1, .locally_always_get_here
    bne.w #0, #1, .locally_always_get_here
    bne.l #0, #1, .locally_always_get_here
    bne.q #0, #1, .locally_always_get_here
    blt.b #1, #2, .locally_always_get_here
    blt.w #1, #2, .locally_always_get_here
    blt.l #1, #2, .locally_always_get_here
    blt.q #1, #2, .locally_always_get_here
    ble.b #1, #1, .locally_always_get_here
    ble.w #1, #1, .locally_always_get_here
    ble.l #1, #1, .locally_always_get_here
    ble.q #1, #1, .locally_always_get_here
    beq.b #1, #1, .locally_always_get_here
    beq.w #1, #1, .locally_always_get_here
    beq.l #1, #1, .locally_always_get_here
    beq.q #1, #1, .locally_always_get_here
    bge.b #2, #2, .locally_always_get_here
    bge.w #2, #2, .locally_always_get_here
    bge.l #2, #2, .locally_always_get_here
    bge.q #2, #2, .locally_always_get_here
    bgt.b #3, #2, .locally_always_get_here
    bgt.w #3, #2, .locally_always_get_here
    bgt.l #3, #2, .locally_always_get_here
    bgt.q #3, #2, .locally_always_get_here
    fbne.s #0., #1., .locally_always_get_here
    fbne.d #0., #1., .locally_always_get_here
    fblt.s #1., #2., .locally_always_get_here
    fblt.d #1., #2., .locally_always_get_here
    fble.s #1., #1., .locally_always_get_here
    fble.d #1., #1., .locally_always_get_here
    fbeq.s #1., #1., .locally_always_get_here
    fbeq.d #1., #1., .locally_always_get_here
    fbge.s #3., #2., .locally_always_get_here
    fbge.d #3., #2., .locally_always_get_here
    fbgt.s #3., #2., .locally_always_get_here
    fbgt.d #3., #2., .locally_always_get_here
    beq.b r0, r0, .locally_always_get_here
    beq.w r0, r0, .locally_always_get_here
    beq.l r0, r0, .locally_always_get_here
    beq.q r0, r0, .locally_always_get_here
    beq.b (r0), (r0), .locally_always_get_here
    beq.w (r0), (r0), .locally_always_get_here
    beq.l (r0), (r0), .locally_always_get_here
    beq.q (r0), (r0), .locally_always_get_here
    beq.b 8(r0), 8(r0), .locally_always_get_here
    beq.w 8(r0), 8(r0), .locally_always_get_here
    beq.l 8(r0), 8(r0), .locally_always_get_here
    beq.q 8(r0), 8(r0), .locally_always_get_here
    beq.b (r0, d1.b), (r0, d1.b), .locally_always_get_here
    beq.w (r0, d1.w * 2), (r0, d1.w * 2), .locally_always_get_here
    beq.l (r0, d1.l * 4), (r0, d1.l * 4), .locally_always_get_here
    beq.q (r0, d1.q * 8), (r0, d1.q * 8), .locally_always_get_here
    beq.b 1(r0, d1.b), 1(r0, d1.b), .locally_always_get_here
    beq.w 2(r0, d1.w * 2), 2(r0, d1.w * 2), .locally_always_get_here
    beq.l 4(r0, d1.l * 4), 4(r0, d1.l * 4), .locally_always_get_here
    beq.q 8(r0, d1.q * 8), 8(r0, d1.q * 8), .locally_always_get_here
    beq.b 8(pc), 8(pc), .locally_always_get_here
    beq.w 8(pc), 8(pc), .locally_always_get_here
    beq.l 8(pc), 8(pc), .locally_always_get_here
    beq.q 8(pc), 8(pc), .locally_always_get_here
    beq.b (pc, d0.b), (pc, d0.b), .locally_always_get_here
    beq.w (pc, d0.w * 2), (pc, d0.w * 2), .locally_always_get_here
    beq.l (pc, d0.l * 4), (pc, d0.l * 4), .locally_always_get_here
    beq.q (pc, d0.q * 8), (pc, d0.q * 8), .locally_always_get_here
    beq.b 1(pc, d0.b), 1(pc, d0.b), .locally_always_get_here
    beq.w 2(pc, d0.w * 2), 2(pc, d0.w * 2), .locally_always_get_here
    beq.l 4(pc, d0.l * 4), 4(pc, d0.l * 4), .locally_always_get_here
    beq.q 8(pc, d0.q * 8), 8(pc, d0.q * 8), .locally_always_get_here
    fbeq.s fp0, fp0, .locally_always_get_here
    fbeq.d fp0, fp0, .locally_always_get_here
    fbeq.s (r0), (r0), .locally_always_get_here
    fbeq.d (r0), (r0), .locally_always_get_here
    fbeq.s 8(r0), 8(r0), .locally_always_get_here
    fbeq.d 8(r0), 8(r0), .locally_always_get_here
    fbeq.s (r0, d1.l * 4), (r0, d1.l * 4), .locally_always_get_here
    fbeq.d (r0, d1.q * 8), (r0, d1.q * 8), .locally_always_get_here
    fbeq.s 4(r0, d1.l * 4), 4(r0, d1.l * 4), .locally_always_get_here
    fbeq.d 8(r0, d1.q * 8), 8(r0, d1.q * 8), .locally_always_get_here
    fbeq.s 8(pc), 8(pc), .locally_always_get_here
    fbeq.d 8(pc), 8(pc), .locally_always_get_here
    fbeq.s (pc, d0.l * 4), (pc, d0.l * 4), .locally_always_get_here
    fbeq.d (pc, d0.q * 8), (pc, d0.q * 8), .locally_always_get_here
    fbeq.s 4(pc, d0.l * 4), 4(pc, d0.l * 4), .locally_always_get_here
    fbeq.d 8(pc, d0.q * 8), 8(pc, d0.q * 8), .locally_always_get_here

    rts ;
