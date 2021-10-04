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

#include "host/definition.hpp"

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

typedef enum {
    INIT = 0,
    PRINT_STRING,     // Print the string pointed to by r0
    PRINT_BYTE,       // Print the byte in r0 using the current format template for byte integer
    PRINT_WORD,       // Print the word in r0 using the current format template for word integer
    PRINT_LONG,       // Print the long in r0 using the current format template for long integer
    PRINT_QUAD,       // Print the quad in r0 using the current format template for quad integer
    PRINT_SINGLE,     // Print the single in fp0 using the current format template for single float
    PRINT_DOUBLE,     // Print the double in fp0 using the current format template for single float
    SET_FMT_BYTE,     // Set the format template for byte integer to the string pointed to by r0
    SET_FMT_WORD,     // Set the format template for word integer to the string pointed to by r0
    SET_FMT_LONG,     // Set the format template for long integer to the string pointed to by r0
    SET_FMT_QUAD,     // Set the format template for quad integer to the string pointed to by r0
    SET_FMT_SINGLE,   // Set the format template for single float to the string pointed to by r0
    SET_FMT_DOUBLE,   // Set the format template for double float to the string pointed to by r0
    CLR_FMT_BYTE,     // Revert to the default format template for byte integer
    CLR_FMT_WORD,     // Revert to the default format template for word integer
    CLR_FMT_LONG,     // Revert to the default format template for long integer
    CLR_FMT_QUAD,     // Revert to the default format template for quad integer
    CLR_FMT_SINGLE,   // Revert to the default format template for single float
    CLR_FMT_DOUBLE,   // Revert to the default format template for double float

} Call;

}

extern MC64K::Host::Definition instance;

}} // namespace

#endif
