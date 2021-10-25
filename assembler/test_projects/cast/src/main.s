

main:
    lea         .doubles, a1
    lea         .longs,   a0

    ; 8-byte source, 4-byte dest
    fmove.d     (a1),  fp0
    fmove.d     8(a1),  fp1
    fmove.d     16(a1),  fp2
    fmove.d     24(a1),  fp3

    fmoved.l    (a1)+, (a0)+
    fmoved.l    (a1)+, (a0)+
    fmoved.l    (a1)+, (a0)+
    fmoved.l    (a1)+, (a0)+

    ; load the longs into d3 - d0 to see what nonsense they contain
    move.l      -(a0), d3
    move.l      -(a0), d2
    move.l      -(a0), d1
    move.l      -(a0), d0

    lea         .bytes, a0

    finfo.d     -(a1), (a0)+
    finfo.d     -(a1), (a0)+
    finfo.d     -(a1), (a0)+
    finfo.d     -(a1), (a0)+

    move.b      -(a0), d7
    move.b      -(a0), d6
    move.b      -(a0), d5
    move.b      -(a0), d4


exit:
    rts

    @align 62, 16
.doubles:
    dc.d    5.0, 7.0, 11.0, 13.0

.longs:
    dc.l    0xABADCAFE, 0xABADCAFE, 0xABADCAFE, 0xABADCAFE

.bytes:
    dc.b    0, 0, 0, 0
