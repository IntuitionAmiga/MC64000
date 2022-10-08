#ifndef MC64K_STANDARD_TEST_HOST_MEM_AVX2_FUNCTIONS_HPP
    #define MC64K_STANDARD_TEST_HOST_MEM_AVX2_FUNCTIONS_HPP

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

enum {
    // Tuning factor that controls how many vectors worth of data there is before
    // actually doing the work to process as vector
    MIN_VECTOR_SIZE_FACTOR = 2
};

/**
 * Helper function. Fills an AVX register with a given integer, based on the word size.
 * Any size other than 2, 4 or 8 will be filled as the low order byte.
 */
template<typename T>
inline __m256i splatInt(T iValue) {
    static_assert(std::is_integral<T>::value, "Invalid type for splatInt<T>()");
    if constexpr(2 == sizeof(T)) {
        return _mm256_set1_epi16((int16)iValue);
    } else if constexpr(4 == sizeof(T)) {
        return _mm256_set1_epi32((int32)iValue);
    } else if constexpr(8 == sizeof(T)) {
        return _mm256_set1_epi64x((int64)iValue);
    }
    return _mm256_set1_epi8((int8)iValue);
}

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
        T const* piFinal = piDestination + uSize;

        if (uSize > (MIN_VECTOR_SIZE_FACTOR * sizeof(__m256i)/sizeof(T)) ) {
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
            __m256i  vValue         = splatInt<T>(uValue);
            __m256i* pviDestination = (__m256i*)piDestination;
            for (; pviDestination < pAlignedTop; pviDestination++) {
                _mm256_stream_si256(pviDestination, vValue);
            }

            // do the misaligned tail
            piDestination = (T*)pAlignedTop;
        }
        while (piDestination < piFinal) {
            *piDestination++ = uValue;
        }
    }
}

/**
 * Block memory bitwise AND generic (naive)
 */
template<typename T>
void bitwiseAnd(void* pBuffer, T uValue, uint64 uSize) {
    static_assert(std::is_integral<T>::value, "Invalid type for bitwiseAnd<T>()");
    T* piDestination;
    if constexpr(1 == sizeof(T)) {
        piDestination = (T*)pBuffer;
    } else {
        piDestination = (T*)__builtin_assume_aligned(alignBlockOf<T>(pBuffer, uSize), sizeof(T));
    }
    T const* piFinal  = piDestination + uSize;

    // Only do the vectorisation if the size is worth it
    if (uSize > (MIN_VECTOR_SIZE_FACTOR * sizeof(__m256i)/sizeof(T)) ) {
        T const* pAlignedBot = (T const*)(
            (((uint64)piDestination) + sizeof(__m256i) - 1) &
            ~(sizeof(__m256i) - 1)
        );

        // do any misaligned lead in
        while (piDestination < pAlignedBot && uSize-- > 0) {
            *piDestination++ &= uValue;
        }

        __m256i const* pAlignedTop = (__m256i*)(
            (((uint64)piFinal)) &
            ~(sizeof(__m256i) - 1)
        );
        __m256i  vValue         = splatInt<T>(uValue);
        __m256i* pviDestination = (__m256i*)piDestination;
        for (; pviDestination < pAlignedTop; pviDestination++) {
            __m256i vTemp = _mm256_load_si256(pviDestination);
            __m256i vOut  = _mm256_and_si256(vTemp, vValue);
            _mm256_store_si256(pviDestination, vOut);
        }
        piDestination = (T*)pAlignedTop;
    }
    while (piDestination < piFinal) {
        *piDestination++ &= uValue;
    }
}

/**
 * Block memory bitwise OR generic (naive)
 */
template<typename T>
void bitwiseOr(void* pBuffer, T uValue, uint64 uSize) {
    static_assert(std::is_integral<T>::value, "Invalid type for bitwiseOr<T>()");

    T* piDestination;
    if constexpr(1 == sizeof(T)) {
        piDestination = (T*)pBuffer;
    } else {
        piDestination = (T*)__builtin_assume_aligned(alignBlockOf<T>(pBuffer, uSize), sizeof(T));
    }

    T const* piFinal  = piDestination + uSize;
    // Only do the vectorisation if the size is worth it
    if (uSize > (MIN_VECTOR_SIZE_FACTOR * sizeof(__m256i)/sizeof(T)) ) {
        T const* pAlignedBot = (T const*)(
            (((uint64)piDestination) + sizeof(__m256i) - 1) &
            ~(sizeof(__m256i) - 1)
        );

        // do any misaligned lead in
        while (piDestination < pAlignedBot && uSize-- > 0) {
            *piDestination++ |= uValue;
        }

        __m256i const* pAlignedTop = (__m256i*)(
            (((uint64)piFinal)) &
            ~(sizeof(__m256i) - 1)
        );
        __m256i  vValue         = splatInt<T>(uValue);
        __m256i* pviDestination = (__m256i*)piDestination;
        for (; pviDestination < pAlignedTop; pviDestination++) {
            __m256i vTemp = _mm256_load_si256(pviDestination);
            __m256i vOut  = _mm256_or_si256(vTemp, vValue);
            _mm256_store_si256(pviDestination, vOut);
        }
        piDestination = (T*)pAlignedTop;
    }
    while (piDestination < piFinal) {
        *piDestination++ |= uValue;
    }
}

/**
 * Block memory bitwise XOR generic (naive)
 */
template<typename T>
void bitwiseXor(void* pBuffer, T uValue, uint64 uSize) {
    static_assert(std::is_integral<T>::value, "Invalid type for bitwiseXor<T>()");

    T* piDestination;
    if constexpr(1 == sizeof(T)) {
        piDestination = (T*)pBuffer;
    } else {
        piDestination = (T*)__builtin_assume_aligned(alignBlockOf<T>(pBuffer, uSize), sizeof(T));
    }

    T const* piFinal  = piDestination + uSize;
    // Only do the vectorisation if the size is worth it
    if (uSize > (MIN_VECTOR_SIZE_FACTOR * sizeof(__m256i)/sizeof(T)) ) {
        T const* pAlignedBot = (T const*)(
            (((uint64)piDestination) + sizeof(__m256i) - 1) &
            ~(sizeof(__m256i) - 1)
        );

        // do any misaligned lead in
        while (piDestination < pAlignedBot && uSize-- > 0) {
            *piDestination++ ^= uValue;
        }

        __m256i const* pAlignedTop = (__m256i*)(
            (((uint64)piFinal)) &
            ~(sizeof(__m256i) - 1)
        );
        __m256i  vValue         = splatInt<T>(uValue);
        __m256i* pviDestination = (__m256i*)piDestination;
        for (; pviDestination < pAlignedTop; pviDestination++) {
            __m256i vTemp = _mm256_load_si256(pviDestination);
            __m256i vOut  = _mm256_xor_si256(vTemp, vValue);
            _mm256_store_si256(pviDestination, vOut);
        }
        piDestination = (T*)pAlignedTop;
    }
    while (piDestination < piFinal) {
        *piDestination++ ^= uValue;
    }
}

} // namespace
#endif
