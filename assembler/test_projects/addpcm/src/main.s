
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

    ; Open audio output
    move.q  .audio_properties, d0
    hcf     audio_open
    biz.q   a0, exit
    move.q  a0, a3             ; audio out context

    ; load in the source file
    lea     .test_file_name, a0
    bsr     read_file
    biz.q   a0, .close_audio
    move.q  a0, a2             ; source buffer


    clr.q   d2
    move.l  AUDIO_REG_BUFFER_SIZE_BYTES(a3), d2
    move.q  AUDIO_REG_BUFFER_ADDRESS(a3), a1

    ; Free source data
    move.q  a2, a0
    hcf     mem_free


.close_audio:
    ; close audio output
    move.q  a3, a0
    hcf     audio_close

exit:
    hcf     audio_done
    hcf     io_done
    rts

; filename in a0
; return buffer address in a0
; return bytes actually read in d0
; trashes d0/d1/a0/a1
read_compressed_file:
    move.q  a2, -(sp)
    move.q  d2, -(sp)

    move.q  #IO_OPEN_READ, d0
    hcf     io_file_open
    biz.q   a0, .done
    move.q  a0, a2             ; save file pointer

    ; Determine the size of the file
    clr.q   d0                 ; offset 0
    move.q  #IO_SEEK_END, d1   ; from file end
    hcf     io_file_seek       ; error in d0
    bnz.q   d0, .close_file

    hcf     io_file_tell       ; size in d0
    bnz.q   d1, .close         ; error in d1
    move.q  d0, d2             ; save file size

    clr.q   d0                 ; offset 0
    move.q  #IO_SEEK_START, d1 ; from file end
    hcf     io_file_seek       ; error in d0
    bnz.q   d0, .close_file

    ; Allocate buffer
    move.q  d2, d0             ; size in d0
    hcf     mem_alloc
    bnz.q   d0, .close_file    ; error in d0

    ; read the file
    move.q  d2, d0             ; size in d0
    move.q  a0, a1             ; buffer in a1
    move.q  a2, a0             ; file* in a0
    hcf     io_file_read       ; load data in

    move.q  d0, d2             ; bytes actually read

.close_file:
    move.q  a2, a0
    hcf     io_file_close

.done:
    move.q  a1, a0 ; buffer address
    move.q  d2, d0 ; bytes actually read
    move.q  (sp)+, d2
    move.q  (sp)+, a2

    rts



    @align 0, 8
.audio_properties:
    dc.w 16000, 20
    dc.b AUDIO_CH_STEREO, AUDIO_INT_16, 0, 0


.test_file_name:
    dc.b "./decode_raw16be.bin\0"
