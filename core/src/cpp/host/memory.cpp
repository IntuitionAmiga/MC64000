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

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <misc/scalar.hpp>
#include <host/memory.hpp>


namespace MC64K::Host::Memory {

template<typename T>
inline T* alignBlock(void* pAddress, uint64& uSize) {
    uint64 uAddress = (uint64)pAddress;
    if (uAddress & (sizeof(T) - 1)) {
        uAddress = (uAddress + sizeof(T) - 1) & ~(sizeof(T) - 1);
        --uSize;
    }
    return (T*)uAddress;
}

template<typename T>
inline T const* alignBlock(void const* pAddress, uint64& uSize) {
    uint64 uAddress = (uint64)pAddress;
    if (uAddress & (sizeof(T) - 1)) {
        uAddress = (uAddress + sizeof(T) - 1) & ~(sizeof(T) - 1);
        --uSize;
    }
    return (T const*)uAddress;
}


/**
 * Returns a one-time initialised magic value
 */
uint64 ElementBuffer::getMagic(ElementBuffer const* pBuffer) {
    static uint64 uElementBufferMagic = 0;
    if (!uElementBufferMagic) {
        uElementBufferMagic = (uint64)&uElementBufferMagic;
        uElementBufferMagic ^= ((uint64)std::rand()) << 32 | (uint64)std::rand();
    }
    return uElementBufferMagic ^ (uint64)pBuffer;
}

/**
 * Valdates that a raw address seems to be a valid ElementBuffer
 */
ElementBuffer::Result ElementBuffer::validate(void const* pRawBuffer) {
    if (
        !pRawBuffer ||
        ((uint64)pRawBuffer) & (alignof(ElementBuffer) - 1)
    ) {
        return INVALID_BUFFER;
    }
    // Check the magic
    ElementBuffer const* pBuffer = (ElementBuffer const*)pRawBuffer;
    if (getMagic(pBuffer) != pBuffer->uMagic) {
        return INVALID_BUFFER;
    }
    return SUCCESS;
}

/**
 * Allocate an element buffer. This is a buffer of up to 65536 elements of up to 65536 bytes each.
 * The elements are aligned to a 64 bit boundary.
 */
ElementBuffer* ElementBuffer::allocateBuffer(uint16 uElementCount, uint16 uElementSize) {

    // Treat a zero element count as the maximum
    size_t uAllocCount      = uElementCount ? uElementCount : 65536;

    // Round up the count to the nearest 64 as we use a bitmap based allocator that operates on
    // 64-bit words.
    uAllocCount             = (uAllocCount + BITMASK_ALIGN_MASK) & ~BITMASK_ALIGN_MASK;
    size_t uMapCount        = uAllocCount >> BITMASK_SIZE_EXP;

    // Treat a zero element size as the maximum
    size_t uAllocSize       = uElementSize ? uElementSize : 65536;

    // Round up the size to the nearest 8-bytes.
    uAllocSize              = (uAllocSize + ELEMENT_ALIGN_MASK) & ~ELEMENT_ALIGN_MASK;

    // Determine the header size. This is the element buffer, with the map entry extended to have
    // enough bits to cover the rounded element count.
    size_t uHeaderSize      = sizeof(ElementBuffer) + (uMapCount - 1) * sizeof(uint64);
    ElementBuffer* pBuffer  = (ElementBuffer*)std::calloc(uHeaderSize + uAllocCount * uAllocSize, sizeof(uint8));

    // Record the actual allocation count and size
    if (pBuffer) {
        pBuffer->uMagic        = getMagic(pBuffer);
        pBuffer->uElementCount = uElementCount;
        pBuffer->uAlignedCount = (uint16)uAllocCount;
        pBuffer->uElementSize  = uElementSize;
        pBuffer->uAlignedSize  = (uint16)uAllocSize;

        // Fill the map with ones.
        std::memset(&pBuffer->aMap, -1, uMapCount * sizeof(uint64));

        // Set the element buffer base and top
        pBuffer->pBase = (uint8*)(&pBuffer->aMap[uAllocCount >> BITMASK_SIZE_EXP]);
        pBuffer->pTop  = pBuffer->pBase + uAllocCount * uAllocSize;

        std::fprintf(
            stderr,
            "Element Buffer Allocated at %p\n"
            "\tMagic: %016lX\n"
            "\tCount: %u [%u]\n"
            "\tSize:  %u [%u]\n"
            "\tMap:   %u\n"
            "\tBase:  %p\n"
            "\tTop:   %p\n",
            pBuffer,
            pBuffer->uMagic,
            (unsigned)pBuffer->uElementCount,
            (unsigned)uAllocCount,
            (unsigned)pBuffer->uElementSize,
            (unsigned)uAllocSize,
            (unsigned)uMapCount,
            pBuffer->pBase,
            pBuffer->pTop
        );
    }
    return pBuffer;
}

/**
 * Free the entire element buffer.
 */
ElementBuffer::Result ElementBuffer::freeBuffer(ElementBuffer* pBuffer) {
    // Make sure we are pointing at an actual buffer.
    Result eResult = validate(pBuffer);
    if (SUCCESS != eResult) {
        std::fprintf(
            stderr,
            "Error freeing ElementBuffer: %d\n",
            eResult
        );
    } else {
        std::fprintf(
            stderr,
            "Freeing Element Buffer at %p\n"
            "\tMagic: %016lX\n"
            "\tCount: %u [%u]\n"
            "\tSize:  %u [%u]\n",
            pBuffer,
            pBuffer->uMagic,
            (unsigned)pBuffer->uElementCount,
            (unsigned)pBuffer->uAlignedCount,
            (unsigned)pBuffer->uElementSize,
            (unsigned)pBuffer->uAlignedSize
        );
        pBuffer->uMagic = 0; // should help protect against double-free
        std::free(pBuffer);
    }
    return eResult;
}

/**
 * Alocate the next available element from the buffer. If the buffer is full, nullptr
 */
void* ElementBuffer::alloc() {
    if (getMagic(this) == uMagic) {
        unsigned uMapSize = uAlignedCount >> BITMASK_SIZE_EXP;
        unsigned uSize    = uAlignedSize ? uAlignedSize : 65536;
        // TODO - go for a 2 level bitmap, which will significantly reduce the search space.
        for (unsigned uIndex = 0; uIndex < uMapSize; ++uIndex) {
            if (uint64 uBitmap = aMap[uIndex]) {
                int iFree = __builtin_ffsl(uBitmap) - 1;
                // Clear the bit to mark as allocated
                aMap[uIndex] ^= 1UL << iFree;

                uint8* pElement  = pBase;
                unsigned uOffset = (uIndex << BITMASK_SIZE_EXP | iFree);
                pElement        += uOffset * uSize;
                return pElement;
            }
        }
    }
    return nullptr;
}

/**
 * Attempt to free an element. May not belong to a buffer.
 */
ElementBuffer::Result ElementBuffer::free(void* pElement) {
    if (
        !pElement ||
        ((uint64)pElement) & ELEMENT_ALIGN_MASK
    ) {
        std::fprintf(
            stderr,
            "Invalid element address %p (null/min align)\n",
            pElement
        );
        return INVALID_ELEMENT;
    }

    uint8* pElementAddress = (uint8*)pElement;

    if (pElementAddress < pBase || pElementAddress >= pTop) {
        std::fprintf(
            stderr,
            "Invalid element address %p\n (range [%p - %p])\n",
            pElementAddress,
            pBase,
            pTop
        );
        return INVALID_ELEMENT;
    }

    // Identify the position in the bitmap, selecting the index and masking out the bit of interest
    uint64 uSize  = uAlignedSize ? uAlignedSize : 65536;
    uint64 uIndex = ((uint64)pElementAddress - (uint64)pBase)/uSize;
    uint64 uFree  = (1ULL << (uSize & BITMASK_ALIGN_MASK));
    uIndex        >>= BITMASK_SIZE_EXP;

    // Set the "free" bit
    aMap[uIndex] |= uFree;

    return SUCCESS;
}

#if defined(__AVX2__)
#include <host/mem/avx2.hpp>
#else

/**
 * Fill a word aligned block with words. If the base addess is not aligned, filling starts from the next aligned
 * address with one fewer element.
 *
 * @todo - explicit vectorisation of larger blocks.
 *
 * @param void*  pBuffer
 * @param uint16 uValue
 * @param uint64 uSize
 */
void fillWord(void* pBuffer, uint16 uValue, uint64 uSize) {
    uint16* p = (uint16*)__builtin_assume_aligned(alignBlock<uint16>(pBuffer, uSize), sizeof(uint16));
    while (uSize--) {
        *p++ = uValue;
    }
}

/**
 * Fill a long aligned block with longs. If the base addess is not aligned, filling starts from the next aligned
 * address with one fewer element.
 *
 * @todo - explicit vectorisation of larger blocks.
 *
 * @param void*  pBuffer
 * @param uint32 uValue
 * @param uint64 uSize
 */
void fillLong(void* pBuffer, uint32 uValue, uint64 uSize) {
    uint32* p = (uint32*)__builtin_assume_aligned(alignBlock<uint32>(pBuffer, uSize), sizeof(uint32));
    while (uSize--) {
        *p++ = uValue;
    }
}

/**
 * Fill a word aligned block with words. If the base adddess is not aligned, filling starts from the next aligned
 * address with one fewer element.
 *
 * @todo - explicit vectorisation of larger blocks.
 *
 * @param void*  pBuffer
 * @param uint64 uValue
 * @param uint64 uSize
 */
void fillQuad(void* pBuffer, uint64 uValue, uint64 uSize) {
    uint64* p = (uint64*)__builtin_assume_aligned(alignBlock<uint64>(pBuffer, uSize), sizeof(uint64));
    while (uSize--) {
        *p++ = uValue;
    }
}

#endif

/**
 * Locate the first occurence of a 16 bit value in a memory block. If the base address is not aligned, search starts
 * from the next aligned address with one fewer element.
 *
 * @todo - explicit vectorisation of larger blocks.
 *
 * @param  void const*    pBuffer
 * @param  uint16         uValue
 * @param  uint64         uSize
 * @return uint16 const*  pFound
 */
uint16 const* findWord(void const* pBuffer, uint16 uValue, uint64 uSize) {
    uint16 const* p = (uint16 const*)__builtin_assume_aligned(alignBlock<uint16>(pBuffer, uSize), sizeof(uint16));
    while (uSize--) {
        if (uValue == *p) {
            return p;
        }
        ++p;
    }
    return 0;
}

/**
 * Locate the first occurence of a 32 bit value in a memory block. If the base address is not aligned, search starts
 * from the next aligned address with one fewer element.
 *
 * @todo - explicit vectorisation of larger blocks.
 *
 * @param  void const*    pBuffer
 * @param  uint32         uValue
 * @param  uint64         uSize
 * @return uint32 const*  pFound
 */
uint32 const* findLong(void const* pBuffer, uint32 uValue, uint64 uSize) {
    uint32 const* p = (uint32 const*)__builtin_assume_aligned(alignBlock<uint32>(pBuffer, uSize), sizeof(uint32));
    while (uSize--) {
        if (uValue == *p) {
            return p;
        }
        ++p;
    }
    return 0;
}

/**
 * Locate the first occurence of a 64 bit value in a memory block. If the base address is not aligned, search starts
 * from the next aligned address with one fewer element.
 *
 * @todo - explicit vectorisation of larger blocks
 *
 * @param  void const*   pBuffer
 * @param  uint64        uValue
 * @param  uint64        uSize
 * @return uint64 const* pFound
 */
uint64 const* findQuad(void const* pBuffer, uint64 uValue, uint64 uSize) {
    uint64 const* p = (uint64 const*)__builtin_assume_aligned(alignBlock<uint64>(pBuffer, uSize), sizeof(uint64));
    while (uSize--) {
        if (uValue == *p) {
            return p;
        }
        ++p;
    }
    return 0;
}

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

