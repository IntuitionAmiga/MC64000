
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
; Full-On DIY Hello World. No stubs. Just for IntuitionAmiga.

    ; Define some names to make the code easier to read. Substitutions declared with @def have file scope only.
    @def io_init         #0, #0
    @def io_done         #1, #0
    @def io_print_string #2, #0

    ; The standard test host expects main to be exported as executable. This directive can go anywhere
    ; in the code but it makes sense to put it just before the declaration.
    @export main x
main:
;    hcf     io_init
    ; And off we go!
;    lea     .hello_string, a0       ; the print routine expects the string address in a0.
;    hcf     io_print_string         ; trigger the vector.

    ; The standard test host expects exit to be exported as executable.


    add.q   d1, d0   ; 2
    move.q  d0, d2   ; 2
    bmi.q   d0, main ; 7 (opcode, subopcode, EA, <displacement>), expect -11 displacement

    dbnz    d6, main

    move.q  #0, d7


    @export exit x
exit:
;    hcf     io_done
    ; We are done.
    rts

    ; Our data...
;.hello_string:
;    dc.b "Hello world!\n\0"         ; This isn't C. You provide your own null termination!
