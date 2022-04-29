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
#include <cstdlib>
#include <cstring>
#include <misc/scalar.hpp>


namespace MC64K::Host::Memory {

typedef uint8  u8x16 __attribute__ ((vector_size (16)));
typedef uint16 u16x8 __attribute__ ((vector_size (16)));
typedef uint32 u32x4 __attribute__ ((vector_size (16)));

/**
 * Fill a word aligned block with words. If the base adddess is not aligned, filling starts from the next aligned
 * address with one fewer element.
 *
 * @todo - explicit vectorisation of larger blocks.
 *
 * @param void*  pBuffer
 * @param uint16 uValue
 * @param uint64 uSize
 */
void fillWord(void* pBuffer, uint16 uValue, uint64 uSize) {
    uint64 uRawAddress = (uint64)pBuffer;
    if (uRawAddress & 1) {
        --uSize;
        ++uRawAddress;
    }
    uint16 *p = (uint16*)uRawAddress;
    while (uSize--) {
        *p++ = uValue;
    }
}

/**
 * Fill a long aligned block with longs. If the base adddess is not aligned, filling starts from the next aligned
 * address with one fewer element.
 *
 * @todo - explicit vectorisation of larger blocks.
 *
 * @param void*  pBuffer
 * @param uint32 uValue
 * @param uint64 uSize
 */
void fillLong(void* pBuffer, uint32 uValue, uint64 uSize) {
    uint64 uRawAddress = (uint64)pBuffer;
    if (uRawAddress & 3) {
        --uSize;
        uRawAddress = (uRawAddress + 3) & ~3ULL;
    }
    uint32 *p = (uint32*)uRawAddress;
    while (uSize--) {
        *p++ = uValue;
    }
}

/**
 * Fill a word aligned block with words. If the base adddess is not aligned, filling starts from the next aligned
 * address with one fewer element.
 *
 * @todo - explicit vectorisation of larger blocks.
 *
 * @param void*  pBuffer
 * @param uint64 uValue
 * @param uint64 uSize
 */
void fillQuad(void* pBuffer, uint64 uValue, uint64 uSize) {
    uint64 uRawAddress = (uint64)pBuffer;
    if (uRawAddress & 7) {
        --uSize;
        uRawAddress = (uRawAddress + 7) & ~7ULL;
    }
    uint64 *p = (uint64*)uRawAddress;
    while (uSize--) {
        *p++ = uValue;
    }
}

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
    uint64 uRawAddress = (uint64)pBuffer;
    if (uRawAddress & 1) {
        --uSize;
        ++uRawAddress;
    }
    uint16 const *p = (uint16 const*)uRawAddress;
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
    uint64 uRawAddress = (uint64)pBuffer;
    if (uRawAddress & 3) {
        --uSize;
        uRawAddress = (uRawAddress + 3) & ~3ULL;
    }
    uint32 const *p = (uint32 const*)uRawAddress;
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
    uint64 uRawAddress = (uint64)pBuffer;
    if (uRawAddress & 7) {
        --uSize;
        uRawAddress = (uRawAddress + 7) & ~7ULL;
    }
    uint64 const *p = (uint64 const*)uRawAddress;
    while (uSize--) {
        if (uValue == *p) {
            return p;
        }
        ++p;
    }
    return 0;
}

void byteswapWord(void* pDestination, void const* pSource, uint64 uCount) {

}

void byteswapLong(void* pDestination, void const* pSource, uint64 uCount) {

}

void byteswapQuad(void* pDestination, void const* pSource, uint64 uCount) {

}

} // namespace

