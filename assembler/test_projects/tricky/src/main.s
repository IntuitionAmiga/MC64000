
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
; Functional tests for tricky, infrequently used instructions.
;

    @align 0, 8
.float1:
    dc.s    1.0
.float2:
    dc.s    2.0
.float3:
    dc.s    0.0

main:
    ;fmove.s .float1, .float3
    ;fadd.s  .float2, .float3
    fbgt.s  .float3, .float1, .nerr
    finfo.s .float1, d0
    rts




    ; swap and exchange
    ;move.q  #$FEDCBA9876543210, d2 ; d2 = #$FEDCBA9876543210
    ;exg     d2, d3                 ; d3 = #$FEDCBA9876543210, d2 = 0
    ;swap.q  d3, d4                 ; d4 = #$1032547698BADCFE
    ;swap.l  d3, d5                 ; d5 = #$0000000010325476
    ;swap    d5, d6                 ; d6 = #$0000000054761032

    ;fmove.s #1.0, fp0
    ;fmove.s #2.0, fp1
    ;fexg    fp0, fp1               ; fp0.s = 2.0, fp1.s = 1.0
    ;fmove.d #3.0, fp2
    ;fmove.d #4.0, fp3
    ;fexg    fp2, fp3               ; fp2.d = 4.0, fp3.d = 3.0
    ;fexg    fp0, fp2               ; fp0.d = 4.0, fp2.s = 1.0

    ; assertions
    ;bne.q   #0, d2,                 .error
    ;bne.q   #$FEDCBA9876543210, d3, .error
    ;bne.q   #$1032547698BADCFE, d4, .error
    ;bne.q   #$0000000010325476, d5, .error
    ;bne.q   #$0000000054761032, d6, .error
    ;fbne.d  #4.0, fp0,              .error
    ;fbne.d  #3.0, fp3,              .error
    ;fbne.s  #1.0, fp1,              .error
    ;fbne.s  #2.0, fp2,              .error

    ; find first one.
    ;move.q  #0, d2                  ; d2 = 1
    ;bfffo   d2, d3                  ; d3 = -1 - a negative index means no bits set
    ;add.q   #1, d2                  ; d2 = 1
    ;bfffo   d2, d4                  ; d4 = 0  - most significant bit position is 0
    ;add.q   #1, d2                  ; d2 = 2
    ;bfffo   d2, d5                  ; d5 = 1  - most significant bit position is 1
    ;add.q   #1, d2                  ; d2 = 3
    ;bfffo   d2, d6                  ; d6 = 1  - most significant bit position still 1
    ;add.q   #1, d2                  ; d2 = 4
    ;bfffo   d2, d7                  ; d7 = 2  - most significant bit position now 2
    ;bfffo   d3, d1                  ; d1 = 63 - most significant bit positon is 63

    ; assertions
    ;bne.q   #63, d1, .error
    ;bne.q   #4,  d2, .error
    ;bne.q   #-1, d3, .error
    ;bne.q   #0,  d4, .error
    ;bne.q   #1,  d5, .error
    ;bne.q   #1,  d6, .error
    ;bne.q   #2,  d7, .error

    ; field count
    ;move.q  #0, d2                  ; d2 = 0
    ;bfcnt   d2, d3                  ; d3 = 0 - 0 bits set
    ;add.q   #1, d2                  ; d2 = 1
    ;bfcnt   d2, d4                  ; d4 = 1 - 1 bit set
    ;add.q   #2, d2                  ; d2 = 3
    ;bfcnt   d2, d5                  ; d5 = 2 - 2 bits set
    ;add.q   #1, d2                  ; d2 = 4
    ;bfcnt   d2, d6                  ; d6 = 1 - 1 bit set
    ;add.q   #3, d2                  ; d2 = 7
    ;bfcnt   d2, d7                  ; d7 = 3 - 3 bits set

    ; assertions
    ;bne.q   #7,  d2, .error
    ;bne.q   #0,  d3, .error
    ;bne.q   #1,  d4, .error
    ;bne.q   #2,  d5, .error
    ;bne.q   #1,  d6, .error
    ;bne.q   #3,  d7, .error

    ;lea     .success_text, a0
    ;hcf     io_print_string

    ;bra     exit

;.error:
    ;lea     .error_text, a0
    ;hcf     io_print_string

.nerr:

exit:
    rts

;.error_text:
;    dc.b "\nFailed assertion test\n\0"

;.success_text:
;    dc.b "\nAll tests passed!\n\0"
