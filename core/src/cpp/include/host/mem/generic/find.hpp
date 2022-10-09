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

#include <cstdlib>
#include <misc/scalar.hpp>
#include <host/memory.hpp>

namespace MC64K::Host::Memory {

template<typename T>
T const* find(void const* pBuffer, T uValue, uint64 uSize) {
    static_assert(std::is_integral<T>::value, "Invalid type for find<T>()");
    if constexpr(1 == sizeof(T)) {
        return (T const*)std::memchr(pBuffer, (int)uValue, uSize);
    } else {
        T const* p = (T const*)__builtin_assume_aligned(alignBlockOf<T>(pBuffer, uSize), sizeof(T));
        while (uSize--) {
            if (uValue == *p) {
                return p;
            }
            ++p;
        }
        return 0;
    }
}


} // namespace
#endif
