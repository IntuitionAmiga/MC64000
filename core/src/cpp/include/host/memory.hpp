#ifndef MC64K_HOST_RUNTIME_HPP
    #define MC64K_HOST_RUNTIME_HPP

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

#include <cstdlib>
#include <cstring>
#include <misc/scalar.hpp>

namespace MC64K::Host::Memory {

inline void copy(void* pDestination, void const* pSource, uint64 uSize) {
    std::memcpy(pDestination, pSource, uSize);
}

inline void fillByte(void* pBuffer, uint16 uValue, uint64 uSize) {
    std::memset(pBuffer, uValue, uSize);
}

void fillWord(void* pBuffer, uint16 uValue, uint64 uSize);
void fillLong(void* pBuffer, uint32 uValue, uint64 uSize);
void fillQuad(void* pBuffer, uint64 uValue, uint64 uSize);
void byteswapWord(void* pDestination, void const* pSource, uint64 uCount);
void byteswapLong(void* pDestination, void const* pSource, uint64 uCount);
void byteswapQuad(void* pDestination, void const* pSource, uint64 uCount);

uint8  const* findByte(void const* pBuffer, uint8  uValue, uint64 uSize) {
    return (uint8 const*)std::memchr(pBuffer, (int)uValue, uSize);
}

uint16 const* findWord(void const* pBuffer, uint16 uValue, uint64 uSize);
uint32 const* findLong(void const* pBuffer, uint32 uValue, uint64 uSize);
uint64 const* findQuad(void const* pBuffer, uint64 uValue, uint64 uSize);

} // namespace

#endif
