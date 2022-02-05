; Expected outputs here
    clr.b   d0
    clr.w   d0
    clr.l   d0
    clr.q   d0
    clr.b   (a0)
    clr.w   (a0)
    clr.l   (a0)
    clr.q   (a0)
    clr.b   (a0)+
    clr.w   (a0)+
    clr.l   (a0)+
    clr.q   (a0)+

; Test cases here
    and.b    #0, d0
    and.w    #0, d0
    and.l    #0, d0
    and.q    #0, d0
    muls.b   #0, d0
    muls.w   #0, d0
    muls.l   #0, d0
    muls.q   #0, d0
    mulu.b   #0, d0
    mulu.w   #0, d0
    mulu.l   #0, d0
    mulu.q   #0, d0

    and.b    #0, (a0)
    and.w    #0, (a0)
    and.l    #0, (a0)
    and.q    #0, (a0)
    muls.b   #0, (a0)
    muls.w   #0, (a0)
    muls.l   #0, (a0)
    muls.q   #0, (a0)
    mulu.b   #0, (a0)
    mulu.w   #0, (a0)
    mulu.l   #0, (a0)
    mulu.q   #0, (a0)

    and.b    #0, (a0)+
    and.w    #0, (a0)+
    and.l    #0, (a0)+
    and.q    #0, (a0)+
    muls.b   #0, (a0)+
    muls.w   #0, (a0)+
    muls.l   #0, (a0)+
    muls.q   #0, (a0)+
    mulu.b   #0, (a0)+
    mulu.w   #0, (a0)+
    mulu.l   #0, (a0)+
    mulu.q   #0, (a0)+
