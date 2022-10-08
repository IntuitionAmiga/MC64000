#ifndef MC64K_STANDARD_TEST_HOST_MEM_GENERIC_FIND_HPP
    #define MC64K_STANDARD_TEST_HOST_MEM_GENERIC_FIND_HPP

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

namespace MC64K::Host::Memory {

/**
 * Locate the first occurence of a 16 bit value in a memory block. If the base address is not aligned, search starts
 * from the next aligned address with one fewer element.
 *
 * @todo - explicit vectorisation of larger blocks.
 *
 * @param  void const*    pBuffer
 * @param  uint16         uValue
 * @param  uint64         uSize
 * @return uint16 const*  pFound
 */
uint16 const* findWord(void const* pBuffer, uint16 uValue, uint64 uSize) {
    uint16 const* p = (uint16 const*)__builtin_assume_aligned(alignBlockOf<uint16>(pBuffer, uSize), sizeof(uint16));
    while (uSize--) {
        if (uValue == *p) {
            return p;
        }
        ++p;
    }
    return 0;
}

/**
 * Locate the first occurence of a 32 bit value in a memory block. If the base address is not aligned, search starts
 * from the next aligned address with one fewer element.
 *
 * @todo - explicit vectorisation of larger blocks.
 *
 * @param  void const*    pBuffer
 * @param  uint32         uValue
 * @param  uint64         uSize
 * @return uint32 const*  pFound
 */
uint32 const* findLong(void const* pBuffer, uint32 uValue, uint64 uSize) {
    uint32 const* p = (uint32 const*)__builtin_assume_aligned(alignBlockOf<uint32>(pBuffer, uSize), sizeof(uint32));
    while (uSize--) {
        if (uValue == *p) {
            return p;
        }
        ++p;
    }
    return 0;
}

/**
 * Locate the first occurence of a 64 bit value in a memory block. If the base address is not aligned, search starts
 * from the next aligned address with one fewer element.
 *
 * @todo - explicit vectorisation of larger blocks
 *
 * @param  void const*   pBuffer
 * @param  uint64        uValue
 * @param  uint64        uSize
 * @return uint64 const* pFound
 */
uint64 const* findQuad(void const* pBuffer, uint64 uValue, uint64 uSize) {
    uint64 const* p = (uint64 const*)__builtin_assume_aligned(alignBlockOf<uint64>(pBuffer, uSize), sizeof(uint64));
    while (uSize--) {
        if (uValue == *p) {
            return p;
        }
        ++p;
    }
    return 0;
}

} // namespace
#endif
