#ifndef MC64K_STANDARD_TEST_HOST_MEM_GENERIC_FILL_HPP
    #define MC64K_STANDARD_TEST_HOST_MEM_GENERIC_FILL_HPP

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
 * Fill a word aligned block with words. If the base addess is not aligned, filling starts from the next aligned
 * address with one fewer element.
 *
 * @param void*  pBuffer
 * @param uint16 uValue
 * @param uint64 uSize
 */
void fillWord(void* pBuffer, uint16 uValue, uint64 uSize) {
    uint16* p = (uint16*)__builtin_assume_aligned(alignBlockOf<uint16>(pBuffer, uSize), sizeof(uint16));
    while (uSize--) {
        *p++ = uValue;
    }
}

/**
 * Fill a long aligned block with longs. If the base addess is not aligned, filling starts from the next aligned
 * address with one fewer element.
 *
 * @todo - explicit vectorisation of larger blocks.
 *
 * @param void*  pBuffer
 * @param uint32 uValue
 * @param uint64 uSize
 */
void fillLong(void* pBuffer, uint32 uValue, uint64 uSize) {
    uint32* p = (uint32*)__builtin_assume_aligned(alignBlockOf<uint32>(pBuffer, uSize), sizeof(uint32));
    while (uSize--) {
        *p++ = uValue;
    }
}

/**
 * Fill a word aligned block with words. If the base adddess is not aligned, filling starts from the next aligned
 * address with one fewer element.
 *
 * @param void*  pBuffer
 * @param uint64 uValue
 * @param uint64 uSize
 */
void fillQuad(void* pBuffer, uint64 uValue, uint64 uSize) {
    uint64* p = (uint64*)__builtin_assume_aligned(alignBlockOf<uint64>(pBuffer, uSize), sizeof(uint64));
    while (uSize--) {
        *p++ = uValue;
    }
}

} // namespace
#endif
