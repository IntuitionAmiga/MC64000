#ifndef MC64K_STANDARD_TEST_HOST_MEM_GENERIC_FUNCTIONS_HPP
    #define MC64K_STANDARD_TEST_HOST_MEM_GENERIC_FUNCTIONS_HPP

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
 * Block memory fill generic (naive)
 */
template<typename T>
void fill(void* pBuffer, T uValue, uint64 uSize) {
    static_assert(std::is_integral<T>::value, "Invalid type for fill<T>()");
    if constexpr(1 == sizeof(T)) {
        std::memset(pBuffer, uValue, uSize);
    } else {
        T* p = (T*)__builtin_assume_aligned(alignBlockOf<T>(pBuffer, uSize), sizeof(T));
        while (uSize--) {
            *p++ = uValue;
        }
    }
}

/**
 * Block memory bitwise AND generic (naive)
 */
template<typename T>
void bitwiseAnd(void* pBuffer, T uValue, uint64 uSize) {
    static_assert(std::is_integral<T>::value, "Invalid type for bitwiseAnd<T>()");
    if constexpr(1 == sizeof(T)) {
        T* p = (T*)pBuffer;
        while (uSize--) {
            *p++ &= uValue;
        }
    } else {
        // Align to word size
        T* p = (T*)__builtin_assume_aligned(alignBlockOf<T>(pBuffer, uSize), sizeof(T));
        while (uSize--) {
            *p++ &= uValue;
        }
    }
}

/**
 * Block memory bitwise OR generic (naive)
 */
template<typename T>
void bitwiseOr(void* pBuffer, T uValue, uint64 uSize) {
    static_assert(std::is_integral<T>::value, "Invalid type for bitwiseOr<T>()");
    if constexpr(1 == sizeof(T)) {
        T* p = (T*)pBuffer;
        while (uSize--) {
            *p++ |= uValue;
        }
    } else {
        // Align to word size
        T* p = (T*)__builtin_assume_aligned(alignBlockOf<T>(pBuffer, uSize), sizeof(T));
        while (uSize--) {
            *p++ |= uValue;
        }
    }
}

/**
 * Block memory bitwise XOR generic (naive)
 */
template<typename T>
void bitwiseXor(void* pBuffer, T uValue, uint64 uSize) {
    static_assert(std::is_integral<T>::value, "Invalid type for bitwiseXor<T>()");
    if constexpr(1 == sizeof(T)) {
        T* p = (T*)pBuffer;
        while (uSize--) {
            *p++ ^= uValue;
        }
    } else {
        // Align to word size
        T* p = (T*)__builtin_assume_aligned(alignBlockOf<T>(pBuffer, uSize), sizeof(T));
        while (uSize--) {
            *p++ ^= uValue;
        }
    }
}


} // namespace
#endif
