#ifndef __MC64K_SCALAR_HPP__
    #define __MC64K_SCALAR_HPP__

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

/**
 * Basic scalar type definitions
 */
typedef unsigned long int  uint64;
typedef signed long int    int64;
typedef unsigned int       uint32;
typedef signed int         int32;
typedef unsigned short int uint16;
typedef signed short int   int16;
typedef unsigned char      uint8;
typedef signed char        int8;
typedef double             float64;
typedef float              float32;

/**
 * Template word type qualifiers for printf() etc.
 */
#define PFS16 "hd"
#define PFU16 "hu"
#define PFS32 "d"
#define PFU32 "u"
#define PFS64 "ld"
#define PFU64 "lu"

#endif
