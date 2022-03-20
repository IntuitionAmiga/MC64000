#ifndef MC64K_STANDARD_TEST_HOST_HPP
    #define MC64K_STANDARD_TEST_HOST_HPP

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

enum Vector {
    ID_IO      = 0,
    ID_MEM     = 1,
    ID_VMATH   = 2,
    ID_DISPLAY = 3,
    ID_AUDIO   = 4
};

/**
 * Enumeration of entry points the host expects to find in the loaded binary.
 */
enum EntryPoint {
    MAIN = 0,
    EXIT = 1
};

/**
 * ABI. Define the registers used for param/return before stack spillage
 */
enum Register {
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
};

/**
 * Common Error codes
 */
enum Result {
    ERR_NONE = 0,
    ERR_NULL_PTR,
    ERR_BAD_SIZE,
};

} // namespace ABI

void setCLIParameters(unsigned const uArgC, char const** pArgV);

extern MC64K::Host::Definition instance;

/**
 * Calls from the VM may choose to package several small values into a single register. This union
 * provides a simple way of extracting parameters that are based on packing together standard sized
 * elements.
 */
union PackedParams {
    uint64 u64;
    int64  i64;
    uint32 u32[2];
    int32  i32[2];
    uint16 u16[4];
    int16  i16[4];
    uint8  u8[8];
    int8   i8[8];
};

}// namespace

#endif
