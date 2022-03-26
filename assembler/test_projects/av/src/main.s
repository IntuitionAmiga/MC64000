
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

main:
    hcf     io_init
    hcf     display_init
    hcf     audio_init

    ; a2 - audio context
    ; a3 - audio file
    ; a4 - display context

    move.q  .display_properties, d0
    hcf     display_open
    biz.q   a0, exit
    move.q  a0, a4

    move.q  .audio_properties, d0
    hcf     audio_open
    biz.q   a0, .close_display
    move.q  a0, a2

    lea     .test_file_name, a0
    move.q  #IO_OPEN_READ, d0
    hcf     io_file_open
    biz.q   a0, .close_audio
    move.q  a0, a3

    clr.q   d2
    move.l  AUDIO_REG_BUFFER_SIZE_BYTES(a2), d2

    lea     on_frame, DISPLAY_REG_CALL_FRAME(a4) ; called every frame before refresh

    move.q  a4, a0

    hcf     display_begin

.close_file:
    move.q  a3, a0
    hcf     io_file_close

.close_audio:
    move.q  a2, a0
    hcf     audio_close

.close_display:
    move.q  a4, a0
    hcf     display_close

exit:
    hcf     audio_done
    hcf     display_done
    hcf     io_done
    rts


on_frame:
    move.q  DISPLAY_REG_PALETTE_ADDRESS(a0), a0
    add.l   #7, (a0)

    move.q  AUDIO_REG_BUFFER_ADDRESS(a2), a1
    move.q  a3, a0
    move.q  d2, d0
    hcf     io_file_read
    move.q  d0, d3
    lsr.l   #1, d0 ; samples actually read
    move.l  d0, AUDIO_REG_SUBMIT_LENGTH(a2)

    move.q  a2, a0
    hcf     audio_write
    beq.q   d3, d2, .carry_on

    bclr.w  #DISPLAY_BIT_EVENT_LOOP_RUNNING, DISPLAY_REG_FLAGS(a4)

.carry_on:
    ; done
    rts


    @align 0, 8
.audio_properties:
    dc.w 44100, 18
    dc.b AUDIO_CH_MONO, AUDIO_INT_16, 60, 0

.display_properties:
    ; dimensions
    dc.w 256, 256

    ; flags
    dc.w 1 << DISPLAY_BIT_DRAW_BUFFER_ALL_FRAMES | 1 << DISPLAY_BIT_FLIP_ALL_FRAMES

    ; pixel format
    dc.b PXL_CLUT_8

    ; target refresh rate (Hz)
    dc.b 60


.test_file_name:
    dc.b "./Testing44100.raw\0"
