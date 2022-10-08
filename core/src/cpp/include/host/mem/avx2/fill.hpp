#ifndef MC64K_STANDARD_TEST_HOST_MEM_AVX2_FILL_HPP
    #define MC64K_STANDARD_TEST_HOST_MEM_AVX2_FILL_HPP

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

#ifndef __AVX2__
    #error "This header can only be used in builds supporting AVX2"
#endif

#include <immintrin.h>
#include <type_traits>
#include <misc/scalar.hpp>
#include <host/memory.hpp>

namespace MC64K::Host::Memory {

template<typename T>
void fill(void* pBuffer, T uValue, uint64 uSize) {
    static_assert(std::is_integral<T>::value, "Invalid type for fill<T>()");

    if constexpr(1 == sizeof(T)) {
        std::memset(pBuffer, uValue, uSize);
    } else {
        T* piDestination = (T*)__builtin_assume_aligned(
            alignBlockOf<T>(pBuffer, uSize),
            sizeof(T)
        );
        T const* piFinal     = piDestination + uSize;
        T const* pAlignedBot = (T*)(
            (((uint64)piDestination) + sizeof(__m256i) - 1) &
            ~(sizeof(__m256i) - 1)
        );

        // do the misaligned lead in
        while (piDestination < pAlignedBot && uSize-- > 0) {
            *piDestination++ = uValue;
        }

        __m256i const* pAlignedTop = (__m256i*)(
            (((uint64)piFinal)) &
            ~(sizeof(__m256i) - 1)
        );

        __m256i vValue;
        if constexpr(2 == sizeof(T)) {
            vValue = _mm256_set1_epi16((int16)uValue);
        } else if constexpr(4 == sizeof(T)) {
            vValue = _mm256_set1_epi32((int32)uValue);
        } else if constexpr(8 == sizeof(T)) {
            vValue = _mm256_set1_epi64x((int64)uValue);
        }

        __m256i* pviDestination = (__m256i*)piDestination;

        // @todo: unroll me?
        for (; pviDestination < pAlignedTop; pviDestination++) {
            _mm256_stream_si256(pviDestination, vValue);
        }

        // do the misaligned tail
        piDestination = (T*)pAlignedTop;
        while (piDestination < piFinal) {
            *piDestination++ = uValue;
        }
    }
}

} // namespace
#endif
