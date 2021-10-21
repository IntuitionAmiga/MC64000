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
; My first project - fold_to_clear.s
;
; Functions that are full of fold-to-clear operations

    dc.b "< fold to clear <"

fold_to_clear:
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

    @enable log_code_folds
    mods.l   #1, d0
    mods.q   #1, d0
    modu.l   #1, d0
    modu.q   #1, d0

    @disable log_code_folds

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
    mods.l   #1, (a0)
    mods.q   #1, (a0)
    modu.l   #1, (a0)
    modu.q   #1, (a0)

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
    mods.l   #1, (a0)+
    mods.q   #1, (a0)+
    modu.l   #1, (a0)+
    modu.q   #1, (a0)+

    and.b    #0, -(a0)
    and.w    #0, -(a0)
    and.l    #0, -(a0)
    and.q    #0, -(a0)
    muls.b   #0, -(a0)
    muls.w   #0, -(a0)
    muls.l   #0, -(a0)
    muls.q   #0, -(a0)
    mulu.b   #0, -(a0)
    mulu.w   #0, -(a0)
    mulu.l   #0, -(a0)
    mulu.q   #0, -(a0)
    mods.l   #1, -(a0)
    mods.q   #1, -(a0)
    modu.l   #1, -(a0)
    modu.q   #1, -(a0)

    dc.b "> fold to clear >"
