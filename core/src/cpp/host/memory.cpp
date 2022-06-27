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

typedef uint8  u8x16 __attribute__ ((vector_size (16)));
typedef uint16 u16x8 __attribute__ ((vector_size (16)));
typedef uint32 u32x4 __attribute__ ((vector_size (16)));


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

ElementBuffer* ElementBuffer::validate(ElementBuffer* pBuffer) {
    if (pBuffer && getMagic(pBuffer) == pBuffer->uMagic) {
        return pBuffer;
    }
    return nullptr;
}

/***
 * Allocate an element buffer. This is a buffer of up to 65536 elements of up to 65536 bytes each.
 * The elements are aligned to a 64 bit boundary.
 */
ElementBuffer* ElementBuffer::allocateBuffer(uint16 uElementCount, uint16 uElementSize) {

    // Treat a zero element count as the maximum
    size_t uAllocCount      = uElementCount ? uElementCount : 65536;

    // Round up the count to the nearest 64 as we use a bitmap based allocator that operates on
    // 64-bit words.
    uAllocCount             = (uAllocCount + 63) & ~63;
    size_t uMapCount        = uAllocCount >> 6;

    // Treat a zero element size as the maximum
    size_t uAllocSize       = uElementSize ? uElementSize : 65536;

    // Round up the size to the nearest 8-bytes.
    uAllocSize              = (uAllocSize + 7) & ~7;

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

        std::fprintf(
            stderr,
            "Element Buffer Allocated at %p\n"
            "\tMagic: %016lX\n"
            "\tCount: %u [%u]\n"
            "\tSize:  %u [%u]\n"
            "\tMap:   %u\n",
            pBuffer,
            pBuffer->uMagic,
            (unsigned)pBuffer->uElementCount,
            (unsigned)uAllocCount,
            (unsigned)pBuffer->uElementSize,
            (unsigned)uAllocSize,
            (unsigned)uMapCount
        );
    }
    return pBuffer;
}

/**
 * Free the entire element buffer. Null safe.
 */
bool ElementBuffer::freeBuffer(ElementBuffer* pBuffer) {
    // Make sure we are pointing at an actual buffer.
    if ( (pBuffer = validate(pBuffer)) ) {
        std::fprintf(
            stderr,
            "Freeing Element Buffer Allocated at %p\n"
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
        pBuffer->uMagic = 0;
        std::free(pBuffer);
        return true;
    }
    return false;
}

uint8* ElementBuffer::getElementBase() const {
    return (uint8*)(&aMap[uAlignedCount >> 6]);
}

/**
 * Alocate the next available element from the buffer. If the buffer is full, nullptr
 */
void* ElementBuffer::alloc() {
    if (getMagic(this) == uMagic) {
        unsigned uMapSize = uAlignedCount >> 6;

        // TODO - go for a 2 level bitmap, which will significantly reduce the search space.
        for (unsigned uIndex = 0; uIndex < uMapSize; ++uIndex) {
            if (uint64 uBitmap = aMap[uIndex]) {
                int iFree = __builtin_ffsl(uBitmap) - 1;
                // Clear the bit to mark as allocated
                aMap[uIndex] ^= 1UL << iFree;

                uint8* pElement  = getElementBase();
                unsigned uOffset = (uIndex << 6 | iFree);
                pElement        += uOffset * uAlignedSize;
                std::fprintf(
                    stderr,
                    "Found free entry at index %u:%d [%u] => %p\n",
                    uIndex,
                    iFree,
                    uOffset,
                    pElement
                );
                return pElement;
            }
        }
    }
    return nullptr;
}

uint64 ElementBuffer::free(void* pElement) {
    if (getMagic(this) == uMagic) {

    }
    return 0;
}


/**
 * Fill a word aligned block with words. If the base adddess is not aligned, filling starts from the next aligned
 * address with one fewer element.
 *
 * @todo - explicit vectorisation of larger blocks.
 *
 * @param void*  pBuffer
 * @param uint16 uValue
 * @param uint64 uSize
 */
void fillWord(void* pBuffer, uint16 uValue, uint64 uSize) {
    uint64 uRawAddress = (uint64)pBuffer;
    if (uRawAddress & 1) {
        --uSize;
        ++uRawAddress;
    }
    uint16 *p = (uint16*)uRawAddress;
    while (uSize--) {
        *p++ = uValue;
    }
}

/**
 * Fill a long aligned block with longs. If the base adddess is not aligned, filling starts from the next aligned
 * address with one fewer element.
 *
 * @todo - explicit vectorisation of larger blocks.
 *
 * @param void*  pBuffer
 * @param uint32 uValue
 * @param uint64 uSize
 */
void fillLong(void* pBuffer, uint32 uValue, uint64 uSize) {
    uint64 uRawAddress = (uint64)pBuffer;
    if (uRawAddress & 3) {
        --uSize;
        uRawAddress = (uRawAddress + 3) & ~3ULL;
    }
    uint32 *p = (uint32*)uRawAddress;
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
    uint64 uRawAddress = (uint64)pBuffer;
    if (uRawAddress & 7) {
        --uSize;
        uRawAddress = (uRawAddress + 7) & ~7ULL;
    }
    uint64 *p = (uint64*)uRawAddress;
    while (uSize--) {
        *p++ = uValue;
    }
}

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
    uint64 uRawAddress = (uint64)pBuffer;
    if (uRawAddress & 1) {
        --uSize;
        ++uRawAddress;
    }
    uint16 const *p = (uint16 const*)uRawAddress;
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
    uint64 uRawAddress = (uint64)pBuffer;
    if (uRawAddress & 3) {
        --uSize;
        uRawAddress = (uRawAddress + 3) & ~3ULL;
    }
    uint32 const *p = (uint32 const*)uRawAddress;
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
    uint64 uRawAddress = (uint64)pBuffer;
    if (uRawAddress & 7) {
        --uSize;
        uRawAddress = (uRawAddress + 7) & ~7ULL;
    }
    uint64 const *p = (uint64 const*)uRawAddress;
    while (uSize--) {
        if (uValue == *p) {
            return p;
        }
        ++p;
    }
    return 0;
}

void byteswapWord(void* pDestination, void const* pSource, uint64 uCount) {

}

void byteswapLong(void* pDestination, void const* pSource, uint64 uCount) {

}

void byteswapQuad(void* pDestination, void const* pSource, uint64 uCount) {

}

} // namespace

