
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
    hcf     audio_init

    move.q  .audio_properties, d0
    hcf     audio_open
    biz.q   a0, exit
    move.q  a0, a2

    lea     .test_file_name, a0
    move.q  #IO_OPEN_READ, d0
    hcf     io_file_open
    biz.q   a0, .close_audio
    move.q  a0, a3

    clr.q   d2
    move.l  AUDIO_REG_BUFFER_SIZE_BYTES(a2), d2

.loop:
    move.q  AUDIO_REG_BUFFER_ADDRESS(a2), a1
    move.q  a3, a0
    move.q  d2, d0
    hcf     io_file_read
    move.q  d0, d3
    lsr.l   #1, d0 ; samples actually read
    move.l  d0, AUDIO_REG_SUBMIT_LENGTH(a2)

    move.q  a2, a0
    hcf     audio_write
    beq.q   d3, d2, .loop

.close_file:
    move.q  a3, a0
    hcf     io_file_close

.close_audio:
    move.q  a2, a0
    hcf     audio_close

exit:
    hcf     audio_done
    hcf     io_done
    rts

    @align 0, 8
.audio_properties:
    dc.w 44100, 5
    dc.b AUDIO_CH_MONO, AUDIO_INT_16, 0, 0

.test_file_name:
    dc.b "./Testing44100.raw\0"
