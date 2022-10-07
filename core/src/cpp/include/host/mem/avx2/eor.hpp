#ifndef MC64K_STANDARD_TEST_HOST_MEM_AVX2_EOR_HPP
    #define MC64K_STANDARD_TEST_HOST_MEM_AVX2_EOR_HPP

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
#include <misc/scalar.hpp>
#include <host/memory.hpp>

namespace MC64K::Host::Memory {

/**
 * @param void*  pBuffer
 * @param uint8 uValue
 * @param uint64 uSize
 */
void eorByte(void* pBuffer, uint8 uValue, uint64 uSize) {
    uint8* p = (uint8*)pBuffer;
    while (uSize--) {
        *p++ ^= uValue;
    }
}


/**
 * @param void*  pBuffer
 * @param uint16 uValue
 * @param uint64 uSize
 */
void eorWord(void* pBuffer, uint16 uValue, uint64 uSize) {
    int16* piDestination = (int16*)__builtin_assume_aligned(
        alignBlockOf<int16>(pBuffer, uSize),
        sizeof(int16)
    );

    int16 iValue             = (int16)uValue;
    int16 const* piFinal     = piDestination + uSize;
    int16 const* pAlignedBot = (int16*)(
        (((uint64)piDestination) + sizeof(__m256i) - 1) &
        ~(sizeof(__m256i) - 1)
    );

    __m256i const* pAlignedTop = (__m256i*)(
        (((uint64)piFinal)) &
        ~(sizeof(__m256i) - 1)
    );

    // do the misaligned lead in
    while (piDestination < pAlignedBot && uSize-- > 0) {
        *piDestination++ ^= iValue;
    }

    // do the aligned middle bit
    __m256i  vValue         = _mm256_set1_epi16(iValue);
    __m256i* pviDestination = (__m256i*)piDestination;

    // @todo: unroll me?
    for (; pviDestination < pAlignedTop; pviDestination++) {
        __m256i vTemp = _mm256_load_si256(pviDestination);
         __m256i vOut  = _mm256_xor_si256(vTemp, vValue);
        _mm256_store_si256(pviDestination, vOut);
    }

    // do the misaligned tail
    piDestination = (int16*)pAlignedTop;
    while (piDestination < piFinal) {
        *piDestination++ ^= iValue;
    }
}

/**
 * @param void*  pBuffer
 * @param uint32 uValue
 * @param uint64 uSize
 */
void eorLong(void* pBuffer, uint32 uValue, uint64 uSize) {
    int32* piDestination = (int32*)__builtin_assume_aligned(
        alignBlockOf<int32>(pBuffer, uSize),
        sizeof(int32)
    );
    int32 iValue             = (int32)uValue;
    int32 const* piFinal     = piDestination + uSize;
    int32 const* pAlignedBot = (int32*)(
        (((uint64)piDestination) + sizeof(__m256i) - 1) &
        ~(sizeof(__m256i) - 1)
    );

    __m256i const* pAlignedTop = (__m256i*)(
        (((uint64)piFinal)) &
        ~(sizeof(__m256i) - 1)
    );

    // do the misaligned lead in
    while (piDestination < pAlignedBot && uSize-- > 0) {
        *piDestination++ ^= iValue;
    }

    // do the aligned middle bit
    __m256i  vValue       = _mm256_set1_epi32(iValue);
    __m256i* pviDestination = (__m256i*)piDestination;

    // @todo: unroll me?
     for (; pviDestination < pAlignedTop; pviDestination++) {
        __m256i vTemp = _mm256_load_si256(pviDestination);
         __m256i vOut  = _mm256_xor_si256(vTemp, vValue);
        _mm256_store_si256(pviDestination, vOut);
    }

    // do the misaligned tail
    piDestination = (int32*)pAlignedTop;
    while (piDestination < piFinal) {
        *piDestination++ ^= iValue;
    }
}

/**
 * @param void*  pBuffer
 * @param uint64 uValue
 * @param uint64 uSize
 */
void eorQuad(void* pBuffer, uint64 uValue, uint64 uSize) {
    int64* piDestination = (int64*)__builtin_assume_aligned(
        alignBlockOf<int64>(pBuffer, uSize),
        sizeof(int64)
    );
    int64 iValue             = (int64)uValue;
    int64 const* piFinal     = piDestination + uSize;
    int64 const* pAlignedBot = (int64*)(
        (((uint64)piDestination) + sizeof(__m256i) - 1) &
        ~(sizeof(__m256i) - 1)
    );

    __m256i const* pAlignedTop = (__m256i*)(
        (((uint64)piFinal)) &
        ~(sizeof(__m256i) - 1)
    );

    // do the misaligned lead in
    while (piDestination < pAlignedBot && uSize-- > 0) {
        *piDestination++ ^= iValue;
    }

    // do the aligned middle bit
    __m256i  vValue       = _mm256_set1_epi64x(iValue);
    __m256i* pviDestination = (__m256i*)piDestination;

    // @todo: unroll me?
    for (; pviDestination < pAlignedTop; pviDestination++) {
        __m256i vTemp = _mm256_load_si256(pviDestination);
         __m256i vOut  = _mm256_xor_si256(vTemp, vValue);
        _mm256_store_si256(pviDestination, vOut);
    }

    // do the misaligned tail
    piDestination = (int64*)pAlignedTop;
    while (piDestination < piFinal) {
        *piDestination++ ^= iValue;
    }
}

} // namespace
#endif
