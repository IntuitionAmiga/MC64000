
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

    ; Define some names to make the code easier to read. Constants declared with @def have file scope only.

    ; This is the HCF vector for the standard test host IO routines.
    @def IO_VECTOR    0

    ; This is the function index in the vector for printing null terminated strings to the terminal.
    @def PRINT_STRING 2

    ; The standard test host expects main to be exported as executable. This directive can go anywhere
    ; in the code but it makes sense to put it just before the declaration.
    @export main x
main:
    ; And off we go!
    lea     .hello_string, a0       ; the print routine expects the string address in a0.
    move.b  #PRINT_STRING, -(sp)    ; the routine offset is pushed onto the stack.
    hcf     #IO_VECTOR              ; trigger the vector.
    add.q   #1, sp                  ; restore the stack.

    ; The standard test host expects exit to be exported as executable.
    @export exit x
exit:
    ; We are done.
    rts

    ; Our data...
.hello_string:
    dc.b "Hello world!\n\0"         ; This isn't C. You provide your own null termination!
