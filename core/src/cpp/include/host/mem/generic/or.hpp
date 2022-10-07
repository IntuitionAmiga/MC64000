#ifndef MC64K_STANDARD_TEST_HOST_MEM_GENERIC_OR_HPP
    #define MC64K_STANDARD_TEST_HOST_MEM_GENERIC_OR_HPP

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

#include <misc/scalar.hpp>
#include <host/memory.hpp>

/**
 * Naive generic versions
 */
namespace MC64K::Host::Memory {

/**
 * @param void*  pBuffer
 * @param uint16 uValue
 * @param uint64 uSize
 */
void orByte(void* pBuffer, uint8 uValue, uint64 uSize) {
    uint8* p = (uint8*)pBuffer;
    while (uSize--) {
        *p++ |= uValue;
    }
}

/**
 * @param void*  pBuffer
 * @param uint16 uValue
 * @param uint64 uSize
 */
void orWord(void* pBuffer, uint16 uValue, uint64 uSize) {
    uint16* p = (uint16*)__builtin_assume_aligned(alignBlockOf<uint16>(pBuffer, uSize), sizeof(uint16));
    while (uSize--) {
        *p++ |= uValue;
    }
}

/**
 * @param void*  pBuffer
 * @param uint32 uValue
 * @param uint64 uSize
 */
void orLong(void* pBuffer, uint32 uValue, uint64 uSize) {
    uint32* p = (uint32*)__builtin_assume_aligned(alignBlockOf<uint32>(pBuffer, uSize), sizeof(uint32));
    while (uSize--) {
        *p++ |= uValue;
    }
}

/**
 * @param void*  pBuffer
 * @param uint64 uValue
 * @param uint64 uSize
 */
void orQuad(void* pBuffer, uint64 uValue, uint64 uSize) {
    uint64* p = (uint64*)__builtin_assume_aligned(alignBlockOf<uint64>(pBuffer, uSize), sizeof(uint64));
    while (uSize--) {
        *p++ |= uValue;
    }
}

} // namespace
#endif
