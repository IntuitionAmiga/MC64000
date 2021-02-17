; forever

    clr.q d0

    bra .unresolved_on_first_pass

.loop: ; this is a local label
    hcf #0xABADCAFE
    bra.b .loop

.unresolved_on_first_pass:
    rts
