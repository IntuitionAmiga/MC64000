; forever

    clr.q d0

.loop: ; this is a local label
    hcf #0xABADCAFE
    bra.b .loop
