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

/**
 * Inline definitions for the host interface
 */
namespace MC64K::StandardTestHost::Mem {

/**
 * Provides a simple compile time checked template for the memory fill group.
 */
template<typename T>
inline void fillBlock() {
    static_assert(std::is_integral<T>::value, "Invalid type for fillBlock<T>()");
    if (uint64 uSize = Interpreter::gpr<ABI::INT_REG_1>().uQuad) {
        if (void* pBuffer = Interpreter::gpr<ABI::PTR_REG_0>().pAny) {
            Host::Memory::fill<T>(pBuffer, Interpreter::gpr<ABI::INT_REG_0>().value<T>(), uSize);
            Interpreter::gpr<ABI::INT_REG_0>().uQuad = ABI::ERR_NONE;
        } else {
            Interpreter::gpr<ABI::INT_REG_0>().uQuad = ABI::ERR_NULL_PTR;
        }
    } else {
        Interpreter::gpr<ABI::INT_REG_0>().uQuad = ABI::ERR_BAD_SIZE;
    }
}

template<typename T>
inline void andBlock() {
    static_assert(std::is_integral<T>::value, "Invalid type for andBlock<T>()");
    if (uint64 uSize = Interpreter::gpr<ABI::INT_REG_1>().uQuad) {
        if (void* pBuffer = Interpreter::gpr<ABI::PTR_REG_0>().pAny) {
            Host::Memory::bitwiseAnd<T>(pBuffer, Interpreter::gpr<ABI::INT_REG_0>().value<T>(), uSize);
            Interpreter::gpr<ABI::INT_REG_0>().uQuad = ABI::ERR_NONE;
        } else {
            Interpreter::gpr<ABI::INT_REG_0>().uQuad = ABI::ERR_NULL_PTR;
        }
    } else {
        Interpreter::gpr<ABI::INT_REG_0>().uQuad = ABI::ERR_BAD_SIZE;
    }
}

template<typename T>
inline void orBlock() {
    static_assert(std::is_integral<T>::value, "Invalid type for orBlock<T>()");
    if (uint64 uSize = Interpreter::gpr<ABI::INT_REG_1>().uQuad) {
        if (void* pBuffer = Interpreter::gpr<ABI::PTR_REG_0>().pAny) {
            Host::Memory::bitwiseOr<T>(pBuffer, Interpreter::gpr<ABI::INT_REG_0>().value<T>(), uSize);
            Interpreter::gpr<ABI::INT_REG_0>().uQuad = ABI::ERR_NONE;
        } else {
            Interpreter::gpr<ABI::INT_REG_0>().uQuad = ABI::ERR_NULL_PTR;
        }
    } else {
        Interpreter::gpr<ABI::INT_REG_0>().uQuad = ABI::ERR_BAD_SIZE;
    }
}

template<typename T>
inline void eorBlock() {
    static_assert(std::is_integral<T>::value, "Invalid type for eorBlock<T>()");
    if (uint64 uSize = Interpreter::gpr<ABI::INT_REG_1>().uQuad) {
        if (void* pBuffer = Interpreter::gpr<ABI::PTR_REG_0>().pAny) {
            Host::Memory::bitwiseXor<T>(pBuffer, Interpreter::gpr<ABI::INT_REG_0>().value<T>(), uSize);
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
inline void swapBlock() {
    static_assert(std::is_integral<T>::value, "Invalid type for swapBlock<T>()");
    static_assert(1 < sizeof(T), "Invalid size for byteswap<T>()");

    if (uint64 uSize = Interpreter::gpr<ABI::INT_REG_0>().uQuad) {
        void* pFrom  = Interpreter::gpr<ABI::PTR_REG_0>().pAny;
        void* pTo    = Interpreter::gpr<ABI::PTR_REG_1>().pAny;
        if (pFrom && pTo) {
            Host::Memory::byteswap<T>(pTo, pFrom, uSize);
            Interpreter::gpr<ABI::INT_REG_0>().uQuad = ABI::ERR_NONE;
        } else {
            Interpreter::gpr<ABI::INT_REG_0>().uQuad = ABI::ERR_NULL_PTR;
        }
    } else {
        Interpreter::gpr<ABI::INT_REG_0>().uQuad = ABI::ERR_BAD_SIZE;
    }
}

/**
 * Provides a simple compile time checked template for the memory find group.
 */
template<typename T>
inline void findBlock() {
    static_assert(std::is_integral<T>::value, "Invalid type for findBlock<T>()");
    if (uint64 uSize = Interpreter::gpr<ABI::INT_REG_1>().uQuad) {
        if (void* pBuffer = Interpreter::gpr<ABI::PTR_REG_0>().pAny) {
            Interpreter::gpr<ABI::PTR_REG_0>().pAny = (void*)Host::Memory::find<T>(
                pBuffer,
                Interpreter::gpr<ABI::INT_REG_0>().value<T>(),
                uSize
            );

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
