; forever

    clr.q d0

.loop: ; this is a global label
    hcf #0xABADCAFE
    bra .loop
