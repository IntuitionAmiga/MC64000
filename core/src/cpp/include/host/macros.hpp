#ifndef __MC64K_HOST_MACROS_HPP__
#define __MC64K_HOST_MACROS_HPP__

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

namespace MC64K :: Host {

#define referenceBytecode(x) _mc64K_##x

/**
 * defineMC64KCode(name) {
 *     <bytecode>
 * }
 */
#define defineBytecode(x) uint8 _mc64K_##x[] =

#define inline32Bytecode(x) \
    (uint8)((uint32)(x) & 0xFF), \
    (uint8)(((uint32)(x) >> 8) & 0xFF), \
    (uint8)(((uint32)(x) >> 16) & 0xFF), \
    (uint8)(((uint32)(x) >> 24) & 0xFF), \

#define inline64Bytecode(x) \
    (uint8)((uint64)(x) & 0xFF), \
    (uint8)(((uint64)(x) >> 8) & 0xFF), \
    (uint8)(((uint64)(x) >> 16) & 0xFF), \
    (uint8)(((uint64)(x) >> 24) & 0xFF), \
    (uint8)(((uint64)(x) >> 32) & 0xFF), \
    (uint8)(((uint64)(x) >> 40) & 0xFF), \
    (uint8)(((uint64)(x) >> 48) & 0xFF), \
    (uint8)(((uint64)(x) >> 56) & 0xFF)

} // namespace

#endif
