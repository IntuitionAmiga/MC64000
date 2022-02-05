; forever

    clr.q d0

    bra .unresolved_on_first_pass

.loop: ; this is a local label
    hcf #0xABADCAFE
    bra.b .loop

.loop2:
    clr.q d0
    biz.b #0, .loop2

.unresolved_on_first_pass:
    rts
