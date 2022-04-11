
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
    @equ DISPLAY_REG_PALETTE_ADDRESS     56

    @equ DISPLAY_REG_SOFT_BUFFER_PIXELS  64
    @equ DISPLAY_REG_SOFT_BUFFER_BYTES   68

    @equ DISPLAY_REG_WIDTH               72
    @equ DISPLAY_REG_HEIGHT              74

    @equ DISPLAY_REG_VIEW_PIXELS         76
    @equ DISPLAY_REG_VIEW_WIDTH          80
    @equ DISPLAY_REG_VIEW_HEIGHT         82

    @equ DISPLAY_REG_VIEW_X_OFFSET       84
    @equ DISPLAY_REG_VIEW_Y_OFFSET       86

    @equ DISPLAY_REG_FILTH               88

    @equ DISPLAY_REG_FLAGS               96
    @equ DISPLAY_REG_PXL_FORMAT          98
    @equ DISPLAY_REG_REFRESH_HZ          99

    @equ DISPLAY_REG_EVENT_CODE          100
    @equ DISPLAY_REG_EVENT_MASK          102
    @equ DISPLAY_REG_POSITION_X          104
    @equ DISPLAY_REG_POSITION_Y          106

    @equ DISPLAY_BIT_DRAW_BUFFER_NEXT_FRAME 0
    @equ DISPLAY_BIT_DRAW_BUFFER_ALL_FRAMES 1
    @equ DISPLAY_BIT_FLIP_NEXT_FRAME        2
    @equ DISPLAY_BIT_FLIP_ALL_FRAMES        3
    @equ DISPLAY_BIT_EVENT_LOOP_RUNNING    15

    @equ DISPLAY_FC_END             0x00
    @equ DISPLAY_FC_WAIT            0x01

    @equ DISPLAY_FC_SET_PALETTE     0x02
    @equ DISPLAY_FC_ADD_PALETTE_RGB 0x03
    @equ DISPLAY_FC_SUB_PALETTE_RGB 0x04

    @equ DISPLAY_FC_SET_PALETTE_R   0x05
    @equ DISPLAY_FC_ADD_PALETTE_R   0x06
    @equ DISPLAY_FC_SUB_PALETTE_R   0x07

    @equ DISPLAY_FC_SET_PALETTE_G   0x08
    @equ DISPLAY_FC_ADD_PALETTE_G   0x09
    @equ DISPLAY_FC_SUB_PALETTE_G   0x0A

    @equ DISPLAY_FC_SET_PALETTE_B   0x0B
    @equ DISPLAY_FC_ADD_PALETTE_B   0x0C
    @equ DISPLAY_FC_SUB_PALETTE_B   0x0D

    @equ DISPLAY_FC_SWP_PALETTE     0x0E

    // View offset commands
    @equ DISPLAY_FC_SET_VIEW_X      0x0F
    @equ DISPLAY_FC_ADD_VIEW_X      0x10
    @equ DISPLAY_FC_SUB_VIEW_X      0x11
    @equ DISPLAY_FC_SET_VIEW_Y      0x12
    @equ DISPLAY_FC_ADD_VIEW_Y      0x13
    @equ DISPLAY_FC_SUB_VIEW_Y      0x14

    @equ DISPLAY_FC_SET_BYTE        0x15
    @equ DISPLAY_FC_SET_WORD        0x16
    @equ DISPLAY_FC_SET_LONG        0x17
    @equ DISPLAY_FC_ADD_BYTE        0x18
    @equ DISPLAY_FC_ADD_WORD        0x19
    @equ DISPLAY_FC_ADD_LONG        0x1A
    @equ DISPLAY_FC_SUB_BYTE        0x1B
    @equ DISPLAY_FC_SUB_WORD        0x1C
    @equ DISPLAY_FC_SUB_LONG        0x1D

    @equ display_init     #0, display_vector
    @equ display_done     #1, display_vector
    @equ display_open     #2, display_vector
    @equ display_close    #3, display_vector
    @equ display_begin    #4, display_vector
    @equ display_update   #5, display_vector
