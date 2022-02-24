#ifndef __MC64K_MACHINE_INLINE_HPP__
#   define __MC64K_MACHINE_INLINE_HPP__

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

#include <cmath>
#include "register.hpp"

namespace MC64K::Machine {

inline void rolByte(uint8* puValue, uint8 uSize) {
    uSize &= 7;
    uint8 val = *puValue;
    *puValue = (uint8)(val << uSize | val >> (8 - uSize));
}

inline void rolWord(uint16* puValue, uint8 uSize) {
    uSize &= 15;
    uint16 val = *puValue;
    *puValue = (uint16)(val << uSize | val >> (16 - uSize));
}

inline void rolLong(uint32* puValue, uint8 uSize) {
    uSize &= 31;
    uint32 val = *puValue;
    *puValue = (uint32)(val << uSize | val >> (32 - uSize));
}

inline void rolQuad(uint64* puValue, uint8 uSize) {
    uSize &= 63;
    uint64 val = *puValue;
    *puValue = (uint64)(val << uSize | val >> (64 - uSize));
}

inline void rorByte(uint8* puValue, uint8 uSize) {
    uSize &= 7;
    uint8 val = *puValue;
    *puValue = (uint8)(val >> uSize | val << (8 - uSize));
}

inline void rorWord(uint16* puValue, uint8 uSize) {
    uSize &= 15;
    uint16 val = *puValue;
    *puValue = (uint16)(val >> uSize | val << (16 - uSize));
}

inline void rorLong(uint32* puValue, uint8 uSize) {
    uSize &= 31;
    uint32 val = *puValue;
    *puValue = (uint32)(val >> uSize | val << (32 - uSize));
}

inline void rorQuad(uint64* puValue, uint8 uSize) {
    uSize &= 63;
    uint64 val = *puValue;
    *puValue = (uint64)(val >> uSize | val << (64 - uSize));
}

inline uint8 mapFloatClassification(int iClass) {
    switch (iClass) {
        case FP_ZERO:
            return 0;
        case FP_NORMAL:
            return 1;
        case FP_SUBNORMAL:
            return 2;
        case FP_INFINITE:
            return 3;
        case FP_NAN:
        default:
            return 4;
    }
}

} // namespace
#endif

