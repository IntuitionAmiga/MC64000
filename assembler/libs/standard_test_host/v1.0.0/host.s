
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
; Stub library for the standard test host ABI routines

; Standard return values

    @equ ERR_NONE     0
    @equ ERR_NULL_PTR 1
    @equ ERR_BAD_SIZE 2

; Imported symbols from this host

    @import host_info r             ; Host information string.
    @import host_cli_num_params r   ; Number of commandline parameters on startup (argc)
    @import host_cli_params r       ; List of commandline parameters on startup (argv)

; Exported symbols that are mandatory for this host

    @export main x                  ; Main entry point on execution.
    @export exit x                  ; Main exit point to ensure any cleanup.

; Standard host vector function ID values

    @equ library_init #0
    @equ library_done #1
