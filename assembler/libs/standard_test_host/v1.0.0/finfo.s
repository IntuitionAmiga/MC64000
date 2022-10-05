
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
; Constants relating to the result of finfo.s/d

    @equ FINFO_BIT_NONZERO   0  ; Set for any valid float that is nonzero
    @equ FINFO_BIT_NEGATIVE  1  ; Set for any valid float that is negative
    @equ FINFO_BIT_SUBNORMAL 2  ; Set for any float that has a subnormal representation
    @equ FINFO_BIT_INFINITE  3  ; Set for any float approaching +/- infinity
    @equ FINFO_BIT_NAN       4  ; Set for any value that is not a valid float
    @equ FINFO_BIT_UNKNOWN   5  ; Set if the value could not be classified

