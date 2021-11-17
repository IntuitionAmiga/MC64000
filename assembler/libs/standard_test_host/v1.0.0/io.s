
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

; Defines used by the IO ABI

    ; file open
    @equ IO_OPEN_READ            0
    @equ IO_OPEN_WRITE           1
    @equ IO_OPEN_APPEND          2
    @equ IO_OPEN_READ_UPDATE     3
    @equ IO_OPEN_WRITE_UPDATE    4
    @equ IO_OPEN_APPEND_UPDATE   5

    ; file seekmodes
    @equ IO_SEEK_START           0
    @equ IO_SEEK_END             1
    @equ IO_SEEK_CURRENT         2

    ; io specific errors
    @equ ERR_FILE              200
    @equ ERR_OPEN              201
    @equ ERR_READ              202
    @equ ERR_WRITE             203

    @def io_vector #0

    ; Functions <offset>, <vector>
    @equ io_init                   #0, io_vector
    @equ io_done                   #1, io_vector
    @equ io_print_string           #2, io_vector
    @equ io_print_byte             #3, io_vector
    @equ io_print_word             #4, io_vector
    @equ io_print_long             #5, io_vector
    @equ io_print_quad             #6, io_vector
    @equ io_print_single           #7, io_vector
    @equ io_print_double           #8, io_vector
    @equ io_set_fmt_byte           #9, io_vector
    @equ io_set_fmt_word          #10, io_vector
    @equ io_set_fmt_long          #11, io_vector
    @equ io_set_fmt_quad          #12, io_vector
    @equ io_set_fmt_single        #13, io_vector
    @equ io_set_fmt_double        #14, io_vector
    @equ io_file_open             #15, io_vector
    @equ io_file_seek             #16, io_vector
    @equ io_file_tell             #17, io_vector
    @equ io_file_read             #18, io_vector
    @equ io_file_write            #19, io_vector
    @equ io_file_close            #20, io_vector
    @equ io_file_print_string     #21, io_vector
    @equ io_file_print_byte       #22, io_vector
    @equ io_file_print_word       #23, io_vector
    @equ io_file_print_long       #24, io_vector
    @equ io_file_print_quad       #25, io_vector
    @equ io_file_print_single     #26, io_vector
    @equ io_file_print_double     #27, io_vector
    @equ io_file_parse_byte       #28, io_vector
    @equ io_file_parse_word       #29, io_vector
    @equ io_file_parse_long       #30, io_vector
    @equ io_file_parse_quad       #31, io_vector
    @equ io_file_parse_single     #32, io_vector
    @equ io_file_parse_double     #33, io_vector
    @equ io_cbuf_format_byte      #34, io_vector
    @equ io_cbuf_format_word      #35, io_vector
    @equ io_cbuf_format_long      #36, io_vector
    @equ io_cbuf_format_quad      #37, io_vector
    @equ io_cbuf_format_single    #38, io_vector
    @equ io_cbuf_format_double    #39, io_vector
    @equ io_cbuf_parse_byte       #40, io_vector
    @equ io_cbuf_parse_word       #41, io_vector
    @equ io_cbuf_parse_long       #42, io_vector
    @equ io_cbuf_parse_quad       #43, io_vector
    @equ io_cbuf_parse_single     #44, io_vector
    @equ io_cbuf_parse_double     #45, io_vector
