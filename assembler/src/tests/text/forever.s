; forever

    bra .unresolved

.loop: ; this is a local label
    hcf #0xABADCAFE
    bra.b .loop
