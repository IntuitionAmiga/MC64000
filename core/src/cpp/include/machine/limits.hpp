#ifndef MC64K_MACHINE_LIMITS_HPP
    #define MC64K_MACHINE_LIMITS_HPP

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

#include <cstdio>
#include "register.hpp"
#include "error.hpp"

namespace MC64K::Machine::Limits {

enum {
    MAX_HCF_VECTORS = 256,
    STACK_ALIGN     = 32,
    MIN_STACK_SIZE  = 64,
    MAX_STACK_SIZE  = 1 << 23
};

enum {
    F_ZERO      =  0,
    F_NONZERO   =  1,
    F_NEGATIVE  =  2,
    F_SUBNORMAL =  4,
    F_INFINITE  =  8,
    F_NAN       = 16,
    F_UNKNOWN   = 32
};

} // namespace
#endif
