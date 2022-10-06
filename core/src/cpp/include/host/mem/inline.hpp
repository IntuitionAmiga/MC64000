#ifndef MC64K_STANDARD_TEST_HOST_MEM_INLINE_HPP
    #define MC64K_STANDARD_TEST_HOST_MEM_INLINE_HPP

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

#include <machine/register.hpp>
#include <host/standard_test_host.hpp>
#include <host/standard_test_host_mem.hpp>
#include <host/memory.hpp>

namespace MC64K::StandardTestHost::Mem {


/**
 * Provides a simple compile time checked template for the memory fill group.
 */
template<typename T>
inline void fill() {
    static_assert(std::is_integral<T>::value, "Invalid type for fill<T>()");
    if (uint64 uSize = Interpreter::gpr<ABI::INT_REG_1>().uQuad) {
        if (void* pBuffer = Interpreter::gpr<ABI::PTR_REG_0>().pAny) {
            if constexpr(8 == sizeof(T)) {
                Host::Memory::fillQuad(pBuffer, Interpreter::gpr<ABI::INT_REG_0>().uQuad, uSize);
            } else if constexpr(4 == sizeof(T)) {
                Host::Memory::fillLong (pBuffer, Interpreter::gpr<ABI::INT_REG_0>().uLong, uSize);
            } else if constexpr(2 == sizeof(T)) {
                Host::Memory::fillWord(pBuffer, Interpreter::gpr<ABI::INT_REG_0>().uWord, uSize);
            } else if constexpr(1 == sizeof(T)) {
                Host::Memory::fillByte(pBuffer, Interpreter::gpr<ABI::INT_REG_0>().uByte, uSize);
            }
            Interpreter::gpr<ABI::INT_REG_0>().uQuad = ABI::ERR_NONE;
        } else {
            Interpreter::gpr<ABI::INT_REG_0>().uQuad = ABI::ERR_NULL_PTR;
        }
    } else {
        Interpreter::gpr<ABI::INT_REG_0>().uQuad = ABI::ERR_BAD_SIZE;
    }
}

/**
 * Provides a simple compile time checked template for the memory byteswap group.
 */
template<typename T>
inline void byteswap() {
    static_assert(std::is_integral<T>::value, "Invalid type for byteswap<T>()");
    static_assert(1 < sizeof(T), "Invalid size for byteswap<T>()");

    if constexpr(8 == sizeof(T)) {
        if (uint64 uSize = Interpreter::gpr<ABI::INT_REG_0>().uQuad) {
            void* pFrom  = Interpreter::gpr<ABI::PTR_REG_0>().pAny;
            void* pTo    = Interpreter::gpr<ABI::PTR_REG_1>().pAny;
            if (pFrom && pTo) {
                Host::Memory::byteswapQuad(pTo, pFrom, uSize);
                Interpreter::gpr<ABI::INT_REG_0>().uQuad = ABI::ERR_NONE;
            } else {
                Interpreter::gpr<ABI::INT_REG_0>().uQuad = ABI::ERR_NULL_PTR;
            }
        } else {
            Interpreter::gpr<ABI::INT_REG_0>().uQuad = ABI::ERR_BAD_SIZE;
        }
    } else if constexpr(4 == sizeof(T)) {
        if (uint64 uSize = Interpreter::gpr<ABI::INT_REG_0>().uQuad) {
            void* pFrom  = Interpreter::gpr<ABI::PTR_REG_0>().pAny;
            void* pTo    = Interpreter::gpr<ABI::PTR_REG_1>().pAny;
            if (pFrom && pTo) {
                Host::Memory::byteswapLong(pTo, pFrom, uSize);
                Interpreter::gpr<ABI::INT_REG_0>().uQuad = ABI::ERR_NONE;
            } else {
                Interpreter::gpr<ABI::INT_REG_0>().uQuad = ABI::ERR_NULL_PTR;
            }
        } else {
            Interpreter::gpr<ABI::INT_REG_0>().uQuad = ABI::ERR_BAD_SIZE;
        }

    } else if constexpr(2 == sizeof(T)) {
        if (uint64 uSize = Interpreter::gpr<ABI::INT_REG_0>().uQuad) {
            void* pFrom  = Interpreter::gpr<ABI::PTR_REG_0>().pAny;
            void* pTo    = Interpreter::gpr<ABI::PTR_REG_1>().pAny;
            if (pFrom && pTo) {
                Host::Memory::byteswapWord(pTo, pFrom, uSize);
                Interpreter::gpr<ABI::INT_REG_0>().uQuad = ABI::ERR_NONE;
            } else {
                Interpreter::gpr<ABI::INT_REG_0>().uQuad = ABI::ERR_NULL_PTR;
            }
        } else {
            Interpreter::gpr<ABI::INT_REG_0>().uQuad = ABI::ERR_BAD_SIZE;
        }
    }
}

/**
 * Provides a simple compile time checked template for the memory find group.
 */
template<typename T>
inline void find() {
    static_assert(std::is_integral<T>::value, "Invalid type for find<T>()");
    if (uint64 uSize = Interpreter::gpr<ABI::INT_REG_1>().uQuad) {
        if (void* pBuffer = Interpreter::gpr<ABI::PTR_REG_0>().pAny) {
            if constexpr(8 == sizeof(T)) {
                Interpreter::gpr<ABI::PTR_REG_0>().pAny = (void*)Host::Memory::findQuad(
                    pBuffer,
                    Interpreter::gpr<ABI::INT_REG_0>().uQuad,
                    uSize
                );
            } else if constexpr(4 == sizeof(T)) {
                Interpreter::gpr<ABI::PTR_REG_0>().pAny = (void*)Host::Memory::findLong(
                    pBuffer,
                    Interpreter::gpr<ABI::INT_REG_0>().uLong,
                    uSize
                );
            } else if constexpr(2 == sizeof(T)) {
                Interpreter::gpr<ABI::PTR_REG_0>().pAny = (void*)Host::Memory::findWord(
                    pBuffer,
                    Interpreter::gpr<ABI::INT_REG_0>().uWord,
                    uSize
                );
            } else if constexpr(1 == sizeof(T)) {
                Interpreter::gpr<ABI::PTR_REG_0>().pAny = (void*)Host::Memory::findByte(
                    pBuffer,
                    Interpreter::gpr<ABI::INT_REG_0>().uByte,
                    uSize
                );
            }
            Interpreter::gpr<ABI::INT_REG_0>().uQuad = ABI::ERR_NONE;
        } else {
            Interpreter::gpr<ABI::INT_REG_0>().uQuad = ABI::ERR_NULL_PTR;
        }
    } else {
        Interpreter::gpr<ABI::INT_REG_0>().uQuad = ABI::ERR_BAD_SIZE;
    }
}

}

#endif
