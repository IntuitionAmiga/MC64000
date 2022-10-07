#ifndef MC64K_STANDARD_TEST_HOST_MEM_GENERIC_SWAP_HPP
    #define MC64K_STANDARD_TEST_HOST_MEM_GENERIC_SWAP_HPP

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

void byteswapWord(void* pDestination, void const* pSource, uint64 uCount) {
    uint16*       pDstWord = (uint16*)pDestination;
    uint16 const* pSrcWord = (uint16 const*)pSource;
    for (uint64 u=0; u < uCount; ++u) {
        pDstWord[u] = __builtin_bswap16(pSrcWord[u]);
    }
}

void byteswapLong(void* pDestination, void const* pSource, uint64 uCount) {
    uint32*       pDstWord = (uint32*)pDestination;
    uint32 const* pSrcWord = (uint32 const*)pSource;
    for (uint64 u=0; u < uCount; ++u) {
        pDstWord[u] = __builtin_bswap32(pSrcWord[u]);
    }
}

void byteswapQuad(void* pDestination, void const* pSource, uint64 uCount) {
    uint64*       pDstWord = (uint64*)pDestination;
    uint64 const* pSrcWord = (uint64 const*)pSource;
    for (uint64 u=0; u < uCount; ++u) {
        pDstWord[u] = __builtin_bswap64(pSrcWord[u]);
    }
}

} // namespace
#endif
