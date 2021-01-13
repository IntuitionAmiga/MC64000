; Example 000


main: ; this is a global label
    move.l #16, d0
.bosh:
    hcf #0xABADCAFE
    dbnz d0, .bosh
    rts
