
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
; My first project - main.s

    ; Testing directives
    @enable log_label_add          ; turns on logging of newly encountered labels in this file
    @define ENTRY_FUNCTION main    ; defines ENTRY_FUNCTION as a substitution for main
    @export ENTRY_FUNCTION         ; indicate that ENTRY_FUNCTION (main) is to be exported

ENTRY_FUNCTION:                    ; should be handled as main:

    ; Using position independent calls for now
    bsr fold_to_empty_source_matches_destination
    bsr fold_to_empty_immediate_zero
    bsr fold_to_empty_conditional_always_taken_to_next_position

    ; this has been defined as rts globally so should not trigger an error
    get_to_the_chopper
