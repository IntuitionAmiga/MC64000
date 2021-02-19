; unconditional branch forwards
    bra .unresolved_0

    clr.q d0

.unresolved_0:
    rts

    biz.l #0, .unresolved_1 ; we expect this to code fold to an unconditional

    fbiz.s #0., .unresolved_2

    clr.q d0

.unresolved_1:
    rts

    beq.q r0, r0, .unresolved_2

    rts

.unresolved_2:
    rts

