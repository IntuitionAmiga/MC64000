
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
; Stub library for the standard test host IO routines


    @def display_vector #3

    @equ PXL_CLUT_8       0
    @equ PXL_ARGB         1

    ; Callback entry points
    @equ DISPLAY_REG_CALL_FRAME           0
    @equ DISPLAY_REG_CALL_KEY_PRESS       8
    @equ DISPLAY_REG_CALL_KEY_RELEASE    16
    @equ DISPLAY_REG_CALL_BUTTON_PRESS   24
    @equ DISPLAY_REG_CALL_BUTTON_RELEASE 32
    @equ DISPLAY_REG_CALL_MOVEMENT       40

    ; Display properties
    @equ DISPLAY_REG_SOFT_BUFFER_ADDRESS 48
    @equ DISPLAY_REG_SOFT_BUFFER_PIXELS  56
    @equ DISPLAY_REG_SOFT_BUFFER_BYTES   60
    @equ DISPLAY_REG_WIDTH               64
    @equ DISPLAY_REG_HEIGHT              66
    @equ DISPLAY_REG_FLAGS               68
    @equ DISPLAY_REG_PXL_FORMAT          70
    @equ DISPLAY_REG_REFRESH_HZ          71

    @equ DISPLAY_REG_EVENT_CODE          72
    @equ DISPLAY_REG_EVENT_MASK          74
    @equ DISPLAY_REG_POSITION_X          76
    @equ DISPLAY_REG_POSITION_Y          78

    @equ display_init     #0, display_vector
    @equ display_done     #1, display_vector
    @equ display_open     #2, display_vector
    @equ display_close    #3, display_vector
    @equ display_begin    #4, display_vector
