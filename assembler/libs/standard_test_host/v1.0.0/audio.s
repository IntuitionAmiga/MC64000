
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
; Stub library for the standard test host Audio routines

    @def audio_vector #4

    @equ ERR_AUDIO_NO_DEVICE      1000
    @equ ERR_AUDIO_INVALID_HZ     1001
    @equ ERR_AUDIO_INVALID_BUF    1002
    @equ ERR_AUDIO_INVALID_FMT    1003
    @equ ERR_AUDIO_INVALID_CHN    1004

    @equ AUDIO_CH_MONO   1
    @equ AUDIO_CH_STEREO 2
    @equ AUDIO_INT_8     0
    @equ AUDIO_INT_16    1
    @equ AUDIO_MIN_HZ    4000
    @equ AUDIO_MAX_HZ    48000
    @equ AUDIO_MIN_MS    5
    @equ AUDIO_MAX_MS    5000

    @equ AUDIO_REG_BUFFER_ADDRESS  0
    @equ AUDIO_REG_BUFFER_LENGTH   8
    @equ AUDIO_REG_SUBMIT_LENGTH   12
    @equ AUDIO_REG_SAMPLES_SENT    26
    @equ AUDIO_REG_PACKET_LENGTH   24
    @equ AUDIO_REG_SAMPLE_RATE_HZ  26
    @equ AUDIO_REG_SAMPLE_FORMAT   28
    @equ AUDIO_REG_SAMPLE_SIZE     29

    @equ audio_init     #0, audio_vector
    @equ audio_done     #1, audio_vector
    @equ audio_open     #2, audio_vector
    @equ audio_close    #3, audio_vector
    @equ audio_write    #4, audio_vector

