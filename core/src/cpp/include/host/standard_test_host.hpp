#ifndef __MC64K_STANDARD_TEST_HOST_HPP__
    #define __MC64K_STANDARD_TEST_HOST_HPP__

/**
 *   888b     d888  .d8888b.   .d8888b.      d8888  888    d8P
 *   8888b   d8888 d88P  Y88b d88P  Y88b    d8P888  888   d8P
 *   88888b.d88888 888    888 888          d8P 888  888  d8P
 *   888Y88888P888 888        888d888b.   d8P  888  888d88K
 *   888 Y888P 888 888        888P "Y88b d88   888  8888888b
 *   888  Y8P  888 888    888 888    888 8888888888 888  Y88b
 *   888   "   888 Y88b  d88P Y88b  d88P       888  888   Y88b
 *   888       888  "Y8888P"   "Y8888P"        888  888    Y88b
 *
 *    - 64-bit 680x0-inspired Virtual Machine and assembler -
 */

#include "definition.hpp"

using MC64K::Machine::Interpreter;

namespace MC64K {
namespace StandardTestHost {

typedef enum {
    MAIN = 0,
    EXIT = 1
} EntryPoint;

/**
 * IO routines callable from the VM via hcf
 */
namespace IO {

/**
 * Call
 *
 * Enumeration of calls in the IO namespace
 */
typedef enum {
    INIT = 0,
    DONE,

    /**
     * void io_print_string r8/a0 [char const*]
     *
     * Print the string pointed to by (r8/a0)
     */
    PRINT_STRING,

    /**
     * void io_print_<type> r0 [byte, word, long, quad]
     *
     * Print the the integer type in r0 using the current format template for the integer type.
     */
    PRINT_BYTE,
    PRINT_WORD,
    PRINT_LONG,
    PRINT_QUAD,

    /**
     * void io_print_<type> fp0 [single, double]
     *
     * Print the floating point type in fp0 using the cirrent template for the float type
     */
    PRINT_SINGLE,
    PRINT_DOUBLE,

    /**
     * void io_setfmt_<type> r8/a0 [char const*]
     *
     * Set the format template for the type to the string pointed to by (r8/a0)
     */
    SET_FMT_BYTE,
    SET_FMT_WORD,
    SET_FMT_LONG,
    SET_FMT_QUAD,
    SET_FMT_SINGLE,
    SET_FMT_DOUBLE,

    /**
     * void io_clrfmt_<type> [void]
     *
     * Revert the format template for the type to the host-specified default.
     */
    CLR_FMT_BYTE,
    CLR_FMT_WORD,
    CLR_FMT_LONG,
    CLR_FMT_QUAD,
    CLR_FMT_SINGLE,
    CLR_FMT_DOUBLE,

    FILE_OPEN,
    FILE_SEEK,
    FILE_TELL,
    FILE_READ,
    FILE_WRITE,
    FILE_CLOSE,
} Call;

/**
 * Allowed modes for file opening
 */
typedef enum {
    MODE_READ = 0,
    MODE_WRITE,
    MODE_APPEND,
    MODE_READ_UPDATE,
    MODE_WRITE_UPDATE,
    MODE_APPEND_UPDATE
} OpenMode;

/**
 * Error return values
 */
typedef enum {
    ERR_NONE = 0,
    ERR_EOF,
    ERR_OPEN,
    ERR_CREATE,
    ERR_READ,
    ERR_WRITE,
    ERR_CLOSE
} Error;

Interpreter::Status hook();

};

extern MC64K::Host::Definition instance;

}} // namespace

#endif
