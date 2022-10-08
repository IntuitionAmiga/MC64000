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
#include <type_traits>
#include <misc/scalar.hpp>
#include <host/memory.hpp>

namespace MC64K::Host::Memory {

template<typename T>
inline T swapWord(T iValue) {
    static_assert(std::is_integral<T>::value, "Invalid type for swapWord<T>()");
    if constexpr(2 == sizeof(T)) {
        return __builtin_bswap16(iValue);
    } else if constexpr(4 == sizeof(T)) {
        return __builtin_bswap32(iValue);
    } else if constexpr(8 == sizeof(T)) {
        return __builtin_bswap64(iValue);
    }
    return iValue;
}

/**
 * Addresses will be aligned and the count reduced by one or two if necessary.
 * Caller bears responsibility for ensuring a non-zero size.
 */
template<typename T>
void byteswap(void* pDestination, void const* pSource, uint64 uCount) {
    static_assert(std::is_integral<T>::value, "Invalid type for byteswap<T>()");
    T* pDstWord = (T*)__builtin_assume_aligned(
        alignBlockOf<T>(pDestination, uCount),
        sizeof(T)
    );
    if (!uCount) {
        return;
    }
    T const* pSrcWord = (T const*)__builtin_assume_aligned(
        alignBlockOf<T>(pSource, uCount),
        sizeof(T)
    );
    while (uCount--) {
        *pDstWord++ = swapWord<T>(*pSrcWord++);
    }
}

} // namespace
#endif
