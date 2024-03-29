#ifndef MC64K_HOST_MEMORY_HPP
    #define MC64K_HOST_MEMORY_HPP

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

class ElementBuffer {
    private:
        enum {
            ELEMENT_ALIGN      = sizeof(uint64),
            ELEMENT_ALIGN_MASK = ELEMENT_ALIGN - 1,
            BITMASK_SIZE       = sizeof(uint64) * 8,
            BITMASK_ALIGN_MASK = BITMASK_SIZE - 1,
            BITMASK_SIZE_EXP   = 6
        };

        /** Identifier */
        uint64  uMagic;
        uint8*  pBase;
        uint8*  pTop;
        float64 fInvSize;

        /** The requested number of elements */
        uint16 uElementCount;

        /** The right-sized number of elements, nearest multiple of BITMASK_SIZE */
        uint16 uAlignedCount;

        /** The requested per element size */
        uint16 uElementSize;

        /** The right-sized per element size, nearest multiple of ELEMENT_ALIGN */
        uint16 uAlignedSize;

        /** The allocation bitmap */
        uint64 aMap[1];

        static uint64 getMagic(ElementBuffer const* pBuffer = nullptr);

    public:
        enum Result {
            SUCCESS = 0,
            INVALID_BUFFER,
            INVALID_ELEMENT
        };

        /** Allocate a new buffer */
        static ElementBuffer* allocateBuffer(uint16 uElementCount, uint16 uElementSize);
        static Result freeBuffer(ElementBuffer* pBuffer);

        /** Check that a raw memory back from the VM really references a legal instance */
        static Result validate(void const* pRawBuffer);

        /** Allocate the next free element in the buffer */
        void* alloc();

        /** Free the element back to the buffer */
        Result free(void* pElement);
};


inline void copy(void* pDestination, void const* pSource, uint64 uSize) {
    std::memcpy(pDestination, pSource, uSize);
}

template<typename T>
void fill(void* pBuffer, T uValue, uint64 uSize);

template<typename T>
void bitwiseAnd(void* pBuffer, T uValue, uint64 uSize);

template<typename T>
void bitwiseOr(void* pBuffer, T uValue, uint64 uSize);

template<typename T>
void bitwiseXor(void* pBuffer, T uValue, uint64 uSize);

template<typename T>
void byteswap(void* pDestination, void const* pSource, uint64 uCount);

template<typename T>
T const* find(void const* pBuffer, T uValue, uint64 uSize);

/**
 * Align a non-const block of memory to some elemental scalar size by
 * rounding up the start address and reducing the count when this happens.
 * The count is passed by reference to permit adjustment.
 */
template<typename T>
inline T* alignBlockOf(void* pAddress, uint64& uSize) {
    uint64 uAddress = (uint64)pAddress;
    if (uAddress & (sizeof(T) - 1)) {
        uAddress = (uAddress + sizeof(T) - 1) & ~(sizeof(T) - 1);
        --uSize;
    }
    return (T*)uAddress;
}

/**
 * Align a const block of memory to some elemental scalar size by
 * rounding up the start address and reducing the count when this happens.
 * The count is passed by reference to permit adjustment.
 */
template<typename T>
inline T const* alignBlockOf(void const* pAddress, uint64& uSize) {
    uint64 uAddress = (uint64)pAddress;
    if (uAddress & (sizeof(T) - 1)) {
        uAddress = (uAddress + sizeof(T) - 1) & ~(sizeof(T) - 1);
        --uSize;
    }
    return (T const*)uAddress;
}

} // namespace

#endif
