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
using MC64K::Machine::GPRegister;
using MC64K::Machine::FPRegister;

namespace MC64K::StandardTestHost {

namespace ABI {

/**
 * Enumeration of entry points the host expects to find in the loaded binary.
 */
typedef enum {
    MAIN = 0,
    EXIT = 1
} EntryPoint;

/**
 * ABI. Define the registers used for param/return before stack spillage
 */
typedef enum {
    // r0-r2 (d0-d2) reserved for the first 3 integer parameter/return
    INT_REG_0 = GPRegister::R0,
    INT_REG_1 = GPRegister::R1,
    INT_REG_2 = GPRegister::R2,

    // r8-r10 (a0-a2) reserved for the first 3 pointer parameter/return
    PTR_REG_0 = GPRegister::R8,
    PTR_REG_1 = GPRegister::R9,
    PTR_REG_2 = GPRegister::R10,

    // fp0-fp2 reserved for the first 3 floating point parameter/return
    FLT_REG_0 = FPRegister::FP0,
    FLT_REG_1 = FPRegister::FP1,
    FLT_REG_2 = FPRegister::FP2,

} Register;

/**
 * Common Error codes
 */
typedef enum {
    ERR_NONE = 0,
    ERR_NULL_PTR,
    ERR_BAD_SIZE,
} Result;

} // namespace ABI

void setCLIParameters(unsigned const uArgC, char const** pArgV);

extern MC64K::Host::Definition instance;

}// namespace

#endif
